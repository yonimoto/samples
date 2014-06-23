#include "osHandles.h"              // Includes all FreeRTOS Files

#include "System/cpu.h"             // Initialize CPU Hardware
#include "drivers/watchdog.h"		// Watchdog startup delay
#include "drivers/uart0.h"       	// Initialize UART
#include "utils/rprintf.h"			// Reduced printf.  STDIO uses lot of FLASH space & Stack space
#include "tasks/userInterface.hpp"	// User interface functions to interact through UART

// TODO 0.   In Eclipse, go to "Tasks" and complete the TODOs to input your SPI driver and read SD Card File System

/*typedef union
		{
			unsigned int full32bit;
			struct
			{
				char P0_0 : 2;
				char P0_1 : 2;
				char P0_2 : 2;
				char P0_3 : 2;
				char P0_4 : 2;
				char P0_5 : 2;
				char P0_6 : 2;
				char P0_7 : 2;
			};
		}__attribute__((packed)) MyPinSelType;
*/

void SD_Status(void *p)
{
	OSHANDLES *osHandles = (OSHANDLES*)p;
	PINSEL1 &= ~(1 << 0); //set P0.16 (CD) to GPIO
	IODIR0 &= ~(1 << 16); //set P0.16 to input
	PINSEL1 &= ~(1 << 10);//set P0.21 (WP) to GPIO
	IODIR0 &= ~(1 << 21); //set P0.21 to input

	for (;;)
	{
		rprintf("SD Card Detect: %i\n", !!(IOPIN0 & (1 << 16)));
		rprintf("SD Card Write-Protect: %i\n", !!(IOPIN0 & (1 << 21)));
		vTaskDelay(1000);
	}

}
void led_task(void *p)
{
	OSHANDLES *osHandles = (OSHANDLES*)p;

	/*MyPinSelType *pPinSel0 = &PINSEL0;
	pPinSel0->P0_5 = 0;
	pPinSel0->P0_6 = 0;
	*/
	const int ledPinNum = 6; //P0.6, pins 13:12

	// Init Switch on P0.5 and LED on P0.6
	PINSEL0 &= ~(0xF << 10); // Select P0.5 and P0.6 as GPIO
	IODIR0 |= (1 << 6); // Set P0.6 as output (LED)
	IODIR0 &= ~(1 << 5); // Set P0.5 as input (Switch)

	for(;;)
	{
		if(IOPIN0 & (1 << 5))
		{
			IOSET0 = (1 << ledPinNum);
		}
		else
		{
			IOCLR0 = (1 << ledPinNum);
		}
		vTaskDelay(10);
	}
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
	uart0Init(38400);

	rprintf_devopen(uart0PutChar);
	cpuPrintMemoryInfo();
	watchdogDelayUs(1000*1000);


	// Use the WATERMARK command to determine optimal Stack size of each task (set to high, then slowly decrease)
	// Priorities should be set according to response required
	if (
		pdPASS != xTaskCreate( uartUI, (signed char*)"Uart UI", STACK_BYTES(1024*6), &SysHandles, PRIORITY_LOW, &SysHandles.task.userInterface )
		||
		pdPASS != xTaskCreate( led_task, (signed char*)"LED Task", STACK_BYTES(512), &SysHandles, PRIORITY_LOW, &SysHandles.task.userInterface )
		||
		pdPASS != xTaskCreate( SD_Status, (signed char*)"SD_STATUS", STACK_BYTES(512), &SysHandles, PRIORITY_LOW, &SysHandles.task.userInterface )
	){
		rprintf("ERROR:  OUT OF MEMORY: Check OS Stack Size or task stack size.\n");
	}

	// Start FreeRTOS to begin servicing tasks created above, vTaskStartScheduler() will not "return"
	rprintf("\n-- Starting FreeRTOS --\n");
	vTaskStartScheduler();

	// In case OS is exited:
	rprintf("ERROR: Unexpected OS Exit!\n");

	return 0;
}

