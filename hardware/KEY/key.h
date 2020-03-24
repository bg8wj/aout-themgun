#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

//KEY 1 2
/*下面方式是通过位带操作方式读取IO*/
#define KEY1 		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)	//PA0
#define KEY2 		HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_11) //PF11

#define KEY1_PRES	1
#define KEY2_PRES	2
#endif

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);

