#include <iom16v.h>
#include <contains.h>
#define fosc  8000000	//����8MHZ
#define baud 	9600 		//������

/*ȫ�ֱ���*/
uchar rdata;
uchar Flag_Blue=0;

/*�˿ڳ�ʼ��*/
void HC05_Init(void)
{         
  uart_init();
}

/*���ڳ�ʼ��������������*/
void uart_init(void)
{
  /*���ƼĴ�������*/
  UCSRB=0x00;
	UCSRA=0x02;
	/*�첽ģʽ,��ֹ��żУ��,ֹͣλΪ1λ,����λΪ8λ*/
	UCSRC=(1<<URSEL)|(0<<UPM0)|(3<<UCSZ0);
	/*9600bps @8.0000MHZ ������*/
	UBRRL=(fosc/8/(baud+1))%256;
	UBRRH=(fosc/8/(baud+1))/256;
	/*���ա�����ʹ�ܣ������ж�ʹ��*/
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	/*��ȫ���ж�*/
	SREG=BIT(7);
	/*����TXΪ���*/
	DDRD|=0x02;
}
/*���ݷ���*/
void uart_senddata(uchar data)
{
  /*����λUDREΪ�ߣ����ݼĴ�����*/
  while(!(UCSRA&(BIT(UDRE))));
	/*��������UDR*/
	UDR=data;
	/*����λTXC�ߣ� ���ͽ���*/
	while(!(UCSRA&(BIT(TXC))));
	/*��λTXC*/
	UCSRA|=BIT(TXC);
}

/*���ݽ����жϷ���*/
#pragma interrupt_handler uart_recdata:12
void uart_recdata(void)
{
	UCSRB&=~BIT(RXCIE);
	rdata=UDR;
	Flag_Blue=1;
  UCSRB|=BIT(RXCIE);
}
