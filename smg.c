#include <smg.h>
#include <iom16v.h>
#include <delay.h>
#include <contains.h>

uchar  discode[10]={ 0xC0, 0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90 };  //�����
uchar  disbit[4]  ={ 0x0E, 0x0D,0x0B,0x07};														 			//ѡֵͨ  

void Display_init(void)
{
 	 DDRA |=0x0F;       //��Ϊ�����     
   DDRB =0xFF;       //��Ϊ����� 
	 PORTA=0x0F;      
   PORTB=0xFF;
}
void Display(uchar i, uchar j)//�ڵ�i��������ϣ���ʾ����j
{  

	  PORTB=discode[j];//��ʾ����j
    PORTA=disbit[i];//��i�������
    Delay_ms(1);
    PORTA=0x0f;
}