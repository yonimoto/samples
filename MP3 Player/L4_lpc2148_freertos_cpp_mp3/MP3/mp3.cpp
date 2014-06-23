#include "mp3.hpp"
#include "../osHandles.h"
#include "../fat/ff.h"
#include "../utils/rprintf.h"
#include "../utils/vector.hpp"
#include "../utils/cString.h"
#include "../drivers/sta013.h"    // STA013 Mp3 Decoder
#include "../drivers/ssp_spi.h"

void mp3Task(void* p)
{
	OSHANDLES *osHandles = (OSHANDLES*)p;
	VECTOR<cString> songs;
	cString vSongName;
	cString doneStr = "done";
	bool done = false;

	for(;;)
	{
		FIL fileHandle;
		// TODO 4.  Perform xQueueReceive from "song name" queue
		char fileName[13];

		while(done == false)
		{
			if (pdTRUE != xQueueReceive(osHandles->queue.songName, fileName, portMAX_DELAY))
				rprintf("Error in QueueReceive: song queue is empty (waited infinitely!)\n");
			else rprintf("Received item from queue: %s\n", fileName);
			if (doneStr.compareTo(fileName) != true)
			{
				vSongName = fileName;
				songs.push_back(vSongName);
			}
			else done = true;
		}

		rprintf("Playing %s\n", songs[0].c_str());

		// TODO 5.  Try to open the file, and read a block from file
		if (FR_OK != f_open (&fileHandle, songs[0].c_str(), FA_OPEN_EXISTING | FA_READ))
			rprintf("Error: unable to open file\n");
		//else rprintf("File Open Successful!\n");

		// Don't forget to #include "fat/ff.h" to read files.
		// You do not need to take SPI Semaphore to read a file since FAT FS will do that

		// Once you read part of the file, stream it over to the MP3 Decoder
		// Here is some sample code assuming bytesRead is the size of buffer you read
		char buffer[512];
		unsigned int bytesRead = 0;
		unsigned int bytesStreamed = 0;

		// See "Mp3 Decoder" lab for more psuedocode on how to continuously read
		// a file until last block in file.
		while(1)
		{
			if (FR_OK != f_read(&fileHandle, &buffer, 512, &bytesRead))
				rprintf("Error: unable to read data\n");
			bytesStreamed = 0;
			if(xSemaphoreTake(osHandles->lock.SPI, 3000))
			{
				SELECT_MP3_CS();
				while (bytesStreamed < bytesRead)
				{
					if(STA013_NEEDS_DATA())
					{
						rxTxByteSSPSPI(buffer[bytesStreamed++]);
					}
					else vTaskDelay(1);
				}
				DESELECT_MP3_CS();
				xSemaphoreGive(osHandles->lock.SPI);

				char buttonCommand[1];
				if (pdTRUE != xQueueReceive(osHandles->queue.buttonControl, buttonCommand, 0))
					*buttonCommand = 0;//rprintf("Error in QueueReceive: button command queue is empty (waited infinitely!)\n");
				//else rprintf("Received item from queue: %x\n", *buttonCommand);
				if ( (512 != bytesRead) || (*buttonCommand == nextTrack) )
				{
					songs.rotateRight();
					*buttonCommand = 0;
					break;
				}
				else if (*buttonCommand == prevTrack)
				{
					songs.rotateLeft();
					*buttonCommand = 0;
					break;
				}
				else if(*buttonCommand == stopPlayer)
				{
					*buttonCommand = 0;
					songs.clear();
					done = false;
					break;
				}
			}
		}
		f_close(&fileHandle);
	}
}
