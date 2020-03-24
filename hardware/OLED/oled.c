#include "main.h"
#include "sys.h"
#include "oled.h"
#include "stdlib.h"	
#include "stdio.h"
#include "delay.h"
#include "oledfont.h"
#include "bmp.h"

extern SPI_HandleTypeDef hspi1;

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[128][8];	 



void OLED_Reset(void) 
{
	OLED_CS=1;
  OLED_RST =0;
  delay_us(20);
	OLED_RST =1;
	delay_us(20);
}

void OLED_WR_Byte(u8 dat,u8 cmd)
{	
  OLED_CS=0; // select OLED
  if(cmd) OLED_DC=1; // data
  else    OLED_DC=0; // command
  HAL_SPI_Transmit(&hspi1,&dat,1, 1000);
  OLED_CS=1;// un-select OLED
} 


//******************************************************************
//��������  OLED_Init
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʼ��SSD1327	  
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 				    
void OLED_Init(void)
{ 		 
	 OLED_Reset(); 
   delay_ms(200);
	 OLED_WR_Byte(0xae,OLED_CMD); //display off

   OLED_WR_Byte(0xFD,OLED_CMD); //Set Command Lock
   OLED_WR_Byte(0x12,OLED_CMD); //unlock the OLED driver IC
  
  
   OLED_WR_Byte(0xD5,OLED_CMD); /*--set display clock divide ratio/oscillator frequency */
   OLED_WR_Byte(0xF0,OLED_CMD); /*--set divide ratio */
    
   OLED_WR_Byte(0xA8,OLED_CMD); /*--set multiplex ratio(1 to 64) - CHECK  */
   OLED_WR_Byte(0x3F,OLED_CMD); /*                                        */

   OLED_WR_Byte(0xD3,OLED_CMD); /*-set display offset - CHECK */
   OLED_WR_Byte(0x00,OLED_CMD); /*-not offset */  

   OLED_WR_Byte(0x40,OLED_CMD); /*--set start line address - CHECK */
   
   OLED_WR_Byte(0x8D,OLED_CMD); /*--set DC-DC enable */
   OLED_WR_Byte(0x14,OLED_CMD); /**/ 

   OLED_WR_Byte(0x20,OLED_CMD); /* Set Memory Addressing Mode */   
   OLED_WR_Byte(0x10,OLED_CMD); /* 00,Horizontal Addressing Mode; 01,Vertical Addressing Mode; */
                                /* 10,Page Addressing Mode (RESET); 11,Invalid */
#ifdef SSD1309_MIRROR_VERT
   OLED_WR_Byte(0xC0,OLED_CMD); /* Mirror vertically */
#else
   OLED_WR_Byte(0xC8,OLED_CMD); /* Set COM Output Scan Direction */
#endif                             
#ifdef SSD1309_MIRROR_HORIZ
   OLED_WR_Byte(0xA0,OLED_CMD); /* Mirror horizontally */
#else
   OLED_WR_Byte(0xA1,OLED_CMD); /* --set segment re-map 0 to 127 - CHECK */
#endif
   OLED_WR_Byte(0xDA,OLED_CMD); /*--set com pins hardware configuration - CHECK */
   OLED_WR_Byte(0x12,OLED_CMD);
   
   OLED_WR_Byte(0x81,OLED_CMD); /*--set contrast control register - CHECK */
   OLED_WR_Byte(0xFF,OLED_CMD);

   OLED_WR_Byte(0xD9,OLED_CMD); /*--set pre-charge period */
   OLED_WR_Byte(0x22,OLED_CMD); /*			  */
    
   OLED_WR_Byte(0xDB,OLED_CMD); /*--set vcomh */
   OLED_WR_Byte(0x20,OLED_CMD); /* 0x20, 0.77xVcc */ 

   OLED_WR_Byte(0xB0,OLED_CMD); /*Set Page Start Address for Page Addressing Mode, 0-7 */
   
   OLED_WR_Byte(0x00,OLED_CMD); /*---set low column address  */
   OLED_WR_Byte(0x10,OLED_CMD); /*---set high column address */

#ifdef SSD1309_INVERSE_COLOR
   OLED_WR_Byte(0xA7,OLED_CMD); /*--set inverse color */
#else
   OLED_WR_Byte(0xA6,OLED_CMD); /*--set normal color */
#endif
   OLED_WR_Byte(0xA4,OLED_CMD); /* 0xA4,Output follows RAM content;0xa5,Output ignores RAM content */
   OLED_WR_Byte(0xAF,OLED_CMD); /*--turn on SSD1309 panel */
   OLED_Clear();
} 

void OLED_Set_Pos(u8 x,u8 y)
{ 
  OLED_WR_Byte(0xb0+y,OLED_CMD);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
  OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
  OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
  OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void  OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
  OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
  OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	

//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}

//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}	
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

/********************************************
// fill_Picture
********************************************/
void fill_picture(u8 fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}

/*********************************************************************
 * @fn		OLED_Clear_line
 *
 * @brief 	clear the LED display, all the screen will be black.
 *
 * @param 	lineNumber <= 64/size,size = 12,16,24
 *
 * @return	void
 */	

void OLED_Clear_Line(u8 lineNumber,u8 size)  
{  
	u8 n,line;  
  
	if((size != 12) && (size != 16) && (size != 24))
		return;
	if(lineNumber < 1 || lineNumber >( 64/size))
			return ;
	
	line = 64/size+1 - lineNumber;

	if(size == 12)
	{
//		line = 6 - line;
	  if(line % 2)
		{
			for(n=0;n<128;n++)
			{
				OLED_GRAM[n][(line/2)*3 +1] = 0X00;

				OLED_GRAM[n][(line/2)*3] &= 0X0F;
			}
		}
		else
		{
			for(n=0;n<128;n++)
			{
				OLED_GRAM[n][(line/2)*3] &= 0Xf0;

				OLED_GRAM[n][(line/2)*3 - 1] = 0X00;
			}
		}
	}
	
	if(size == 16)
	{
//		line = 5 - line;
		for(n=0;n<128;n++)
		{
			OLED_GRAM[n][2*line-1] = 0X00;

			OLED_GRAM[n][2*line-2] = 0X00;
		}
	}
	
	if(size == 24)
	{
//		line = 3 - line;
		for(n=0;n<128;n++)
		{
			OLED_GRAM[n][3*line+1] = 0X00;

			OLED_GRAM[n][3*line] = 0X00;
			
			OLED_GRAM[n][3*line-1] = 0X00;
		}
	}
	OLED_Refresh_Gram();
}

/*********************************************************************
 * @fn		OLED_Clear_Half
 *
 * @brief 	fill the screen with black but not update display , need external update
 *
 * @param 	void
 *
 * @return	void
 */	  
void OLED_Clear_Half(void)  
{  
	u8 i,n;  
	for(i=0;i<2;i++)
		for(n=0;n<128;n++)
		    OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();
}

