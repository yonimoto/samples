#include "userInterface.hpp"

#include <stdlib.h>                 // atoi()
#include <stdio.h>                  // printf() or iprintf()
#include <string.h>                 // strtok()

#include "osHandles.h"
#include "utils/rprintf.h"
#include "drivers/uart0.h"

#define MATCH(a,b) (0 == strcmp(a,b))
void getUartLine(char* uartInput);
void printLine()
{
	rprintf("------------------------------------------------------------------------------------\n");
}



void uartUI(void *pvParameters)
{
	OSHANDLES *osHandles = (OSHANDLES*)pvParameters;
	char uartInput[128];

	for (;;)
	{
		rprintf("LPC: ");
		memset(uartInput, 0, sizeof(uartInput));
		getUartLine(uartInput);
		if (strlen(uartInput) <= 0){
			continue;
		}

		char *command = strtok(uartInput, " ");

		if (MATCH(command, "echo")){
			char *echoBack = strtok(NULL, "");
			rprintf("Echo: %s\n", echoBack);
		}
#if configGENERATE_RUN_TIME_STATS
		else if(MATCH(command, "CPU")) {
			char buff[512];
			vTaskGetRunTimeStats(buff, 0);
			rprintf("%s", buff);
		}
		else if(MATCH(command, "CPUr")) {
			char buff[512];
			vTaskGetRunTimeStats(buff, 1);
			rprintf("%s", buff);
		}
		else if(MATCH(command, "CPUn")) {
			int delayTimeMs = atoi(strtok(NULL, ""));
			if(delayTimeMs > 0) {
				char buff[512];
				rprintf("Delaying for %ims.  CPU Usage will be reported afterwards...\n", delayTimeMs);
				vTaskGetRunTimeStats(buff, 1);
				vTaskDelay(OS_MS(delayTimeMs));
				vTaskGetRunTimeStats(buff, 1);
				rprintf("%s", buff);
			}
			else {
				rprintf("You must define a delay time in milliseconds as parameter.\n");
			}
		}
#endif
#if configUSE_TRACE_FACILITY != 0
		else if(MATCH(command, "info")) {
			char buff[512];
			vTaskList(buff);
			rprintf("%s", buff);
		}
#endif
		else if (MATCH("HELP", command)) {
			rprintf("\n");
			printLine();
			rprintf("Command list:\n");
			printLine();
			rprintf("echo <anything>    - Echoes back anything sent.\n");
			rprintf("ls                 - Get directory listing.\n");
			rprintf("read <filename>    - Read a file.\n");
			rprintf("write <filename>   - Wrie a file.\n");
				#if configGENERATE_RUN_TIME_STATS
			rprintf("CPU                - Show tasks and CPU Allocation.\n");
			rprintf("CPUr               - Show tasks and CPU Allocation & reset counters\n");
			rprintf("CPUn <# ms>        - Reset counters, delay, then report CPU usage & reset counters\n");
				#endif
				#if configUSE_TRACE_FACILITY != 0
			rprintf("info               - Show tasks and their Stack-high watermark.\n");
				#endif
			printLine();
		}
		else if (MATCH(command,"demo")){
			rprintf("Enter your name: ");
			getUartLine(uartInput);
			rprintf("Your name is %s\n",uartInput);
		}
		else {
			rprintf("Command not recognized.\n");
		}
	}
}

void getUartLine(char* uartInput)
{
	char data;
	unsigned int uartInputPtr = 0;

	while (1)
	{
		while(!uart0GetChar(&data, portMAX_DELAY));

		if (data == '\n'){
			uartInput[uartInputPtr] = 0;
			uart0PutChar('\n', 100);
			break;
		}
		else if (data == '\r'){
			// Ignore it
		}
		else if (data == '\b')
		{
			if (uartInputPtr > 0){
				uartInputPtr--;
				rprintf("\b \b");
			}
			else {					// Nothing to backspace
				uart0PutChar(7, 0);	// ASCII Char 7 is for bell
			}
		}
		/*
		// If you want +/- keys to be directly considered as commands:
		else if(uartInputPtr == 0 && (data == '+' || data == '-')) {
			uartInput[0] = data;
			uartInput[1] = '\0';
			break;
		}
		*/
		else{
			uart0PutChar(data, 100);
			uartInput[uartInputPtr++] = data;
		}
	}
}
