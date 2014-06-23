#include "uart0.h"
#include "System/lpc214x.h"	// PCLK
#include <stdlib.h>
#include "FreeRTOS\task.h"
#include "FreeRTOS\FreeRTOS.h"


char uart0Init (unsigned long baudRate)
{
	// TODO 1.  Initialize UART with the given baud rate
	// Note: PCLK is already defined
	PINSEL0 &= ~(0xF << 0);	//unset B3:B0
	PINSEL0 |= (5 << 0);	//set B3:B0 to 0101 to select UART0 Rx/Tx

	U0LCR |= (1 << 7); //set DLAB = 1 to modify baudrate
	//Modify baudrate
	U0DLL = PCLK/(16*baudRate);
	U0DLM = 0;
	U0LCR &= ~(1 << 7); //set DLBA = 0 to access Rx/Tx registers

	U0LCR = 3; //sets LCR to 8-bit word length, 1 stop bit, no parity
}

unsigned long uart0GetChar (char *pcRxedChar, portTickType xBlockTime)
{
	// TODO 2.  Wait until UART captures a char, and store it to pcRxedChar
	// Ignore the Block time variable
	while (!(U0LSR & (1<<0)));
	*pcRxedChar = U0RBR;
}

unsigned long uart0PutChar (char cOutChar, portTickType xBlockTime)
{
	// TODO 3.  Send a char out the UART
	// Ignore the Block time variable
	while (!(U0LSR & (1<<5)));
	U0THR = cOutChar;
}
