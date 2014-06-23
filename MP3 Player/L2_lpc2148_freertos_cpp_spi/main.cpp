#include "osHandles.h"              // Includes all FreeRTOS Files

#include "System/cpu.h"             // Initialize CPU Hardware
#include "drivers/watchdog.h"		// Watchdog startup delay
#include "drivers/uart0.h"       	// Initialize UART
#include "utils/rprintf.h"			// Reduced printf.  STDIO uses lot of FLASH space & Stack space
#include "fat/diskio.h"				// disk_timerproc()

#include "tasks/userInterface.hpp"	// User interface functions to interact through UART
#include "utils/timer.hpp"			// FreeRTOS based Timer class


// TODO 0.   In Eclipse, go to "Tasks" and complete the TODOs to input your SPI driver and read SD Card File System



/// This function is required for File System Disk Timout that should run every 10ms
void FatFsDiskTimer(void* p)
{
	disk_timerproc();
}



/* INTERRUPT VECTORS:
 * 0:    OS Timer Tick
 * 1:    Not Used
 * 2:    UART0 Interrupt
 * 3:    Not Used
 * 4:    Not Used
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

