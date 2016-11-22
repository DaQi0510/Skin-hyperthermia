#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

void SpeakerShutDown(void);
void Heat_Init(u16 arr,u16 psc);
void Speaker_Init();
void DoSpeak(u8 Num);
void SetTemperature(u16 data);

#endif
