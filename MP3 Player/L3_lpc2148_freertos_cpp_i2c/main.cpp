#include "osHandles.h"              // Includes all FreeRTOS Files

#include "System/cpu.h"             // Initialize CPU Hardware
#include "drivers/watchdog.h"		// Watchdog startup delay
#include "drivers/uart0.h"       	// Initialize UART
#include "utils/rprintf.h"			// Reduced printf.  STDIO uses lot of FLASH space & Stack space
#include "fat/diskio.h"				// disk_timerproc()

#include "tasks/userInterface.hpp"	// User interface functions to interact through UART
#include "utils/timer.hpp"			// FreeRTOS based Timer class
#include "drivers/i2c.h"

// TODO 0.   In Eclipse, go to "Tasks" and complete the TODOs to input your SPI driver and read SD Card File System



/// This function is required for File System Disk Timeout that should run every 10ms
void FatFsDiskTimer(void* p)
{
	disk_timerproc();
}

void blinkTask(void* p)
{
	const char portExpanderI2CAddr = 0x40; //(i2c device address)
	const char ledPortRegNum = 2; //set port 0 for output (led)
	const char ledPortConfigNum = 6; //config port 0
	const char configureForOutput = 0x00;

	initialize_I2C0(100 * 1000); //initialize i2c and set clock to 100KHz
	i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortConfigNum, configureForOutput);

	for(;;)
	{
		i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, 0xAA);
		vTaskDelay(100);

		i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, 0x55);
		vTaskDelay(100);
	}
}

void button2LED(void *p)
{
	initialize_I2C0(100 * 1000); //initialize i2c and set clock to 100KHz
	const char portExpanderI2CAddr = 0x40; //(i2c device address)

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

	char dataRead = 0;
	for (;;)
	{
		dataRead = i2cReadDeviceRegister(portExpanderI2CAddr, buttonPortRegNum); //read button data
		i2cWriteDeviceRegister(portExpanderI2CAddr, ledPortRegNum, dataRead); //turn on appropriate leds
		vTaskDelay(10);
	}
}

/* INTERRUPT VECTORS:
 * 0:    OS Timer Tick
 * 1:    Not Used
 * 2:    UART0 Interrupt
 * 3:    Not Used
 * 4:    I2C
 * 5-16: Not Used
 */
int main (void)
{
	OSHANDLES SysHandles;        // Should contain all OS Handles

	cpuSetupHardware();          // Setup PLL, enable MAM etc.
	watchdogDelayUs(1000*1000);  // Some startup delay
	uart0Init(38400, 64);        // 64 is size of Rx/Tx FIFO

	// Use polling version of uart0 to do printf/rprintf before starting FreeRTOS
	rprintf_devopen(uart0PutCharPolling);
	cpuPrintMemoryInfo();

	// Open interrupt-driven version of UART0 Rx/Tx
	rprintf_devopen(uart0PutChar);
	watchdogDelayUs(1000*1000);

	/** Create timer needed for SD Card I/O */
	Timer diskTimer(FatFsDiskTimer, 10, TimerPeriodic);
	diskTimer.start();

	/** Create any Queues and Mutexes **/
	SysHandles.lock.SPI = xSemaphoreCreateMutex();


	// Use the WATERMARK command to determine optimal Stack size of each task (set to high, then slowly decrease)
	// Priorities should be set according to response required
	if (
		pdPASS != xTaskCreate( uartUI, (signed char*)"Uart UI", STACK_BYTES(1024*6), &SysHandles, PRIORITY_LOW, &SysHandles.task.userInterface )
		||
		//pdPASS != xTaskCreate( blinkTask, (signed char*)"blink", STACK_BYTES(1024), &SysHandles, PRIORITY_LOW, &SysHandles.task.blink )
		pdPASS != xTaskCreate( button2LED, (signed char*)"buttonLed", STACK_BYTES(1024), &SysHandles, PRIORITY_LOW, &SysHandles.task.buttonLed )
	){
		rprintf("ERROR:  OUT OF MEMORY: Check OS Stack Size or task stack size.\n");
	}

	// Start FreeRTOS to begin servicing tasks created above, vTaskStartScheduler() will not "return"
	rprintf("\n-- Starting FreeRTOS --\n");
	vTaskStartScheduler();

	// In case OS is exited:
	rprintf_devopen(uart0PutCharPolling);
	rprintf("ERROR: Unexpected OS Exit!\n");

	return 0;
}

