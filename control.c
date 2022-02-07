#include <iom16v.h>
#include <contains.h>

#define Min_Speed 5
#define Change_Speed 5
extern float g_iCarSpeedSet;
extern float g_fBluetoothDirection;

//OCR0:PB3,left OCR1:PD3,right
void Control_init(void)
{
 	 //PDB3,PD7����ߵ�ƽ
 	 DDRB|=(1<<PB3);		 											//PB3����Ϊ���
	 ReSet_Bit(PORTB,PB3);										//PB3���0
	 DDRD|=(1<<PD7);													//PD3����Ϊ���
	 ReSet_Bit(PORTD,PD7);										//PD3���0
	 
	 DDRB|=((1<<PB0)|(1<<PB1));								//PA1,2,2��Ϊ���
	 //��ʱ��
	 timer0_init();
	 timer2_init();
	 SREG|=0X80;
	 //ռ�ձȳ�ʼֵʵ��Ϊ50.19%
	 //Ĭ����ͣ
	 Speed_Stop();

}

void timer0_init(void)
{
 	 TCCR0=(1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<COM00)|(1<<CS00)|(0<<CS02);
	 																		//����PWM���Ƚ�ƥ�䷢��ʱOC0A��λ��������TOPʱOC0���㣬1��Ƶ
	 OCR0=150;													//����Ƚ����������������ֵ�Ƚ�
}

void timer2_init(void)
{
 	 TCCR2=(1<<WGM21)|(1<<WGM20)|(1<<COM21)|(1<<COM20)|(1<<CS20)|(0<<CS22);
	 																		//����PWM���Ƚ�ƥ�䷢��ʱOC0A��λ��������TOPʱOC0���㣬1��Ƶ
	 OCR2=150;													//����Ƚ����������������ֵ�Ƚ�
}

void Speed_up(int Speed)
{
   OCR0=OCR0-Speed;										//���ּ���
   OCR2=OCR2-Speed;										//���ּ���
   if(OCR0<=Min_Speed)
   {
     OCR0=Min_Speed;
   }
   if(OCR2<=Min_Speed)
   {
     OCR2=Speed;
   }
   
}
void Speed_down(int Speed)
{  
   OCR0=OCR0+Speed;										//���ּ���
   OCR2=OCR2+Speed;										//���ּ���
   if(OCR0>=255-Min_Speed)
   {
     OCR0=255-Min_Speed;
   }
    if(OCR2>=255-Min_Speed)
   {
     OCR2=255-Min_Speed;
   }
}

void Turn_left(int Speed)
{
   OCR2=OCR2-Speed;			 	 						//���ּ���
   OCR0=OCR0+Speed;										//���ּ���
   if(OCR2<=Min_Speed)
   {
     OCR2=Min_Speed;
   }
   if(OCR0>=255-Min_Speed)
   {
     OCR0=255-Min_Speed;
   }
}
void Turn_right(int Speed)
{
   OCR0=OCR0-Speed;										//���ּ���
   OCR2=OCR2+Speed;										//���ּ���
   if(OCR0<=Min_Speed)
   {
     OCR0=Min_Speed;
   }
     if(OCR2>=255-Min_Speed)
   {
     OCR2=255-Min_Speed;
   }
}
void Speed_Init(void)
{
   OCR2=150;		
	 OCR0=150;	
}

/***************************
����Ĺ��������
  ENA  IN1  IN2
  1    1    1  �ƶ�
  1    0    1  ��ת
  1    1    0  ��ת
  0    0    0  ֹͣ
	PB0-IN1
	PB1-IN2
****************************/
void Speed_Stop(void)
{
  Set_Bit(PORTB,PB0);
	Set_Bit(PORTB,PB1);
}

void Move_Forward(void)
{
	ReSet_Bit(PORTB,PB0);
	Set_Bit(PORTB,PA1);
}

void Move_Reverse(void)
{
	Set_Bit(PORTB,PB0);
	ReSet_Bit(PORTB,PB1);
}

void Move(uchar x)
{
  switch(x)
	{
	  //�ٶ�
		case 0x00:Speed_Init();	 																					break;
	  case 0x01:Move_Forward();Speed_up(Change_Speed); 									break;    						//ǰ��
		case 0x02:Turn_left(Change_Speed);																break;								//��ת
		case 0x03:Speed_down(Change_Speed);																break;								//����
		case 0x04:Turn_right(Change_Speed);																break;								//��ת
		case 0x05:Speed_Stop();																						break;								//ͣת
		//ת��
		case 0x06:Move_Forward();																					break;								//��ת
		case 0x07:Move_Reverse();																					break;								//��ת
		default  :Speed_Stop();
	}
}

void Move_pid(uchar x)
{
  switch(x)
	{
	  //�ٶ�
		case 0x00:g_iCarSpeedSet=150;g_fBluetoothDirection=0;	 				break;
	  case 0x01:Move_Forward();g_iCarSpeedSet -=Change_Speed;				break;    						//ǰ������
		case 0x02:g_fBluetoothDirection +=Change_Speed; 							break;								//��ת
		case 0x03:g_iCarSpeedSet +=Change_Speed;											break;								//����
		case 0x04:g_fBluetoothDirection -=Change_Speed;								break;								//��ת
		case 0x05:Speed_Stop();																				break;								//ͣת
		//ת��
		case 0x06:Move_Forward();																			break;								//��ת
		case 0x07:Move_Reverse();																			break;								//��ת
		default  :Speed_Stop();
	}

}

void Move_try(uchar x)
{
  switch(x)
	{
	  case 0x01:Display(1,1);break;    						//ǰ��
		case 0x02:Display(1,2);break;								//��ת
		case 0x03:Display(1,3);break;								//����
		case 0x04:Display(1,4);break;								//��ת
		case 0x05:Display(1,5);break;								//ͣת
		case 0x06:Display(1,6);break;
		case 0x07:Display(1,7);break;
		default  :Display(1,0);break;
	}
}

