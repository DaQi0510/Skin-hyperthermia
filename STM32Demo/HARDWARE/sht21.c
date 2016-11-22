#include "delay.h"
#include "sht21.h"
#include "lcd.h"
#include "common.h"
double T,H;
u16 data_T,data_H;
u8 SHT_bit_out=0,SHT_bit_in=0;
u8 k1,k2;
void SHT_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	 // πƒ‹PC∂Àø⁄ ±÷”
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}
void SHT_SDA_out(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//Õ®”√Õ∆ÕÏ ‰≥ˆƒ£ Ω
 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// ‰≥ˆ◊Ó¥Û∆µ¬ Œ™10MHz
   GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
void SHT_SDA_in(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//…œ¿≠ ‰»Îƒ£ Ω
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
}	
void SHT_SCL_out(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//Õ®”√Õ∆ÕÏ ‰≥ˆƒ£ Ω
 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// ‰≥ˆ◊Ó¥Û∆µ¬ Œ10MHz
   GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
void SHT_SCL_in(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//…œ¿≠ ‰»Îƒ£ Ω
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
}	
void SHT_init(void)
{
	u8 i;
  SHT_GPIOInit();
	SHT_SDA_H; 
	SHT_SCL_L;
	for(i=0;i<9;i++)
	{
		SHT_SCL_H;
		SHT_SCL_L;
	}
}
void SHT_start_bit(void) 
{ 
   SHT_SDA_out();
   SHT_SDA_H; 
   SHT_SCL_H; 
   SHT_SDA_L; 
   SHT_delay();
	 SHT_delay();
   SHT_SCL_L; 
	 SHT_delay();
	 SHT_delay();

}
void SHT_stop_bit(void) 
{ 
	SHT_SDA_L; 
	SHT_SCL_L; 
	SHT_SCL_H; 
	SHT_delay();
	SHT_delay();
	SHT_SDA_H;
  SHT_delay();
	SHT_delay();
} 


u8 SHT_tx_byte(u8 dat_byte) 
{ 
	u8 i,dat; 
  u8	data_in=0;
	dat=dat_byte; 
	for(i=0;i<8;i++) 
	{ 
		if(dat&0x80) 
			SHT_SDA_H; 
		else 
		 SHT_SDA_L; 
		SHT_delay();
		SHT_SCL_H; 
		SHT_delay();
		SHT_SCL_L; 
		dat=dat<<1; 
	}
	SHT_SDA_in();
	SHT_SCL_H;
	 SHT_delay();
  data_in=SHT_SDA;
	SHT_SCL_L;
	SHT_SDA_out();
	if(data_in==1)
	{
		return 1;
	}
	if(data_in==0)
	{	
		return 0;	
	}		
}
u8  SHT_rx_byte(u8 ack) 
{
	u8 i,dat; 
	dat=0;  
	SHT_SDA_in();
	for(i=0;i<8;i++) 
	{ 
		dat=dat<<1;
		SHT_SCL_H;
		SHT_delay();
		if(SHT_SDA==1)
		dat=dat+1; 
		SHT_SCL_L;
		SHT_delay();
		
	}
	SHT_SDA_out();
	if(ack==1)
		SHT_SDA_H; 
	else
		SHT_SDA_L;
	SHT_SCL_H; 
	SHT_delay();
	SHT_SCL_L;
	SHT_delay();
	return dat; 
}
void SHT_Send(u8 home,u8 mem)
{
	SHT_start_bit();
	SHT_tx_byte(home);
	SHT_tx_byte(mem);
	SHT_stop_bit();	
}
u8 SHT21_Humidity(void)
{
	u8 k=1;
	SHT_start_bit();
  SHT_tx_byte(0x80);
	SHT_tx_byte(0xf5);
	while(k==1)
	{
	SHT_start_bit();
	k=SHT_tx_byte(0x81);
	SHT_delay();
	SHT_delay();
	SHT_delay();
	}
	k1=SHT_rx_byte(0);
	k2=SHT_rx_byte(1);
	SHT_stop_bit() ;
	data_T=k1;
	data_T=data_T<<8;
	data_T=data_T+k2;
	data_T&=0xfffc;
	T=-6+125*data_T/65536;
	return (u8)T;
}
short SHT21_Temperature(void)
{
	u8 k=1;
	SHT_start_bit();
  SHT_tx_byte(0x80);
	SHT_tx_byte(0xf3);
	while(k==1)
	{
	SHT_start_bit();
	k=SHT_tx_byte(0x81);
	SHT_delay();
	SHT_delay();
	SHT_delay();
	}
	k1=SHT_rx_byte(0);
	k2=SHT_rx_byte(1);
	SHT_stop_bit() ;
	data_T=k1;
	data_T=data_T<<8;
	data_T=data_T+k2;
	data_T&=0xfffc;
	T=(-46.85+175.72*data_T/65536)*10;
	return (short)T;
}
void reset_sht(void)
{
	SHT_start_bit();
	SHT_tx_byte(0x80);
	SHT_tx_byte(0xfe);
	SHT_stop_bit() ;
	SHT_delay();
	SHT_delay();
	SHT_delay();
	SHT_delay();
}
//—” ±
void SHT_delay(void)
{
	u8 i;
	for(i=0;i<200;i++);
}