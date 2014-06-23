#include "userInterface.hpp"

#include <stdlib.h>                 // atoi()
#include <stdio.h>                  // printf() or iprintf()
#include <string.h>                 // strtok()


#include "osHandles.h"
#include "utils/rprintf.h"     // rprintf
#include "fat/ff.h"            // FAT File System Library
#include "fat/diskio.h"        // Disk IO Initialize SPI Mutex

#include "drivers/uart0.h"     // uart0GetChar()
#include "drivers/ssp_spi.h"
#include "chipSelectSrc.c"

#define MATCH(a,b) (0 == strcmp(a,b))
void getUartLine(char* uartInput);
void printLine()
{
	rprintf("------------------------------------------------------------------------------------\n");
}

void writeEnable()
{
	CS_Sel(sel_Flash);
	char opCode = 0x06;
	rxTxByteSSPSPI(opCode);
	CS_Sel(sel_Nothing);
}

void globalProtectionDisable()
{
	CS_Sel(sel_Flash);
	char opCode = 0x01;
	rxTxByteSSPSPI(opCode);
	rxTxByteSSPSPI(0);
	CS_Sel(sel_Nothing);
}

void waitUntilNotBusy()
{
	CS_Sel(sel_Flash);
	char opCode = 0x05;
	rxTxByteSSPSPI(opCode);
	while (char mID=rxTxByteSSPSPI(0))
	{
		if (mID == 0x11) continue;
		else break;
	}
	CS_Sel(sel_Nothing);
}

void eraseBlock(int address)
{
	typedef union
	{
		int raw;
		struct{
			char byte0;
			char byte1;
			char byte2;
			char byte3;
		};
	}intSplitter;

	intSplitter splitter;
	splitter.raw = address;
	CS_Sel(sel_Flash);
	char opCode = 0x20;
	rxTxByteSSPSPI(opCode);
	rxTxByteSSPSPI(splitter.byte2);
	rxTxByteSSPSPI(splitter.byte1);
	rxTxByteSSPSPI(splitter.byte0);
	CS_Sel(sel_Nothing);
}

void writePage(int address,char *data,int length)
{
	typedef union
	{
		int raw;
		struct{
				char byte0;
				char byte1;
				char byte2;
				char byte3;
			   };
	}IntSplitter;
	IntSplitter splitter;
	splitter.raw = address ;
	CS_Sel(sel_Flash);
	char opCode = 0x02;
	rxTxByteSSPSPI(opCode);

	rxTxByteSSPSPI(splitter.byte2);
	rxTxByteSSPSPI(splitter.byte1);
	rxTxByteSSPSPI(splitter.byte0);

	while(length--)
	{
		rxTxByteSSPSPI(*data++);
	}
	rxTxByteSSPSPI(0);
	CS_Sel(sel_Nothing);
}

void readPage (int address)
{
	typedef union
	{
		int raw;
		struct
		{
			char byte0;
			char byte1;
			char byte2;
			char byte3;
		};
	}intSplitter;
	intSplitter splitter;
	splitter.raw = address;
	char opRead = 0x0B; //op code for flash read, requires a don't care
						//after writing 3 address bytes

	rxTxByteSSPSPI(opRead); //op code to read array
	rxTxByteSSPSPI(splitter.byte2); //send address msb
	rxTxByteSSPSPI(splitter.byte1);
	rxTxByteSSPSPI(splitter.byte0); //send address lsb
	rxTxByteSSPSPI(0); //don't care required by op code

	bool moreData = true;
	char temp;
	while(moreData)
	{
		temp = rxTxByteSSPSPI(0);
		rprintf("%c",temp);
		if (temp == '\0')
			moreData = false;
	}
	rprintf("\n");
}

