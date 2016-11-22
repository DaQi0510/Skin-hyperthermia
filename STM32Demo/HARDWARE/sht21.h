#ifndef __SHT21_H
#define __SHT21_H
#include "stm32f10x.h"
#include "sys.h"

void SHT_GPIOInit(void);
void SHT_init(void);
void SHT_SDA_out(void);
void SHT_SDA_in(void);
void SHT_SCL_out(void);
void SHT_SCL_in(void);
void SHT_start_bit(void);
void SHT_stop_bit(void);
void SHT_Respons(u8 a);
void SHT_send_bit(u8);
void SHT_receive_bit(void);
u8  SHT_tx_byte(u8 dat_byte);
void SHT_Send(u8 home,u8 mem);
u16 SHT_Read(u8 home);
void reset_sht(void);
u8 SHT21_Humidity(void);
short SHT21_Temperature(void);
void SHT_delay(void);



#endif
