#ifndef __COMMON_H
#define __COMMON_H	

#include "stm32f10x.h"
#include "sys.h"

//LED¶Ë¿Ú¶¨Òå
#define LED4 PBout(15) // PB15
#define LED3 PBout(14) // PB14
#define LED2 PBout(13) // PB13
#define LED1 PBout(12) // PB12

#define PLed PCout(7)  //PC7

#define Power PAout(2)  //PA2

#define SHT_SCL_H  PCout(8)=1
#define SHT_SCL_L  PCout(8)=0
#define SHT_SDA_H  PCout(9)=1
#define SHT_SDA_L  PCout(9)=0
#define SHT_SDA    PCin(9)


#endif

