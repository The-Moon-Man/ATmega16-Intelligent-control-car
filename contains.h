#ifndef __CONTAINS_H
#define __CONTAINS_H

#include <delay.h>
#include <macros.h>
#include <mpu6050.h>
#include <hc05.h>
#include <CodingPid.h>

#include <ultrasonic.h>
#include <control.h>
#include <adc.h>
#include <smg.h>

typedef unsigned char uchar;
typedef unsigned int uint;


#define key1 PIND&(1<<PD2)

#define Set_Bit(val, bitn)    (val |=(1<<(bitn))) 
#define ReSet_Bit(val, bitn)    (val&=~(1<<(bitn))) 
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) ) 

extern uchar rdata;
extern uchar Flag_Blue;


uint ADC_Read(uint ch);
void Ultra_Deal(void);
void Display(uchar i, uchar j);
void uart_senddata(uchar data);
void Move(uchar x);
void Move_try(uchar x);
void trace(uint adal,uint adar,uint adam);
void Move_pid(uchar x);

#endif