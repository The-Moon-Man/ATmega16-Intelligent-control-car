#include <smg.h>
#include <iom16v.h>
#include <delay.h>
#include <contains.h>

uchar  discode[10]={ 0xC0, 0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90 };  //段码表
uchar  disbit[4]  ={ 0x0E, 0x0D,0x0B,0x07};														 			//选通值  

void Display_init(void)
{
 	 DDRA |=0x0F;       //设为输出口     
   DDRB =0xFF;       //设为输出口 
	 PORTA=0x0F;      
   PORTB=0xFF;
}
void Display(uchar i, uchar j)//在第i个数码管上，显示数字j
{  

	  PORTB=discode[j];//显示数字j
    PORTA=disbit[i];//第i个数码管
    Delay_ms(1);
    PORTA=0x0f;
}