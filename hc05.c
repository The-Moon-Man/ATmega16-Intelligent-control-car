#include <iom16v.h>
#include <contains.h>
#define fosc  8000000	//晶振8MHZ
#define baud 	9600 		//波特率

/*全局变量*/
uchar rdata;
uchar Flag_Blue=0;

/*端口初始化*/
void HC05_Init(void)
{         
  uart_init();
}

/*串口初始化，波特率设置*/
void uart_init(void)
{
  /*控制寄存器清零*/
  UCSRB=0x00;
	UCSRA=0x02;
	/*异步模式,禁止奇偶校验,停止位为1位,数据位为8位*/
	UCSRC=(1<<URSEL)|(0<<UPM0)|(3<<UCSZ0);
	/*9600bps @8.0000MHZ 不倍数*/
	UBRRL=(fosc/8/(baud+1))%256;
	UBRRH=(fosc/8/(baud+1))/256;
	/*接收、发送使能，接收中断使能*/
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	/*开全局中断*/
	SREG=BIT(7);
	/*配置TX为输出*/
	DDRD|=0x02;
}
/*数据发送*/
void uart_senddata(uchar data)
{
  /*第五位UDRE为高，数据寄存器空*/
  while(!(UCSRA&(BIT(UDRE))));
	/*发数据至UDR*/
	UDR=data;
	/*第六位TXC高， 发送结束*/
	while(!(UCSRA&(BIT(TXC))));
	/*置位TXC*/
	UCSRA|=BIT(TXC);
}

/*数据接收中断服务*/
#pragma interrupt_handler uart_recdata:12
void uart_recdata(void)
{
	UCSRB&=~BIT(RXCIE);
	rdata=UDR;
	Flag_Blue=1;
  UCSRB|=BIT(RXCIE);
}
