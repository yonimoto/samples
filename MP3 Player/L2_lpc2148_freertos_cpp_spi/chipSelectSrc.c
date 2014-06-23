#include "chipSelectSrc.h"
#include "System/lpc214x.h"

void CS_Sel(CS_Signal s)
{
	IOSET0 = (1 << 10); /* SD:    High to Deselect */
	IOSET0 = (1 << 12); /* Flash: High to Deselect */
	IOCLR1 = (1 << 29); /* MP3:   Low to Deselect */

	switch(s)
	{
		case sel_SD:	IOCLR0 = (1 << 10); break; /* Active Low = LOW to sel */
		case sel_Flash:	IOCLR0 = (1 << 12); break; /* Active Low = LOW to sel */
		case sel_MP3:	IOSET1 = (1 << 29); break; /* Active High = HIGH to sel */

		case sel_Nothing:
		default:
			break;
	}
}


void CS_Init()
{
	IODIR0 |= (1 << 10); // SD: P0.10
	IODIR0 |= (1 << 12); // Flash: P0.12
	IODIR1 |= (1 << 29); // MP3: P1.29

	CS_Sel(sel_Nothing);

	CS_Sel(sel_SD);
	CS_Sel(sel_Flash);
	CS_Sel(sel_MP3);
	CS_Sel(sel_Nothing);
}
