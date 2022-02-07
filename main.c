/******************************************/
/*Ò£¿ØÖÇÄÜÐ¡³µ*/
#include <iom16v.h>
#include <contains.h>

void Device_Init(void)
{
  Control_init();
	//Display_init();
	ADC_Init();
  Ultrasonic_Init();
	HC05_Init();
	Coding_Init();
	CarUpstandInit();
	SREG|=0x80;
}

void main(void)
{
  uint ada1,ada2,ada3;
	uchar Blue_Control;
	int flag_Pid=0;
  Device_Init();
  while(1)
  {
	  //À¶ÑÀ¿ØÖÆÒÆ¶¯
	  if(Flag_Blue)
	  {
		  uart_senddata(rdata);
			Move(rdata);
			Flag_Blue=0;
		}
		Delay_ms(1);
		//ºìÍâ
		if(rdata==0x10)
		{
		  ada1 = ADC_Read(6);//a6 ×ó
			ada3 = ADC_Read(4);//a4 ÓÒ
			ada2 = ADC_Read(3);
			trace(ada1,ada3,ada2);
		}
		
		//³¬Éù²¨
		if(rdata==0x11)
		{
		  if(Ultra_Read())Ultra_Deal();
			else
			{
			  Speed_Init();
			  Move_Forward();
			}
		}
		//PID
		if(rdata==0x12)
		{
			flag_Pid=!flag_Pid;
		}
		if(flag_Pid)
		{
		  Move_pid(rdata);
		  Speed_read();
			PID();
		}
		
  }
}
