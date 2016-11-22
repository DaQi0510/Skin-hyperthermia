#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "pwm.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//按键输入 驱动代码		   
//技术论坛:www.openedv.com
//修改日期:2011/11/28 
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) ALIENTEK  2009-2019
//All rights reserved
//********************************************************************************
extern volatile u8 WorkFlag;
void KEY_Init(void) //IO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);

	//init GPIOC6  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure); 
}
void EXTI9_5_IRQHandler(void)
{
	Delayk();
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		if(WorkFlag<=1)
			WorkFlag++;
	}
	EXTI_ClearITPendingBit(EXTI_Line6);  //清除EXTI6线路挂起位
}
void Delayk(void)
{
	u8 i,j;
	for(i=0;i<200;i++)
		for(j=0;j<20;j++);
}

