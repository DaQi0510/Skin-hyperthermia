#include "lcd.h"
const u8 F6x8[][6] =
{
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 }    // *
};
u8 Message1[64];
u8 Message2[64];
u8 Message3[64];
u8 Message4[64];
const u8 Lcotion1=11;
const u8 Lcotion2=17;
const u8 Lcotion3=17;
const u8 Lcotion4=4;

/*************************
-----------|------------
     PB14 -> SDA
		 PB13 -> SCL
		 PB12 -> RESET
*************************/
void LCD_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13|GPIO_Pin_14;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	LCD_RST=0;
  delay(500);
  LCD_RST=1; 
  delay(200); 
	
	transfer_command(0xae);	//关显示

	transfer_command(0xd5);	//晶振频率
	transfer_command(0xd1);

	transfer_command(0xa8);	//duty设置
	transfer_command(0x1f);	//duty=1/64  //复用率设置与页有关

	transfer_command(0xd3);	//显示偏移
	transfer_command(0x00);	

	transfer_command(0x40);	//起始行

	transfer_command(0x8d);	//升压允许
	transfer_command(0x14);	

	transfer_command(0x20);	//page address mode
	transfer_command(0x02);

	transfer_command(0xc8); //行扫描顺序：从上到下
	transfer_command(0xa1); //列扫描顺序：从左到右

	transfer_command(0xda); //sequential configuration
	transfer_command(0x12);




	transfer_command(0x81); //微调对比度,本指令的0x81不要改动，改下面的值
	transfer_command(0xcf); //微调对比度的值，可设置范围0x00～0xff


	transfer_command(0xd9); //Set Pre-Charge Period
	transfer_command(0xf1);

	transfer_command(0xdb); //Set VCOMH Deselect Level
	transfer_command(0x40);

	transfer_command(0xaf); //开显示
}
void ShowData(u8 x,u8 y,u8 data)
{
	u8 i;
	lcd_address(x,y);
	for(i=0;i<6;i++)
		transfer_data(F6x8[data][i]);
	
}
//全屏清屏
void clear_screen()
{
	unsigned char i,j;
 	for(j=0;j<4;j++)
	{
		lcd_address(1+j,1);
		for(i=0;i<64;i++)
		{
			transfer_data(0x00);
		}
	}	
}
//显示实时温度信息
void ShowMessage1(int d1)
{
	u8 i,j;
	u8 k;
	if(d1<0)
	{
		for(i=0;i<6;i++)
		{
			Message1[Lcotion1+i]=(F6x8[11][i]&0x0f)<<4;
			Message2[Lcotion1+i]=(Message2[11+i]&0xf0)+((F6x8[11][i]&0xf0)>>4);
		}
		d1=-d1;
	}
	else
	{
		for(i=0;i<6;i++)
		{
			Message1[Lcotion1+i]=0;
			Message2[Lcotion1+i]=(Message2[11+i]&0xf0);
		}
	}
  k=d1/100;
	if(k==0)
	{
		for(i=0;i<6;i++)
		{
			Message1[Lcotion1+6+i]=0;
			Message2[Lcotion1+6+i]=(Message2[11+i]&0xf0);
		}
	}
	else
	{
		for(i=0;i<6;i++)
		{
			Message1[Lcotion1+6+i]=(F6x8[k][i]&0x0f)<<4;
			Message2[Lcotion1+6+i]=(Message2[11+6+i]&0xf0)+((F6x8[k][i]&0xf0)>>4);
		}
	}
	k=(d1%100)/10;
	for(i=0;i<6;i++)
	{
		Message1[Lcotion1+12+i]=(F6x8[k][i]&0x0f)<<4;
		Message2[Lcotion1+12+i]=(Message2[11+12+i]&0xf0)+((F6x8[k][i]&0xf0)>>4);
	}
	for(i=0;i<6;i++)
	{
		Message1[Lcotion1+18+i]=(F6x8[10][i]&0x0f)<<4;
		Message2[Lcotion1+18+i]=(Message2[11+18+i]&0xf0)+((F6x8[10][i]&0xf0)>>4);
	}
	k=d1%10;
	for(i=0;i<6;i++)
	{
		Message1[Lcotion1+24+i]=(F6x8[k][i]&0x0f)<<4;
		Message2[Lcotion1+24+i]=(Message2[11+24+i]&0xf0)+((F6x8[k][i]&0xf0)>>4);
	}
	for(i=0;i<5;i++)
	{
		Message1[Lcotion1+31+i+1]=(F6x8[10][i]&0xf0);
	}
	for(i=0;i<5;i++)
	{
		Message1[Lcotion1+37+i]=(F6x8[12][i+1]&0x0f)<<4;
		Message2[Lcotion1+37+i]=(Message2[Lcotion1+37+i]&0xf0)+((F6x8[12][i+1]&0xf0)>>4);
	}
	lcd_address(1,Lcotion1);
	for(i=0;i<42;i++)
	{
		transfer_data(Message1[Lcotion1+i]);
	}
	lcd_address(2,Lcotion1);
	for(i=0;i<44;i++)
	{
		transfer_data(Message2[Lcotion1+i]);
	}
}
//显示设定温度
void ShowMessage2(int d2)
{
	u8 i,j;
	u8 k;
	k=d2/100;
	if(k==0)
	{
		for(i=0;i<6;i++)
		{
			Message2[Lcotion2+i]=(Message2[Lcotion2+i]&0x0f);
			Message3[Lcotion2+i]=(Message3[Lcotion2+i]&0xf0);
		}
	}
	else
	{
		for(i=0;i<6;i++)
		{
			Message2[Lcotion2+i]=(Message2[Lcotion2+i]&0x0f)+((F6x8[k][i]&0x0f)<<4);
			Message3[Lcotion2+i]=(Message3[Lcotion2+i]&0xf0)+((F6x8[k][i]&0xf0)>>4);
		}
	}
	k=(d2%100)/10;
	for(i=0;i<6;i++)
	{
		Message2[Lcotion2+6+i]=(Message2[Lcotion2+6+i]&0x0f)+((F6x8[k][i]&0x0f)<<4);
		Message3[Lcotion2+6+i]=(Message3[Lcotion2+6+i]&0xf0)+((F6x8[k][i]&0xf0)>>4);
	}
	for(i=0;i<6;i++)
	{
		Message2[Lcotion2+12+i]=(Message2[Lcotion2+12+i]&0x0f)+((F6x8[10][i]&0x0f)<<4);
		Message3[Lcotion2+12+i]=(Message3[Lcotion2+12+i]&0xf0)+((F6x8[10][i]&0xf0)>>4);
	}
	k=d2%10;
	for(i=0;i<6;i++)
	{
		Message2[Lcotion2+18+i]=(Message2[Lcotion2+18+i]&0x0f)+((F6x8[k][i]&0x0f)<<4);
		Message3[Lcotion2+18+i]=(Message3[Lcotion2+18+i]&0xf0)+((F6x8[k][i]&0xf0)>>4);
	}	
	for(i=0;i<5;i++)
	{
		Message2[Lcotion2+25+i+1]=(Message2[Lcotion2+25+i+1]&0x0f)+(F6x8[10][i]&0xf0);
	}
	for(i=0;i<5;i++)
	{
		Message2[Lcotion2+31+i]=(Message2[Lcotion2+31+i]&0x0f)+((F6x8[12][i+1]&0x0f)<<4);
		Message3[Lcotion2+31+i]=(Message3[Lcotion2+31+i]&0xf0)+((F6x8[12][i+1]&0xf0)>>4);
	}
	lcd_address(2,Lcotion2);
	for(i=0;i<36;i++)
	{
		transfer_data(Message2[Lcotion2+i]);
	}
	lcd_address(3,Lcotion2);
	for(i=0;i<38;i++)
	{
		transfer_data(Message3[Lcotion2+i]);
	}
}
//显示设定时间
void ShowMessage3(u16 d3)
{
	u8 i,j;
	u8 k;
	k=d3/1000;
	if(k==0)
	{
		for(i=0;i<6;i++)
		{
			Message3[Lcotion3+i]=(Message3[Lcotion3+i]&0x0f);
			Message4[Lcotion3+i]=0;
		}
	}
	else
	{
		for(i=0;i<6;i++)
		{
			Message3[Lcotion3+i]=(Message3[Lcotion3+i]&0x0f)+((F6x8[k][i]&0x0f)<<4);
			Message4[Lcotion3+i]=((F6x8[k][i]&0xf0)>>4);
		}
	}
	k=(d3%1000)/100;
	if(d3<100)
	{
		for(i=0;i<6;i++)
		{
			Message3[Lcotion3+6+i]=(Message3[Lcotion3+6+i]&0x0f);
			Message4[Lcotion3+6+i]=0;
		}
	}
	else
	{
		for(i=0;i<6;i++)
		{
			Message3[Lcotion3+6+i]=(Message3[Lcotion3+6+i]&0x0f)+((F6x8[k][i]&0x0f)<<4);
			Message4[Lcotion3+6+i]=((F6x8[k][i]&0xf0)>>4);
		}
	}
	k=(d3%100)/10;
	if(d3<10)
	{
		for(i=0;i<6;i++)
		{
			Message3[Lcotion3+12+i]=(Message3[Lcotion3+12+i]&0x0f);
			Message4[Lcotion3+12+i]=0;
		}
	}
	else
	{
		for(i=0;i<6;i++)
		{
			Message3[Lcotion3+12+i]=(Message3[Lcotion3+12+i]&0x0f)+((F6x8[k][i]&0x0f)<<4);
			Message4[Lcotion3+12+i]=((F6x8[k][i]&0xf0)>>4);
		}
	}
	k=d3%10;
	for(i=0;i<6;i++)
	{
		Message3[Lcotion3+18+i]=(Message3[Lcotion3+18+i]&0x0f)+((F6x8[k][i]&0x0f)<<4);
		Message4[Lcotion3+18+i]=((F6x8[k][i]&0xf0)>>4);
	}
	for(i=0;i<6;i++)
	{
		Message3[Lcotion3+28+i]=(Message3[Lcotion3+28+i]&0x0f)+((F6x8[13][i]&0x0f)<<4);
		Message4[Lcotion3+28+i]=((F6x8[13][i]&0xf0)>>4);
	}
	lcd_address(3,Lcotion3);
	for(i=0;i<34;i++)
	{
		transfer_data(Message3[Lcotion3+i]);
	}
	lcd_address(4,Lcotion3);
	for(i=0;i<34;i++)
	{
		transfer_data(Message4[Lcotion3+i]);
	}
}
//显示光标
void ShowMessage4(u8 d4)
{
	u8 i;
	if(d4==1)
	{
		lcd_address(2,Lcotion4);
		for(i=0;i<6;i++)
		{
			transfer_data((F6x8[14][i]&0x0f)<<4);
		}
		lcd_address(3,Lcotion4);
		for(i=0;i<6;i++)
		{
			transfer_data((F6x8[14][i]&0xf0)>>4);
		}
		lcd_address(4,Lcotion4);
		for(i=0;i<6;i++)
		{
			transfer_data(0);
		}
	}
	if(d4==2)
	{
		lcd_address(2,Lcotion4);
		for(i=0;i<6;i++)
		{
			transfer_data(0);
		}
		lcd_address(3,Lcotion4);
		for(i=0;i<6;i++)
		{
			transfer_data((F6x8[14][i]&0x0f)<<4);
		}
		lcd_address(4,Lcotion4);
		for(i=0;i<6;i++)
		{
			transfer_data((F6x8[14][i]&0xf0)>>4);
		}
	}
	
}
void lcd_address(u8 page,u8 column)
{
	column=column-1;  							//我们平常所说的第1列，在LCD驱动IC里是第0列。所以在这里减去1.
	page=page-1;
	transfer_command(0xb0+page);   			//设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1
	transfer_command(((column>>4)&0x0f)+0x12);	//设置列地址的高4位
	transfer_command(column&0x0f);				//设置列地址的低4位
}

