#include <delay.h>
#include <iom16v.h>

void Delay_ms(unsigned int ms)				//1ms
{
  unsigned int i,j;
  for(i=0;i<ms;i++)
  {
    for(j=0;j<1141;j++);
  }
}
void Delay_10us(unsigned int us)			//10ms×óÓÒ
{
	while(us--);
}