#ifndef __MLX90614_H
#define __MLX90614_H
#include "sys.h"
#define ACK	 0 //Ӧ��
#define NACK 1 //��Ӧ��
#define SA				0x5a //Slave address ����MLX90614ʱ��ַΪ0x00,���ʱ��ַĬ��Ϊ0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM��ȡ����
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM��ȡ����
#define RAM_TOBJ1		0x07 //To1 address in the eeprom Ŀ��1�¶�,��⵽�ĺ����¶� -70.01 ~ 382.19��

#define SMBUS_PORT	GPIOB      //PB�˿�(�˿ں������������ſ��Զ���)
#define SMBUS_SCK		GPIO_PIN_6 //PB6��SCL
#define SMBUS_SDA		GPIO_PIN_7 //PB7��SDA



//SCL ���ź궨��
#define SMBUS_SCK_H()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SCK,GPIO_PIN_SET)//�øߵ�ƽ
#define SMBUS_SCK_L()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SCK,GPIO_PIN_RESET)//�õ͵�ƽ
//SDA ���ź궨��
#define SMBUS_SDA_H()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SDA,GPIO_PIN_SET) 
#define SMBUS_SDA_L()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SDA,GPIO_PIN_RESET)

#define SMBUS_SDA_PIN() HAL_GPIO_ReadPin(SMBUS_PORT, SMBUS_SDA)//��ȡ���ŵ�ƽ

void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Init(void);

u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
float SMBus_ReadTemp(void); //��ȡ�¶�ֵ

#endif


