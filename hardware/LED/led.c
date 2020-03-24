#include "led.h"

/*------------------------------LED IO≥ı ºªØ----------------*/
void LED_Init(void)
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);//PC0÷√1 
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);//PD3÷√1  
}
