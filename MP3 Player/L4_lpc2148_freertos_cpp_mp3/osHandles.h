#ifndef OSHANDLES_H
#define OSHANDLES_H

/***********************************************************************************************/
/*** This file has to be .h (C not C++) because if it is hpp, then C-code can't include this ***/
/***********************************************************************************************/


#include "./FreeRTOS/FreeRTOS.h"	// Includes for RTOS & Other services provided by it:
#include "./FreeRTOS/task.h"
#include "./FreeRTOS/queue.h"
#include "./FreeRTOS/semphr.h"


// A pointer to OSHANDLES is all that is needed for any file/task to access
// Semaphores, task handles, queue handles etc...
// Pointer can be passed to task as pvParameter
typedef struct
{
	struct {
		// TODO 1.  Add a Queue Handle for a "song name"
		xQueueHandle songName;
		xQueueHandle buttonData;
		xQueueHandle buttonControl;
	}queue;

	struct {
		xTaskHandle userInterface;
		xTaskHandle mp3;
		xTaskHandle buttons;
		xTaskHandle controls;
	}task;

	struct {
		xSemaphoreHandle SPI;
	}lock;
}OSHANDLES;



#endif /* osHandles */
