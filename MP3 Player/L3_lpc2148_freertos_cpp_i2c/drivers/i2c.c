#include "i2c.h"
#include "../System/lpc214x.h"
#include "../System/sysConfig.h"  // Need PCLK to setup I2C Rate.

#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/Semphr.h"
#include "../FreeRTOS/task.h"     // runTimeStats() functions
#include "../utils/rprintf.h"


// TODO 4.  Declare global Variables that are needed to execute I2C Function
char mSlaveAddr = 0;
char mSlaveReg = 0;
char mSlaveData = 0;
int  mTrxLen = 0;
int  mRxLen = 0;
int  mReadNotWrite = 0;

// Utility functions for I2C communication/state machine
#define clearSIBit() (I2CONCLR = (1 << 3))
#define setStartBit() (I2CONSET = (1 << 5))
#define clearStartBit() (I2CONCLR = (1 << 5))
#define setStopBit() (I2CONSET = (1 << 4))
#define clearStopBit() (I2CONCLR = (1 << 4))

xSemaphoreHandle i2cMutex;
xSemaphoreHandle i2cReadCompleteSignal;

// TODO 6.  Observe: Here is I2C State Machine Function that we need to execute
//					to complete I2C transaction.  This type of function with return
//					status is important because this will enable us to use interrupts
typedef enum { busy, readComplete, writeComplete} i2cStateMachineStatus;

void I2C0_ISR()__attribute__((interrupt));
i2cStateMachineStatus i2c_StateMachine();

void initialize_I2C0(unsigned int i2cFrequency)
{
	// TODO 1.  Write your I2C Initialization here
	PINSEL0 &= ~(0xF << 4);  //clear bits 7:4 (P0.3, P0.2)
	PINSEL0 |= (5 << 4); //set bits 7:4 to 0101(SDA0, SLC0)

	//Enable I2C (I2EN) and clear other bits
	I2CONSET = (1 << 6);

	//set i2c SLC frequency
	unsigned int i2cDivider = (PCLK / i2cFrequency);
	I2SCLH = i2cDivider/2;
	I2SCLL = i2cDivider/2;

	VICIntEnable = (1 << 9);	// enable i2c interrupts
	VICVectCntl4 = (9 << 0) | (1 << 5);	// enable vector #4 and set i2c for this vector
	VICVectAddr4 = &I2C0_ISR; // set address of ISR

	i2cMutex = xSemaphoreCreateMutex();	// create i2c semaphore
	vSemaphoreCreateBinary(i2cReadCompleteSignal); // create binary semaphore for i2c read
	xSemaphoreTake(i2cReadCompleteSignal, 0); // take binary semaphore immediately
}

void I2C0_ISR()
{
	int notUsed = 0;
	i2cStateMachineStatus status = i2c_StateMachine();

	if (status == writeComplete)
		xSemaphoreGiveFromISR(i2cMutex, &notUsed);

	if (status == readComplete)
		xSemaphoreGiveFromISR(i2cReadCompleteSignal, &notUsed);

	VICVectAddr = 0;
}

char i2cReadDeviceRegister(char deviceAddress, char registerAddress)
{
	if(xSemaphoreTake(i2cMutex, 20))
	{
		mSlaveAddr = deviceAddress;
		mSlaveReg = registerAddress;
		mRxLen = 1;
		mReadNotWrite = 1;

		setStartBit();
		char readData = 0;

		if(xSemaphoreTake(i2cReadCompleteSignal, 3))
			readData = mSlaveData;

		xSemaphoreGive(i2cMutex);
		return readData;
	}
	else return 0;
}

char i2cWriteDeviceRegister(char deviceAddress, char registerAddress, char value)
{
	if(xSemaphoreTake(i2cMutex, 20))
	{
		mSlaveAddr = deviceAddress;
		mSlaveReg = registerAddress;
		mSlaveData = value;
		mTrxLen = 1;
		mReadNotWrite = 0;

		setStartBit();
		return 1;
	}
	else return 0;
}

i2cStateMachineStatus i2c_StateMachine()
{
	i2cStateMachineStatus state = busy;

	// TODO 7.  Perform I2C transactions and return appropriate status

	switch(I2STAT)
	{
	case 0x08:
		I2DAT = mSlaveAddr;
		I2DAT &= ~(1 << 0);
		break;
	case 0x10:
		I2DAT = mSlaveAddr;
		I2DAT |= (1 << 0);
	    break;
	case 0x18:
	    clearStartBit();
	    I2DAT = mSlaveReg;
	    break;
	case 0x28:
		if (mReadNotWrite == 0)
		{
			if (mTrxLen >= 1)
				I2DAT = mSlaveData;
			else
			{
				setStopBit();
				state = writeComplete;
			}
			mTrxLen--;
			break;
		}
		else setStartBit();
	    break;
	case 0x40:
		if (mRxLen >= 2) //reading 2 or more bytes
		{
			clearStartBit();
			I2CONSET = (1 << 2);//set AA to 1
		}
		else //reading 1 byte
		{
			clearStartBit();
			I2CONCLR = (1 << 2); //clear AA
		}
		break;
	case 0x50:
		if (mRxLen >= 2) //reading 2 or more bytes
		{
			mSlaveData = I2DAT;
			I2CONSET = (1 << 2);//set AA to 1
		}
		else //reading 1 byte
		{
			mSlaveData = I2DAT;
			I2CONCLR = (1 << 2); //clear AA
		}
		mRxLen--;
		break;
	case 0x58:
		mSlaveData = I2DAT;
		setStopBit();
		state = readComplete;
		break;
	default:
		rprintf("This should never happen! State: %x\n", I2STAT);
		setStopBit();
		state = writeComplete;
	}
	clearSIBit(); //HW will take next action
	return state;
}

