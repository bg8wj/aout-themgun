#ifndef _LED_H
#define _LED_H
#include "SYS.h"
/******************************************************************************
STM32F407 expro ���������ⲿsdramͨ��ϵͳ�ļ�
******************************************************************************/
#define LED0 PCout(0)   //LED0
#define LED1 PDout(3)   //LED1

void LED_Init(void);
#endif
