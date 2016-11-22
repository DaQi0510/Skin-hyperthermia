#ifndef __LCD_H
#define __LCD_H	

#include "stm32f10x.h"
#include "common.h"



void LCD_Init(void);
void ShowData(u8 x,u8 y,u8 data);
void clear_screen(void);
void ShowMessage1(int d1);
void ShowMessage2(int d2);
void ShowMessage3(u16 d3);
void ShowMessage4(u8 d4);
void write_w(u8 data);
void transfer_command(u8 ins);
void transfer_data(u8 dat);
void lcd_address(u8 page,u8 column);
void Start(void);
void Stop(void);
void delay(int i);
void delays(void);


#endif
