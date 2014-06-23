#include "control.hpp"
#include "../osHandles.h"
#include "../utils/rprintf.h"
#include "../utils/cString.h"
#include "../drivers/sta013.h"    // STA013 Mp3 Decoder
#include "../drivers/pcm1774.h"
#include "../fat/ff.h"            // FAT File System Library

void controlTask(void* p)
{
	OSHANDLES *osHandles = (OSHANDLES*)p;
	sta013SetVolume(80, 0);
	int effectLevel = 0;
	bool loadComplete = false;
	bool paused = false;

	for(;;)
	{
		char data[1];
		if (pdTRUE != xQueueReceive(osHandles->queue.buttonData, data, portMAX_DELAY))
			rprintf("Error in QueueReceive: control queue is empty (waited 5 seconds)\n");
		//else rprintf("Received item from queue: %x\n", *buttonData);

		char buttonValues = *data;

		if (buttonValues & (1 << 7)) //start/play/pause
		{
			if (loadComplete)
			{
				if (paused)
				{
					rprintf("Resume\n");
					paused = false;
					sta013ResumeDecoder();
				}
				else
				{
					rprintf("Pause...");
					paused = true;
					sta013PauseDecoder();
				}
			}
			else
			{
				DIR Dir;
				FILINFO Finfo;
				FRESULT returnCode;

				#if _USE_LFN
				char Lfname[512];
				#endif

				char dirPath[] = "0:";
				if (FR_OK != (returnCode = f_opendir(&Dir, dirPath))) {
					rprintf("Invalid directory: |%s|\n", dirPath);
					continue;
				}

				for (;;)
				{
					#if _USE_LFN
					char Lfname[512];
					Finfo.lfname = Lfname;
					Finfo.lfsize = sizeof(Lfname);
					#endif

					char returnCode = f_readdir(&Dir, &Finfo);
					if ( (FR_OK != returnCode) || !Finfo.fname[0] )
						break;

					//char * fileName = Finfo.fname;
					cString fileName = Finfo.fname;
					#if _USE_LFN
					fileName = Lfname;
					#endif

					unsigned int length = fileName.getLen();
					if ( (fileName[length-4] == '.') && (fileName[length-3] == 'm')
							&& (fileName[length-2] == 'p') && (fileName[length-1] == '3'))
					{
						if (pdTRUE != xQueueSend(osHandles->queue.songName, Finfo.fname, 3*1000/portTICK_RATE_MS))
							rprintf("Error in QueueSend: song queue is full (waited 3 seconds)\n");
						else rprintf("Put song in queue: %s\n", Finfo.fname);
					}
				}
				if (pdTRUE != xQueueSend(osHandles->queue.songName, "done", 3*1000/portTICK_RATE_MS))
					rprintf("Error in QueueSend: song queue is full (waited 3 seconds)\n");
				//else rprintf("done sent\n");
				loadComplete = true;
			}
		}
		if (buttonValues & (1 << 6)) //next track
		{
			char controlData = nextTrack;
			if (pdTRUE != xQueueSend(osHandles->queue.buttonControl, &controlData, 0));
				//rprintf("Error in QueueSend: control queue is full (waited 3 seconds)\n");
			//else rprintf("Put item on queue: %x\n", controlData);
		}
		if (buttonValues & (1 << 5)) //previous track
		{
			char controlData = prevTrack;
			if (pdTRUE != xQueueSend(osHandles->queue.buttonControl, &controlData, 0));
				//rprintf("Error in QueueSend: control queue is full (waited 3 seconds)\n");
			//else rprintf("Put item on queue: %x\n", controlData);
		}
		if (buttonValues & (1 << 4)) //volume up
		{
			pcm1774_VolumeAdjust(4);
		}
		if (buttonValues & (1 << 3)) //volume down
		{
			pcm1774_VolumeAdjust(-4);
		}
		if ((buttonValues & (1 << 3)) && (buttonValues & (1 << 4))) //mute/unmute
		{
			sta013Mute();
		}
		if (buttonValues & (1 << 2)) // 3D sound effects
		{
			if (effectLevel == 0)
			{
				effectLevel = 10;
				rprintf("3D Sound Effect On\n");
			}
			else
			{
				effectLevel = 0;
				rprintf("3D Sound Effect Off\n");
			}

			pcm1774_3dSoundEffect(0,effectLevel);
		}
		if (buttonValues & (1 << 1))
		{

		}
		if (buttonValues & (1 << 0)) //stop
		{
			char controlData = stopPlayer;
			if (pdTRUE != xQueueSend(osHandles->queue.buttonControl, &controlData, 0));
				//rprintf("Error in QueueSend: control queue is full (waited 3 seconds)\n");
			//else rprintf("Put item on queue: %x\n", controlData);
			loadComplete = false;
		}
		vTaskDelay(10);
	}

}
