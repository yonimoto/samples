#include "ssp_spi.h"
#include "../System/lpc214x.h"



void initialize_SSPSPI()
{
	// TODO 1.  Write your SSP Initialization here
	PINSEL1 &= ~(3 << 2); //P0.17 (B3:B2) = 0
	PINSEL1 &= ~(3 << 4); //P0.18 (B5:B4) = 0
	PINSEL1 &= ~(3 << 6); //P0.19 (B7:B6) = 0

	PINSEL1 |= (2 << 2); // P0.17 (B3:B2) = 10 (SCK1)
	PINSEL1 |= (2 << 4); // P0.18 (B5:B4) = 10 (MISO1)
	PINSEL1 |= (2 << 6); // P0.19 (B7:B6) = 10 (MOSI1)

	SSPCR0 = (7 << 0);	// 8-bit payload size
	SSPCR1 = (1 << 1); // Enable SSP

	setSSPSPI_Divider(4);
}

char rxTxByteSSPSPI(char data)
{
	// TODO 2.  Write your SSP Exchange Byte Function here
	SSPDR = data;
	while (SSPSR & (1 << 4)); // wait until not busy

	return SSPDR;
}

void setSSPSPI_Divider(unsigned int divider)
{
	// TODO 3.  Write SSP Divider setting function here
	SSPCPSR = divider;
}
