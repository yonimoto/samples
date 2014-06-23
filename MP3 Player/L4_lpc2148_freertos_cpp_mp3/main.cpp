#include "osHandles.h"              // Includes all OS Files

#include "System/cpu.h"             // Initialize CPU Hardware
#include "drivers/watchdog.h"
#include "drivers/uart0.h"       	// Initialize UART
#include "utils/rprintf.h"			// Reduced printf.  STDIO uses lot of FLASH space & Stack space
#include "fat/diskio.h"


#include "general/userInterface.hpp"	// User interface functions to interact through UART
#include "MP3/mp3.hpp"
#include "MP3/button.hpp"
#include "MP3/control.hpp"
#include "utils/timer.hpp"

void FatFsDiskTimer(void* p)
{
	disk_timerproc();
}

/**************************************************************************************
 * TODO 0.  In Eclipse, go to "Tasks" and complete the TODOs to play an mp3 song
 * Hints:
 * 			- SPI & I2C Driver is all setup for you
 * 			- All Drivers and Peripherals are already initialized inside Uart UI task
 * 			- Read "MP3 Decoder" Lab in your Lab Manual
 * ************************************************************************************/

/* INTERRUPT VECTORS:
 * 0:    OS Timer Tick
 * 1:    Not Used
 * 2:    UART0 Interrupt
 * 3:    Not Used
 * 4:    I2C0 Interrupt
 * 5-16: Not Used
 */
int main (void)
{
	OSHANDLES SysHandles;        // Should contain all OS Handles

	cpuSetupHardware();          // Setup PLL, enable MAM etc.
	watchdogDelayUs(1000*1000);  // Some startup delay
	uart0Init(38400, 128);       // 128 is size of Rx/Tx FIFO

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
	// TODO 2.  Create the "song name" Queue here
	SysHandles.queue.songName = xQueueCreate(10, 13); // song names from UI to mp3
	SysHandles.queue.buttonData = xQueueCreate(10,1); // button values from button to control (pause/resume/volume)
	SysHandles.queue.buttonControl = xQueueCreate(1,1); // button values from control to mp3 (next/previous track)

	// TODO 6.  Create a "Button Task" or timer preferably to watch for button presses
	// and control volume, stop/pause/play etc.

	// Use the WATERMARK command to determine optimal Stack size of each task (set to high, then slowly decrease)
	// Priorities should be set according to response required
	if (
		pdPASS != xTaskCreate( uartUI, (signed char*)"Uart UI", STACK_BYTES(1024*6), &SysHandles, PRIORITY_LOW,  &SysHandles.task.userInterface )
		||
		pdPASS != xTaskCreate( mp3Task, (signed char*)"MP3", STACK_BYTES(1024*6), &SysHandles, PRIORITY_HIGH,  &SysHandles.task.mp3 )
		||
		pdPASS != xTaskCreate( buttonTask, (signed char*)"ButtonTask", STACK_BYTES(1024), &SysHandles, PRIORITY_LOW,  &SysHandles.task.buttons )
		||
		pdPASS != xTaskCreate( controlTask, (signed char*)"ControlTask", STACK_BYTES(1024*2), &SysHandles, PRIORITY_LOW,  &SysHandles.task.controls )
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

/********************************************************************************
 * TODO 7.  Hints (Please read)
 *
 *  I'm totally lost!
 *  	Start from TODOs, and that should enable you to play ONE song.
 *  	Get one song to play, the rest of the pieces will fall in place.
 *      Read this documentation on how to open and read a file:
 *      	http://elm-chan.org/fsw/ff/00index_e.html
 *
 *	How do I get listing of MP3 files on SD Card?
 *		- Reference "ls" command at uartUI, it does file listing.  You need to
 *			reuse this code, and modify it so that you can use it to scan for
 *			MP3 files in your SD Card root folder.
 *			Don't get scared of the code, remove iprintf, and see how simple
 *			the code is.  File name is at this variable: Finfo.fname[0]
 *
 *  Ok, I'm able to read a file, how exactly do I play it?
 *			Once you read a file and store to buffer, such as "char buff[512]"
 *			you simply need to "Hand it off" to MP3 Decoder called STA013
 *			This macro tells you when MP3 needs data: STA013_NEEDS_DATA()
 *			Once you detect that MP3 decoder needs data, simply transfer it
 *			using your SPI transfer function.
 *
 * *****************************************************************************/
