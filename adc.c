#include<iom16v.h>
#include <adc.h>
#include <contains.h>

void ADC_Init(void)
{
  ADMUX =  0x40;   //adc6  01 0 00110
  ADCSRA = 0x86;      //  1 0 0  0 0  110
  SFIOR = 0x00;
	
}

uint ADC_Read(uint ch)
{
  uint data; 
  ADMUX |= ch;
  ADCSRA |= 0x40;    // 0 1 00 0000
  while((ADCSRA & 0x10)==0);
  ADCSRA |= 0x10;  
  ADMUX=0x40; 
  data=ADCL;
  data=data+ADCH*256;
  return data;
}
void trace(uint adal,uint adar,uint adam)
{
   int af1,af2,af3;
   af1=adal-adam;
   af2=adar-adam;
   af3=adal-adar;
	 
   Move_Forward();
   if((af1<-50&&af1>-150)&&(af2<50&&af2>-50))
   {
     Speed_Init();
     OCR0=255;
     OCR2=255;
	 	 //uart_senddata(0x00);
   }
   else if(af3<-95)
   {
     Speed_Init();
	 	 OCR2=120;
     OCR0=250;
	 	 //uart_senddata(0x01);
   }
   else if(af3>-50)
   {
     Speed_Init();
	 	 OCR2=250;
     OCR0=120;
	 	 //uart_senddata(0x02);
   }
   else
   {
     OCR0=180;
     OCR2=180;
	 	 Move_Forward();
     //uart_senddata(0x04);
   }
   
}