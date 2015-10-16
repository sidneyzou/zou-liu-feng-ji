#ifndef __OLED_DISPLAY_H
#define	__OLED_DISPLAY_H

#include "stm32f10x.h"

void Oled_Display(void);
void Oled_Displayinit(void);
void mode2_Display(u8 lenth);
void mode3_Display(u8 angle);
void mode5_Display(u8 r);
void clear(void);

#endif
