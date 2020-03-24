#ifndef __MLX90614_H
#define __MLX90614_H
#include "sys.h"
#define ACK	 0 //应答
#define NACK 1 //无应答
#define SA				0x5a //Slave address 单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM存取命令
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM存取命令
#define RAM_TOBJ1		0x07 //To1 address in the eeprom 目标1温度,检测到的红外温度 -70.01 ~ 382.19度

#define SMBUS_PORT	GPIOB      //PB端口(端口和下面的两个针脚可自定义)
#define SMBUS_SCK		GPIO_PIN_6 //PB6：SCL
#define SMBUS_SDA		GPIO_PIN_7 //PB7：SDA



//SCL 引脚宏定义
#define SMBUS_SCK_H()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SCK,GPIO_PIN_SET)//置高电平
#define SMBUS_SCK_L()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SCK,GPIO_PIN_RESET)//置低电平
//SDA 引脚宏定义
#define SMBUS_SDA_H()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SDA,GPIO_PIN_SET) 
#define SMBUS_SDA_L()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SDA,GPIO_PIN_RESET)

#define SMBUS_SDA_PIN() HAL_GPIO_ReadPin(SMBUS_PORT, SMBUS_SDA)//读取引脚电平

void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Init(void);

u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
float SMBus_ReadTemp(void); //获取温度值

#endif


