#include "main.h"
#include "sys.h"
#include "hcsr04.h"
#include "led.h"
/*
HC-SR04 STM32
VCC -> VCC
Trig -> PC3
Echo -> PA0
GND -> GND
*/

extern TIM_HandleTypeDef TIM5_Handler;         //��ʱ��5���
extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
   
}

//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{  
    TIM_IC_InitTypeDef TIM5_CH1Config;  
    GPIO_InitTypeDef GPIO_Initure;   
    __HAL_RCC_GPIOC_CLK_ENABLE();           // ����GPIOCʱ��
    __HAL_RCC_TIM5_CLK_ENABLE();
   
    GPIO_Initure.Pin=GPIO_PIN_3;            //PC3
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);//�ر�PC3 
    
    TIM5_Handler.Instance=TIM5;                          //ͨ�ö�ʱ��5
    TIM5_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM5_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_IC_Init(&TIM5_Handler);//��ʼ�����벶��ʱ������
  
    HAL_NVIC_SetPriority(TIM5_IRQn,2,0);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�0
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM5�ж�ͨ��     
   
    TIM5_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM5_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI1��
    TIM5_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM5_CH1Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CH1Config,TIM_CHANNEL_1);//����TIM5ͨ��1
	
    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_1);   //����TIM5�Ĳ���ͨ��1�����ҿ��������ж�
    __HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);   //ʹ�ܸ����ж�
}


//��ʱ��5�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_IC_Init()����
//htim:��ʱ��5���
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef GPIO_Initure;
      
  __HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM5ʱ��
  __HAL_RCC_GPIOA_CLK_ENABLE();			  //����GPIOAʱ��
	 
  GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
  GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
  GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
  GPIO_Initure.Alternate=GPIO_AF2_TIM5;   //PA0����ΪTIM5ͨ��1
  HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}


//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32 TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}
 
//��ʱ��5�жϷ�����
void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM5_Handler);//��ʱ�����ô�����
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM3)
  {
	__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
	HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
	HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
  }
}



//��ʱ�������жϣ�����������жϴ���ص������� �ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�����жϣ����������ʱִ��
{
 if(htim==(&TIM3_Handler))
 {
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);//��PC3
   HAL_Delay(20);
   LED0=~LED0;
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);//�ر�PC3 
  
 }
  if(htim==(&TIM5_Handler))
  {
    HAL_TIM_Base_Stop_IT(&TIM3_Handler);//�ȹر�time3
   if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
	 if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
	 {
     if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
	  {
		 TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
		 TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
		}
     else TIM5CH1_CAPTURE_STA++;
	  }	 
	 }
    LED1=~LED1;  	
   }
  
}


//��ʱ�����벶���жϴ���ص��������ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//�����жϷ���ʱִ��
{
 
  if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
  {
	if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
	{	  			
	  TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
     TIM5CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
     TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
     TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//����TIM5ͨ��1�����ز���
	 }else  								//��δ��ʼ,��һ�β���������
	 {
	  TIM5CH1_CAPTURE_STA=0;			//���
	  TIM5CH1_CAPTURE_VAL=0;
	  TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
	  __HAL_TIM_DISABLE(&TIM5_Handler);        //�رն�ʱ��5
	  __HAL_TIM_SET_COUNTER(&TIM5_Handler,0);
	  TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
	  TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//��ʱ��5ͨ��1����Ϊ�½��ز���
	  __HAL_TIM_ENABLE(&TIM5_Handler);//ʹ�ܶ�ʱ��5
	 }	
  
   }		
}
