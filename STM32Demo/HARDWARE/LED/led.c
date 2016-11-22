#include "stm32f10x.h"
#include "led.h"

//LED IO初始化
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);						 //PB12、PB13、PB13、PB13
 
 //电源指示灯部分
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA端口时钟    
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC,GPIO_Pin_7);
	
 //电源使能
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟    
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIO_SetBits(GPIOA,GPIO_Pin_2);		
}
void SetLed(u8 num)
{
	if(num==1)
	{
		LED1=0;
		LED2=1;
		LED3=1;
		LED4=1;
	}
	if(num==2)
	{
		LED1=1;
		LED2=0;
		LED3=1;
		LED4=1;
	}
	if(num==3)
	{
		LED1=1;
		LED2=1;
		LED3=0;
		LED4=1;
	}
	if(num==4)
	{
		LED1=1;
		LED2=1;
		LED3=1;
		LED4=0;
	}
}
