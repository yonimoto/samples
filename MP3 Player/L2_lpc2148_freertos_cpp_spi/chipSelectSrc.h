#ifndef CHIPSELECTSRC_H_
#define CHIPSELECTSRC_H_

typedef enum { sel_SD, sel_Flash, sel_MP3, sel_Nothing} CS_Signal;
void CS_Sel(CS_Signal s);
void CS_Init();

#endif
