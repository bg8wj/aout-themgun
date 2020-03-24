#ifndef __OLED_H
#define __OLED_H
#include "main.h"
#include "sys.h"
//使用4线串行接口时使用 		    						  
//-----------------OLED端口定义----------------  		
//#define SPI1_CS_Pin GPIO_PIN_8
//#define SPI1_CS_GPIO_Port GPIOA
//#define SPI1_REST_Pin GPIO_PIN_13
//#define SPI1_REST_GPIO_Port GPIOG
//#define SPI1_DC_Pin GPIO_PIN_14
//#define SPI1_DC_GPIO_Port GPIOG


#define OLED_CS     PAout(8)
#define OLED_RST    PGout(13)
#define OLED_DC     PGout(14)

#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据

#define SIZE        16
#define XLevelL		  0x02
#define XLevelH		  0x10
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xFF 
#define X_WIDTH 	  128
#define Y_WIDTH 	  64

#define SSD1309_MIRROR_VERT
//#define SSD1309_MIRROR_HORIZ
//#define SSD1309_INVERSE_COLOR 

//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);	
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);

void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture(void);
#endif
