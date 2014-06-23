#include "button.hpp"
#include "../osHandles.h"
#include "../utils/rprintf.h"
#include "../drivers/i2c.h"
#include "../drivers/sta013.h"
#include "../drivers/pcm1774.h"



void buttonTask(void* p)
{
	OSHANDLES *osHandles = (OSHANDLES*)p;

	initialize_I2C0(100 * 1000); //initialize i2c and set clock to 100KHz
	const char portExpanderI2CAddr = 0x40;

	//initialize leds
	const char ledPortRegNum = 2; //set port 0 for output (led)
	const char ledPortConfigNum = 6; //config port 0
	const char configureForOutput = 0x00;
	i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortConfigNum, configureForOutput);

	//initialize buttons
	const char buttonPortRegNum = 1; //set port 1 for input (button)
	const char buttonPortConfigNum = 7; //config port 1
	const char configureForInput = 0xFF;
	i2cWriteDeviceRegister(portExpanderI2CAddr, buttonPortConfigNum, configureForInput);

	char dataRead;
	unsigned char oldVolume, newVolume;
	oldVolume = newVolume = 0;

	for (;;)
	{
		dataRead = i2cReadDeviceRegister(portExpanderI2CAddr, buttonPortRegNum); //read button data

		if (pdTRUE != xQueueSend(osHandles->queue.buttonData, &dataRead, 3*1000/portTICK_RATE_MS))
			rprintf("Error in QueueSend: control queue is full (waited 3 seconds)\n");
		//else rprintf("Put item on queue: %x\n", dataRead);

		//output volume with leds
		newVolume = pcm1774_GetVolume();
		if (newVolume != oldVolume)
			rprintf("Volume Level: %i\n", (newVolume*300/190));
		if (newVolume == volumeLimit0)
			i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, ledLevel0);
		else if (newVolume <= volumeLimit1)
			i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, ledLevel1);
		else if (newVolume <= volumeLimit2)
			i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, ledLevel2);
		else if (newVolume <= volumeLimit3)
			i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, ledLevel3);
		else if (newVolume <= volumeLimit4)
			i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, ledLevel4);
		else if (newVolume <= volumeLimit5)
			i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, ledLevel5);
		else if (newVolume <= volumeLimit6)
			i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, ledLevel6);
		else if (newVolume <= volumeLimit7)
			i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, ledLevel7);
		else if (newVolume <= volumeLimit8)
			i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, ledLevel8);

		oldVolume = newVolume;

		vTaskDelay(200);
	}

}

