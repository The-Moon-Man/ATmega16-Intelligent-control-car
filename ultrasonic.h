#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

int Ultra_read(void);
void Ultrasonic_Init(void);
void Ultra_Deal(void);  

extern float Ultrasonic_Distance;

#endif