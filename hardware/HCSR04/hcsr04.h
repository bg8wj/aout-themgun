#ifndef HCSR04_H_
#define HCSR04_H_
#include "main.h"
#include "sys.h"

extern TIM_HandleTypeDef TIM3_Handler;   //��ʱ����� 
extern TIM_HandleTypeDef TIM5_Handler;   //��ʱ��5��� 


void TIM3_Init(u16 arr,u16 psc);
void TIM5_CH1_Cap_Init(u32 arr,u16 psc);

#endif /* HCSR04_H_ */
