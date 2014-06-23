#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#define ledLevel8 0xFF
#define ledLevel7 0x7F
#define ledLevel6 0x3F
#define ledLevel5 0x1F
#define ledLevel4 0x0F
#define ledLevel3 0x07
#define ledLevel2 0x03
#define ledLevel1 0x01
#define ledLevel0 0x00
#define volumeLimit8 0x3F
#define volumeLimit7 0x3E
#define volumeLimit6 0x35
#define volumeLimit5 0x2C
#define volumeLimit4 0x23
#define volumeLimit3 0x1A
#define volumeLimit2 0x11
#define volumeLimit1 0x08
#define volumeLimit0 0x00

void buttonTask(void* p);

#endif /* BUTTON_HPP_ */
