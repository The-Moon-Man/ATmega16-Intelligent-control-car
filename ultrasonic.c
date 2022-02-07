#include <iom16v.h>
#include <macros.h>
#include "ultrasonic.h"
#include <contains.h>
#include <delay.h>

// �����������룬��λcm
float Ultrasonic_Distance = 0;
//��ʱ������
int Time_Ultrasonic=0;
int Ultrasonic_flag=0;

int Ultra_Read(void)												//�����źſ�ʼ������
{
 		//������ƽ
 		ReSet_Bit(PORTA, PA5);
	  Set_Bit(PORTA, PA5);
	  Delay_10us(1);
	  ReSet_Bit(PORTA, PA5);
		//10ms�ڽ��м�⣬��������
		TCNT1 = 0;
	  ICR1=0;
		Set_Bit(TIMSK, TICIE1);
		Delay_ms(10);
		ReSet_Bit(TIMSK, TICIE1);
		
	 	Ultrasonic_Distance=(Time_Ultrasonic*1.7)/1000;
		if((Ultrasonic_Distance<15)&&(Ultrasonic_Distance!=0)&&(Ultrasonic_Distance>0.5))
		{ 
		  uart_senddata(0x01);
			return 1;
		}
		else 
		{
		  uart_senddata(0x02);
		  return 0;
		}
}
void Ultra_Deal(void)    											//������������
{
      Move_Reverse();
			OCR2=0;
      OCR0=155;
			Delay_ms(200);
			Speed_Init();
			Move_Forward();
			Speed_Stop();
}

void Ultrasonic_Init(void)															 //ʹ����PA5�����ź�,PD6���벶��
{
	Set_Bit(DDRA, PA5);
	ReSet_Bit(PORTA, PA5);
	ReSet_Bit(DDRD, PD6);
	//������ʱ��
	TCCR1A=0X00;
	TCCR1B=(1<<ICES1)|(0<<CS12)|(0<<CS11)|(1<<CS10);			 //1��Ƶ
	ReSet_Bit(TIMSK, TICIE1);															 //����Ƚ�ƥ���ж�ʹ��
}


#pragma interrupt_handler Ultrasonic_CAPT_isr:iv_TIM1_CAPT
void Ultrasonic_CAPT_isr(void)
{
	static uint high_time = 0; // ����ߵ͵�ƽ�ĳ���ʱ��
	
  if ((!(TCCR1B & (1<<ICES1)))&&(Ultrasonic_flag==0)) // �����β�������½���
	{
		
		high_time =ICR1; // ��¼�ߵ�ƽ����ʱ��if (TCCR1B & _BV(ICES1)) // �����β�������½���
		Ultrasonic_flag=1;
	}
	else
	{
	  ICR1=0;
	  TCNT1=0;
		Ultrasonic_flag=0;

	}
	
	if(Ultrasonic_flag==1)
	{
	  Time_Ultrasonic=high_time;
	}
	TCCR1B ^= (1<<ICES1);
	TIFR = (1<<ICF1);
}