#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "wdg.h"
#include "timer.h"
#include "pwm.h"
#include "lcd.h"
#include "sht21.h"
#include "key.h"
u16 Threshold=3470;
volatile u8 Mode;
u8 Flag;
volatile u8 WorkFlag;
volatile int Temperature_Set;    //温度设定值
volatile int Temperature;        //实际温度值
volatile u16 Time_Set;
volatile u8 TemperatureFlag;
void ModeScan(void);
void uart_init(u32 bound);
void AckState(void);
int main(void)
{

	u8 i;
	SystemInit();
	delay_init(72);	     //延时初始化
	Flag=0;
	Mode=1;
	WorkFlag=0;
	TemperatureFlag=0;
	Adc_Init();
	
	while(Flag==0)
	{
		ModeScan();
	}
  SpeakerShutDown();
	KEY_Init();
	SHT_GPIOInit();
	reset_sht();
	SHT_init();
  Heat_Init(200,7199);
	uart_init(115200);
	Timer2_Init(0,0);
	Timer4_Init(0,0);
  SetTemperature(10);
	while(1)
	{
		ModeScan();
		AckState();
		delay_ms(10);
		
	
//		 Parameters=Get_Adc(ADC_Channel_0) ;
//	
//		delay_ms(1000);

//		USART_SendData(USART1,0);
//		while((USART1->SR&0X40)==0);//等待发送结束
//		USART_SendData(USART1,Temperature/256);
//		while((USART1->SR&0X40)==0);//等待发送结束
//		USART_SendData(USART1,Temperature%256);
//		while((USART1->SR&0X40)==0);//等待发送结束
	}	 

}
void uart_init(u32 bound){
    //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC 配置

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  
   //USART 初始化设置
   
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
   

//    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
   
    USART_Cmd(USART1, ENABLE);                    //使能串口 
}
void ModeScan(void)
{
	u16 AD_Voltage;
	u16 FlagTime;
	AD_Voltage=Get_Adc(ADC_Channel_0);
	if(AD_Voltage>Threshold)
	{
		delay_ms(10); //消抖
		AD_Voltage=Get_Adc(ADC_Channel_0);
		if(AD_Voltage>Threshold)
		{
			if(Flag==0)     //刚开机
			{
				LED_Init();
				Power=1;
				delay_ms(1500);
				PLed=0;	
				SetLed(Mode);
				AD_Voltage=Get_Adc(ADC_Channel_0);
				while(AD_Voltage>Threshold)
				{
					AD_Voltage=Get_Adc(ADC_Channel_0);
					delay_ms(10); 
				}	
			}
			else    //  
			{
					AD_Voltage=Get_Adc(ADC_Channel_0);
					while(AD_Voltage>Threshold)                 //等待释放
					{
						AD_Voltage=Get_Adc(ADC_Channel_0);
						delay_ms(10); 
						FlagTime++;
						if(FlagTime>=250)
							break;
					}
					if(FlagTime<250)
					{
						if(Mode==4)
							Mode=1;
						else
							Mode++;
						SetLed(Mode);
						if(WorkFlag==0)
						{
							if(Mode==1)
							{
								Temperature_Set=480;
								Time_Set=150;
							}
							if(Mode==2)
							{
								Temperature_Set=480;
								Time_Set=120;
							}
							if(Mode==3)
							{
								Temperature_Set=490;
								Time_Set=150;
							}
							if(Mode==4)
							{
								Temperature_Set=490;
								Time_Set=120;
							}
						}
					}
					else
					{
						Power=0;
				    LED1=1;
						LED2=1;
						LED3=1;
						LED4=1;
						PLed=1;	
						AD_Voltage=Get_Adc(ADC_Channel_0);
						while(AD_Voltage>Threshold)                 //等待释放
						{
							AD_Voltage=Get_Adc(ADC_Channel_0);
						  delay_ms(10); 
						}	
					}
			}
			Flag =1;
		}
	}
}
void AckState(void)
{
	if(WorkFlag==2)
	{
		SetLed(Mode);
	}
	if(WorkFlag==3)
	{
		WorkFlag=0;
		TemperatureFlag=0;
		DoSpeak(2);
		if(Mode==1)
		{
			Temperature_Set=480;
			Time_Set=150;
		}
		if(Mode==2)
		{
			Temperature_Set=480;
			Time_Set=120;
		}
		if(Mode==3)
		{
			Temperature_Set=490;
			Time_Set=150;
		}
		if(Mode==4)
		{
			Temperature_Set=490;
			Time_Set=120;
		}
		SetLed(Mode);
	}
	if(WorkFlag==1)
	{
		if(TemperatureFlag==1)
		{
			DoSpeak(1);
			TemperatureFlag=2;
		}
	}
}
void LEDAck(u8 n)
{

}
