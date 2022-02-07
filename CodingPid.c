#include <iom16v.h>
#include <CodingPid.h>
#include <contains.h>

uchar left_time=0,right_time=0,Coding_time=0;
uchar Speed_left[3]={0,0,0},Speed_right[3]={0,0,0},Speed_left_fin=0,Speed_right_fin=0;

#define CTime (25)
#define pi    (3.1415926)
#define fosc  8000000//晶振8MHZ
#define Clun  65

//INT0,INT1:PD2,PD3
//转一圈产生11*48=528个脉冲
//每次的测量时间为1/8us*65535*(CTime-1)
//left_time和right_time代表在这个时间内的脉冲数量
//轮子直径65mm，周长为65*pi
//则实际速度为left_time/528*轮子周长/测量时间

/******电机控制参数******/
float g_fSpeedControlOut;
float g_fBluetoothDirection;
float g_fLeftMotorOut;
float g_fRightMotorOut;
/******速度控制参数******/
float g_fCarSpeed;
float g_iCarSpeedSet;
float g_fCarSpeedOld;

extern g_u8SpeedControlPeriod;
/******电机控制相关宏定义******/
#define MOTOR_OUT_DEAD_VAL       0	   //死区值
#define MOTOR_OUT_MAX           255	   //占空比正最大值
#define MOTOR_OUT_MIN            0     //占空比负最大值

//初始PID参数设置
PID_t g_tCarSpeedPID={0.2,0.05, 0};	//0.2,0.05
//数据初始化

void CarUpstandInit(void)
{
	g_fCarSpeed 					= g_fCarSpeedOld			= 0;
	g_fLeftMotorOut    		= g_fRightMotorOut		= 0;
	g_fBluetoothDirection = 0;
	g_iCarSpeedSet=150;														//PID默认初始目标
}
//PID设置
void PID(void)
{
  float fP,fI;   	
	float fDelta;
	static float fDelta_pre_pre=0,fDelta_pre=0;
	
	//低通滤波，使速度更平滑
  g_fCarSpeed = (Speed_left_fin  + Speed_right_fin ) * 0.5 ;
	g_fCarSpeed = 0.7 * g_fCarSpeedOld + 0.3 * g_fCarSpeed ;
	g_fCarSpeedOld = g_fCarSpeed;
	
	//限制设置速度
	if((int)g_iCarSpeedSet  > MOTOR_OUT_MAX)	g_iCarSpeedSet  = MOTOR_OUT_MAX;
	if((int)g_iCarSpeedSet  < MOTOR_OUT_MIN)	g_iCarSpeedSet  = MOTOR_OUT_MIN;
	
	fDelta = g_iCarSpeedSet;
	fDelta -= g_fCarSpeed;  
	//增量PI
	fP = (fDelta-fDelta_pre) *(g_tCarSpeedPID.P);
  fI = fDelta * (g_tCarSpeedPID.I);
	
	fDelta_pre_pre=fDelta_pre;
	fDelta_pre=fDelta;
	
	g_fSpeedControlOut=g_iCarSpeedSet+fP+fI;
	
	g_fLeftMotorOut  =g_fSpeedControlOut - g_fBluetoothDirection ;	
	g_fRightMotorOut =g_fSpeedControlOut + g_fBluetoothDirection ;
	//速度阈值
	if((int)g_fLeftMotorOut  > MOTOR_OUT_MAX)	g_fLeftMotorOut  = MOTOR_OUT_MAX;
	if((int)g_fLeftMotorOut  < MOTOR_OUT_MIN)	g_fLeftMotorOut  = MOTOR_OUT_MIN;
	if((int)g_fRightMotorOut > MOTOR_OUT_MAX)	g_fRightMotorOut = MOTOR_OUT_MAX;
	if((int)g_fRightMotorOut < MOTOR_OUT_MIN)	g_fRightMotorOut = MOTOR_OUT_MIN;
	
	OCR2=(int)g_fRightMotorOut;	
	OCR0=(int)g_fLeftMotorOut;
}
void Coding_Init(void)
{
  MCUCR = (1<<ISC11)|(1<<ISC10)|(1<<ISC01)|(1<<ISC00);	//外部中断触发方式
  GICR  = (1<<INT1)|(1<<INT0);													//外部中断使能
	TCCR1A=0X00;
	TCCR1B=(0<<CS12)|(0<<CS11)|(1<<CS10);									//1分频8Mhz
	
	ReSet_Bit(DDRD,PD2);
	ReSet_Bit(DDRD,PD3);	
}

int count=0;
void Speed_read(void)
{
  TCNT1 = 0;		 																				//初值清零
	ICR1=0;
	
	count++;
	Set_Bit(TIMSK, TOIE1);																//开定时器溢出中断
	while(Coding_time);
	ReSet_Bit(TIMSK, TOIE1);															//关定时器溢出中断
	
	//更新数据
  Speed_left[2]=Speed_left[1];
	Speed_left[1]=Speed_left[0];
	Speed_left[0]=left_time;
	left_time=0;
	
	Speed_right[2]=Speed_right[1];
	Speed_right[1]=Speed_right[0];
	Speed_right[0]=right_time;
	right_time=0;
	if(count>=3)count=0;
	//右侧速度
  if((abs(Speed_right[0]-Speed_right[1])>30))
	{
    if(Speed_right[0]>Speed_right[1])				 Speed_right_fin=Speed_right[0];
		else 														 				 Speed_right_fin=Speed_right[1];
	}
	else
	{
	  Speed_right_fin=Speed_right[2];
  }
	//左侧速度
	if((abs(Speed_left[0]-Speed_left[1])>30))
	{
	  if(Speed_left[0]>Speed_left[1])				 	 Speed_left_fin=Speed_left[0];
		else  															 	 	 Speed_left_fin=Speed_left[1];
  }
	else
	{
		Speed_left_fin=Speed_left[2];
	}
	
	//发送数据
	uart_senddata('s');
	uart_senddata( (uchar)( ((Speed_left_fin+Speed_right_fin)*0.5*fosc*Clun*pi)/(528.0*65535*(CTime-1))));
}

#pragma interrupt_handler int0_isr:iv_INT0
void int0_isr(void)
{
  left_time++;
	GIFR=(1<<INTF0);
}

#pragma interrupt_handler int1_isr:iv_INT1
void int1_isr(void)
{
  right_time++;
	GIFR=(1<<INTF1);
}
#pragma interrupt_handler timer1_interrupt_handler:9
void timer1_interrupt_handler(void)
{
  Coding_time++;
	if(Coding_time>CTime)
	{
    Coding_time=0;
	}
}