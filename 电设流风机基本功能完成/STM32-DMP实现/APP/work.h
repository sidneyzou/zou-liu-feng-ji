#ifndef __WORK_H
#define	__WORK_H

#include "stm32f10x.h"

extern u8 mode;
extern int16_t mot1,mot2,mot3,mot4;

extern float rollmax;

void RUN(void);
void Get_mode(void);

#endif
