#ifndef _CONTROL_H
#define _CONTROL_H
#include <iom16v.h>

void timer0_init(void);
void timer2_init(void);

void Control_init(void);
void Speed_Init(void);
void Move_Forward(void);
void Speed_Stop(void);
void Move_Reverse(void);

#endif