#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
extern u8 Mode;
extern u16 Temperature_Set;
extern u16 Time_Set;
extern u8 State;


//外部中断初始化程序
/****************************
------------|-------------
        PA0 -> Shift
				PA1 -> +
				PA2 -> -
				PA3 -> OK
*****************************/
void EXTIX_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

  //上拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//PA0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//先占优先级4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
	
	//PA1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//先占优先级4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 

  //PA2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//先占优先级4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 	

  //PA3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//先占优先级4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 	 	 	
}

//Shift
void EXTI0_IRQHandler(void)
{
	delay_ms(20);    //消抖
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
		if(Mode==1)
			Mode=2;
		else
			Mode=1;
//		ShowMessage4(Mode);
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
}
//+
void EXTI1_IRQHandler(void)
{
	delay_ms(20);    //消抖
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
		if(Mode==1)
		{
			Temperature_Set+=5;
//			ShowMessage2(Temperature_Set);
		}
		if(Mode==2)
		{
			Time_Set+=15;
//			ShowMessage3(Time_Set);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除EXTI0线路挂起位
}
//-
void EXTI2_IRQHandler(void)
{
	delay_ms(20);    //消抖
	EXTI_ClearITPendingBit(EXTI_Line3);
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
		if(Mode==1)
		{
			if(Temperature_Set>=5)
				Temperature_Set-=5;
			else
				Temperature_Set=0;
		}
		if(Mode==2)
		{
			if(Time_Set>=15)
			 Time_Set-=15;
			else
			 Time_Set=0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除EXTI0线路挂起位
	EXTI_ClearITPendingBit(EXTI_Line3);
}
//OK
void EXTI3_IRQHandler(void)
{
	delay_ms(20);    //消抖
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	 
    if(State==3)
		{
			State=4;//治疗模式
			LED1=0;
			LED2 =0;
		}			
		if(State==1)
		{
			State=2;//加热模式
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除EXTI0线路挂起位
}

