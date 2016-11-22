#include "timer.h"
#include "led.h"
#include "stm32f10x_adc.h"
#include "sht21.h"
#include "pwm.h"
extern volatile u8 Mode;
extern u8 State;
extern u16 Time_Set;
extern volatile u8 WorkFlag;
extern volatile u8 TemperatureFlag;
extern volatile int Temperature_Set;    //温度设定值
extern volatile int Temperature;        //实际温度值
float P=20.0,I=0.0002,D=3.0;
int Inum;
char Temx1,Temx2;
u8 Num=0;
u8 PID;
void Timer2_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = (5000-1); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	 
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设					 
}
//10ms进中断一次，读取电压值
void Timer4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 1000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设		
}
void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		if(WorkFlag==1)
		{
			Num++;
			if(Num==2)
			{
				Num=0;
				USART_SendData(USART1,0);
				while((USART1->SR&0X40)==0);//等待发送结束
				USART_SendData(USART1,Temperature/256);
				while((USART1->SR&0X40)==0);//等待发送结束
				USART_SendData(USART1,Temperature%256);
				while((USART1->SR&0X40)==0);//等待发送结束
			}
			if(Mode==1)
			{
				LED1=!LED1;
			}
			if(Mode==2)
			{
				LED2=!LED2;
			}
			if(Mode==3)
			{
				LED3=!LED3;
			}
			if(Mode==4)
			{
				LED4=!LED4;
			}
		}

				
		if(WorkFlag==2)
		{
			Num++;
			if(Num==2)
			{
				Num=0;
				Time_Set--;
				if(Time_Set==0)
				{
					WorkFlag=3;
				}
			}
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}
void TIM4_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		if(WorkFlag>0)
		{
			PwmSet();
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}
//读取引脚电压部分初始化
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1,ENABLE);	  //使能ADC1通道时钟
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC最大时间不能超过14M
	//PA0/1/2/3 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
 

	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//重置指定的ADC1的校准寄存器
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1重置校准寄存器的状态,设置状态则等待
	
	ADC_StartCalibration(ADC1);		//开始指定ADC1的校准状态
 
	while(ADC_GetCalibrationStatus(ADC1));		//获取指定ADC1的校准程序,设置状态则等待
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}	

u16 Get_Adc(u8 ch)   
{
  //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道3,规则采样顺序值为1,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
void PwmSet()
{
	Temperature=SHT21_Temperature();
	if((Temperature_Set-Temperature>8)&&(TemperatureFlag==0))  //大于1度
	{
		 SetTemperature(199);    //最大功率加热
		 Inum=0;
	}
  else if((Temperature-Temperature_Set<3)&&(Temperature-Temperature_Set>-3))  //温度大于设定值3度，停止加热	
	{
		SetTemperature(20);    //最大功率加热
		Inum=0;
	}
	else if(Temperature-Temperature_Set>5)
	{
		SetTemperature(1);    //最小功率加热
		Inum=0;
	}
	else
	{
		Temx2=Temperature_Set-Temperature;
		Inum+=Temx2;
		PID=P*Temx2+I*(float)Inum+D*(float)(Temx2-Temx1);
    Temx1=Temx2;
		if(PID>199)
			PID=199;
		if(PID<20)
			PID=20;
		 SetTemperature(PID); 
	}
	if(WorkFlag==1)
	{
		if((Temperature_Set-Temperature<5)&&(Temperature_Set-Temperature>-5))
		{
			if(TemperatureFlag==0)
				TemperatureFlag=1;
		}
  }
//	if(Temperature_Set>Temperature)   //实际温度小与设定值
//	{
//		if(Temperature_Set-Temperature>50)  //大于5度
//		{
//		   SetTemperature(199);    //最大功率加热
//		}
//	}
   	
}