void write_w(u8 data)
{
	unsigned char m,da;
  unsigned char j;
     da=data;
     for(j=0;j<8;j++)
{
     m=da;
     LCD_SCL=0;
     m=m&0x80;
     if(m==0x80)
{
     LCD_SDA=1;
}
     else
{
     LCD_SDA=0;
}
     da=da<<1;
 //    Delay10us();   //12T增加延时
     LCD_SCL=1;
}
     LCD_SCL=0;
     delay(3) ; //12T增加延时
     LCD_SCL=1;
}

void transfer_command(u8 ins)
{
	 Start();
	 write_w(0x78);
	 write_w(0x00);
	 write_w(ins);
	 Stop();
}

void transfer_data(u8 dat)
{
	 Start();
	 write_w(0x78);
	 write_w(0x40);
	 write_w(dat);
	 Stop();
}

void Start(void)
{
	
	LCD_SCL=1;
	delays();
  LCD_SDA=1;
	delays();
  LCD_SDA=0;
	delays();
  LCD_SCL=0;
	delays();
}

void Stop(void)
{
	LCD_SCL=0;
	delays();
  LCD_SDA=0;
	delays();
  LCD_SDA=1;
	delays();
  LCD_SCL=1;
	delays();
}

void delay(int i)
{
	int j,k;
	for(j=0;j<i;j++)
	{
		for(k=0;k<800;k++);
	}
}

void delays(void)
{
	u8 i;
	for(i=0;i<30;i++);
}