void uartUI(void *pvParameters)
{
	OSHANDLES *osHandles = (OSHANDLES*)pvParameters;
	char uartInput[128];

	CS_Init();
	initialize_SSPSPI();
	diskio_initializeSPIMutex(&(osHandles->lock.SPI));
	initialize_SdCardSignals();

	FATFS SDCard; // Takes ~550 Bytes
	if(FR_OK != f_mount(0, &SDCard)) { // Mount the Card on the File System
		rprintf("WARNING: SD CARD Could not be mounted!\n");
	}

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
		// TODO 5.  Add your Flash Read/Write Functions here
		else if (MATCH(command, "ReadSig")){
			int loc = atoi(strtok(0, ""));
			rprintf("Loc specified was %i\n", loc);

			if (xSemaphoreTake(osHandles->lock.SPI, 3000))
			{
				CS_Sel(sel_Flash);

				char opMfgRead = 0x9F;
				rxTxByteSSPSPI(opMfgRead);

				char mID  = rxTxByteSSPSPI(0);
				char dID1 = rxTxByteSSPSPI(0);
				char dID2 = rxTxByteSSPSPI(0);
				char eID  = rxTxByteSSPSPI(0);

				rprintf("ID Info: %X, %X, %X, %X\n", mID, dID1, dID2, eID);

				CS_Sel(sel_Nothing);
				xSemaphoreGive(osHandles->lock.SPI);
			}
			else
			{
				rprintf("ReadSig: Error: Could not take SPI semaphore in time\n");
			}
		}
		else if (MATCH(command, "ReadFlash"))
		{
			int address = { atoi(strtok(0, "")) }; //get address

			if (xSemaphoreTake(osHandles->lock.SPI, 3000))
			{
				CS_Sel(sel_Flash);
				readPage(address);
				CS_Sel(sel_Nothing);
				xSemaphoreGive(osHandles->lock.SPI);
			}
			else
			{
				rprintf("ReadFlash: Error: Could not take SPI semaphore in time\n");
			}
		}
		else if (MATCH(command, "WriteFlash"))
		{
			int loc = atoi(strtok(0," "));
			char *data  = strtok(0, "");
			int dataSize = strlen(data);
			if(xSemaphoreTake(osHandles->lock.SPI,3000))
			{
			CS_Sel(sel_Flash);
			writeEnable();
			globalProtectionDisable();

			waitUntilNotBusy();
			writeEnable();
			eraseBlock(loc);

			waitUntilNotBusy();
			writeEnable();
			writePage(loc, data, dataSize);
			CS_Sel(sel_Nothing);
			xSemaphoreGive(osHandles->lock.SPI);
			}
			else
			{
				rprintf("WriteFlash: Error: Could not take SPI semaphore in time\n");
			}
		}
		else if(MATCH("ls", command))
		{
			DIR Dir;
			FILINFO Finfo;
			FATFS *fs;
			FRESULT returnCode = FR_OK;

			unsigned int fileBytesTotal, numFiles, numDirs;
			fileBytesTotal = numFiles = numDirs = 0;
			#if _USE_LFN
			char Lfname[512];
			#endif

			char dirPath[] = "0:";
			if (FR_OK != (returnCode = f_opendir(&Dir, dirPath))) {
				rprintf("Invalid directory: |%s|\n", dirPath);
				continue;
			}

			rprintf("Directory listing of: %s\n\n", dirPath);
			for (;;) {
				#if _USE_LFN
				Finfo.lfname = Lfname;
				Finfo.lfsize = sizeof(Lfname);
				#endif

				char returnCode = f_readdir(&Dir, &Finfo);
				if ( (FR_OK != returnCode) || !Finfo.fname[0])
					break;
				if (Finfo.fattrib & AM_DIR){
					numDirs++;
				}
				else{
					numFiles++;
					fileBytesTotal += Finfo.fsize;
				}
				iprintf("%c%c%c%c%c %u/%2u/%2u %2u:%2u %10lu %13s",
						(Finfo.fattrib & AM_DIR) ? 'D' : '-',
						(Finfo.fattrib & AM_RDO) ? 'R' : '-',
						(Finfo.fattrib & AM_HID) ? 'H' : '-',
						(Finfo.fattrib & AM_SYS) ? 'S' : '-',
						(Finfo.fattrib & AM_ARC) ? 'A' : '-',
						(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
						(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63, Finfo.fsize, &(Finfo.fname[0]));
				#if _USE_LFN
				iprintf(" -- %s", Lfname);
				#endif
				iprintf("\n");
			}
			iprintf("\n%4u File(s), %10u bytes total\n%4u Dir(s)", numFiles, fileBytesTotal, numDirs);

			if (f_getfree(uartInput, (DWORD*) &fileBytesTotal, &fs) == FR_OK)
			{
				iprintf(", %10uK bytes free\n", fileBytesTotal * fs->csize / 2);
			}
		}
		else if(MATCH(command, "read")) {
			char *filename = strtok(0, "");
			rprintf("File to read: |%s|\n", filename);

			FIL file;
			if(FR_OK == f_open(&file, filename, (FA_READ | FA_OPEN_EXISTING)))
			{
				char buff[513] = {0};
				unsigned int bytesRead = 0;
				rprintf("File Contents:\n");
				printLine();
				do
				{
					if(FR_OK != f_read(&file, buff, sizeof(buff)-1, &bytesRead))
					{
						rprintf("--Failed to read file after having it opened--\n");
						break;
					}
					rprintf("%s", buff);
				}while(0); // }while(bytesRead == bytesToRead); <- Don't want to read entire file in a loop
				printLine();

				f_close(&file);
				rprintf("File is closed\n");
			}
			else
			{
				rprintf("Failed to open the file\n");
			}
		}
		else if(MATCH(command, "write")) {
			char *filename = strtok(0, "");
			rprintf("File to write: |%s|\n", filename);

			FIL file;
			if(FR_OK == f_open(&file, filename, (FA_WRITE | FA_CREATE_ALWAYS)))
			{
				rprintf("File successfully opened\n");
				char buff[] = "File written using FATFS From Chen\n";

				unsigned int bytesWritten = 0;
				if(FR_OK != f_write(&file, buff, sizeof(buff), &bytesWritten))
				{
					rprintf("Failed to write file\n");
				}

				f_close(&file);
				rprintf("File is closed\n");
			}
			else
			{
				rprintf("Failed to open the file\n");
			}
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
		else if (MATCH("HELP", command) || MATCH("help", command))
		{
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
			rprintf("ReadSig            - Display Flash Mfg Information.\n");
			rprintf("WriteFlash <addr> <data>\n");
			rprintf("ReadFlash <addr>\n");
			printLine();
		}
		else
		{
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
