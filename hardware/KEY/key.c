#include "main.h"
#include "key.h"
#include "delay.h"

/*----------------KEY_APP------------------------*/
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��WKUP���� WK_UP
uint8_t KEY_Scan(uint8_t mode)
{
  static uint8_t key_up=1;     //�����ɿ���־
  if(mode==1)key_up=1;    //֧������
  if(key_up&&(KEY1==1||KEY2==1))
  {
    delay_ms(50);   
    key_up=0;
    if(KEY1==1)       return KEY1_PRES;
    else if(KEY2==1)  return KEY2_PRES;
    }else if(KEY1==0&&KEY2==0)key_up=1;
   return 0;   //�ް�������
}
