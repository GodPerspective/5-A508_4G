#include "AllHead.h"

#define KEYTIME_SHORT  20    //�̰��İ���ȥ������ʱ��ʱ��
#define KEYTIME_LONG   800     //�����İ���ȥ������ʱ��ʱ��

typedef struct{
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    KeyStatesType key_states;
  }key_ptt;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_2;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_3;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_4;
}KEYSCAN_DRV;

static KEYSCAN_DRV KeyscanDrvObj;

void Key_Init(void)
{
  GPIO_Init(GPIO_Key_2,GPIO_PIN_Key_2,GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIO_Key_3,GPIO_PIN_Key_3,GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIO_Key_4,GPIO_PIN_Key_4,GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIO_Key_PTT,GPIO_PIN_Key_PTT,GPIO_MODE_IN_PU_NO_IT);
}



void key_scan()//����ɨ�������ڶ�ʱ�ж���
{
/*******PTT***********/
  if(ReadInput_KEY_PTT==0)
  {
    if(KeyscanDrvObj.key_ptt.key_lock_flag==FALSE)//�а������£����ǵ�һ�α�����
    {
      KeyscanDrvObj.key_ptt.key_time_count++; //�ۼӶ�ʱ�жϴ���
      if(KeyscanDrvObj.key_ptt.key_time_count>KEYTIME_SHORT)
      {
        KeyscanDrvObj.key_ptt.key_time_count=0;
        KeyscanDrvObj.key_ptt.key_lock_flag=TRUE;//����������λ,����һֱ����
        KeyscanDrvObj.key_ptt.key_states=m_key_press_moment;
      }
    }
  }
  else//����δ�����£���ʱ������ر�־λ
  {
    KeyscanDrvObj.key_ptt.key_lock_flag=FALSE;//����������־λ����
    KeyscanDrvObj.key_ptt.key_time_count=0;//����ȥ������������
    if(KeyscanDrvObj.key_ptt.key_states==m_key_press_state)
    {
      KeyscanDrvObj.key_ptt.key_states=m_key_loosen_moment;
    }
  }
/*******KEY2***********/
  if(ReadInput_KEY_2==0)
  {
    if(KeyscanDrvObj.key_2.key_lock_flag==FALSE)//�а������£����ǵ�һ�α�����
    {
      KeyscanDrvObj.key_2.key_time_count++; //�ۼӶ�ʱ�жϴ���
      if(KeyscanDrvObj.key_2.key_time_count>KEYTIME_SHORT)
      {
        KeyscanDrvObj.key_2.key_short_press_temp_flag=TRUE;   //������̰�����Ч��־  
      }
      if(KeyscanDrvObj.key_2.key_time_count>KEYTIME_LONG)
      {
        KeyscanDrvObj.key_2.key_short_press_temp_flag=FALSE;  //��������̰�����Ч��־
        KeyscanDrvObj.key_2.key_time_count=0;
        KeyscanDrvObj.key_2.key_lock_flag=TRUE;  //����������λ,����һֱ����
        KeyscanDrvObj.key_2.key_states=m_key_long_press; //����1�ż��ĳ���
      }
    }
  }
  else//����δ�����£���ʱ������ر�־λ
  {
    KeyscanDrvObj.key_2.key_lock_flag=FALSE;//����������־λ����
    KeyscanDrvObj.key_2.key_time_count=0;//����ȥ������������
    if(KeyscanDrvObj.key_2.key_short_press_temp_flag==TRUE)
    {
      KeyscanDrvObj.key_2.key_short_press_temp_flag=FALSE;
      KeyscanDrvObj.key_2.key_states=m_key_short_press;//����һ�ż��Ķ̰�
    }
  }
/*******KEY3***********/
  if(ReadInput_KEY_3==0)
  {
    if(KeyscanDrvObj.key_3.key_lock_flag==FALSE)//�а������£����ǵ�һ�α�����
    {
      KeyscanDrvObj.key_3.key_time_count++; //�ۼӶ�ʱ�жϴ���
      if(KeyscanDrvObj.key_3.key_time_count>KEYTIME_SHORT)
      {
        KeyscanDrvObj.key_3.key_short_press_temp_flag=TRUE;   //������̰�����Ч��־  
      }
      if(KeyscanDrvObj.key_3.key_time_count>KEYTIME_LONG)
      {
        KeyscanDrvObj.key_3.key_short_press_temp_flag=FALSE;  //��������̰�����Ч��־
        KeyscanDrvObj.key_3.key_time_count=0;
        KeyscanDrvObj.key_3.key_lock_flag=TRUE;  //����������λ,����һֱ����
        KeyscanDrvObj.key_3.key_states=m_key_long_press; //����1�ż��ĳ���
      }
    }
  }
  else//����δ�����£���ʱ������ر�־λ
  {
    KeyscanDrvObj.key_3.key_lock_flag=FALSE;//����������־λ����
    KeyscanDrvObj.key_3.key_time_count=0;//����ȥ������������
    if(KeyscanDrvObj.key_3.key_short_press_temp_flag==TRUE)
    {
      KeyscanDrvObj.key_3.key_short_press_temp_flag=FALSE;
      KeyscanDrvObj.key_3.key_states=m_key_short_press;//����һ�ż��Ķ̰�
    }
  }
/*******KEY4***********/
  if(ReadInput_KEY_4==0)
  {
    if(KeyscanDrvObj.key_4.key_lock_flag==FALSE)//�а������£����ǵ�һ�α�����
    {
      KeyscanDrvObj.key_4.key_time_count++; //�ۼӶ�ʱ�жϴ���
      if(KeyscanDrvObj.key_4.key_time_count>KEYTIME_SHORT)
      {
        KeyscanDrvObj.key_4.key_short_press_temp_flag=TRUE;   //������̰�����Ч��־  
      }
      if(KeyscanDrvObj.key_4.key_time_count>KEYTIME_LONG)
      {
        KeyscanDrvObj.key_4.key_short_press_temp_flag=FALSE;  //��������̰�����Ч��־
        KeyscanDrvObj.key_4.key_time_count=0;
        KeyscanDrvObj.key_4.key_lock_flag=TRUE;  //����������λ,����һֱ����
        KeyscanDrvObj.key_4.key_states=m_key_long_press; //����1�ż��ĳ���
      }
    }
  }
  else//����δ�����£���ʱ������ر�־λ
  {
    KeyscanDrvObj.key_4.key_lock_flag=FALSE;//����������־λ����
    KeyscanDrvObj.key_4.key_time_count=0;//����ȥ������������
    if(KeyscanDrvObj.key_4.key_short_press_temp_flag==TRUE)
    {
      KeyscanDrvObj.key_4.key_short_press_temp_flag=FALSE;
      KeyscanDrvObj.key_4.key_states=m_key_short_press;//����һ�ż��Ķ̰�
    }
  }
}

KeyStatesType get_key_ptt_states(void)
{
  return KeyscanDrvObj.key_ptt.key_states;
}
KeyStatesType get_key_2_states(void)
{
  return KeyscanDrvObj.key_2.key_states;
}
KeyStatesType get_key_3_states(void)
{
  return KeyscanDrvObj.key_3.key_states;
}
KeyStatesType get_key_4_states(void)
{
  return KeyscanDrvObj.key_4.key_states;
}
void set_key_ptt_states(KeyStatesType a)
{
  KeyscanDrvObj.key_ptt.key_states=a;
}
void set_key_2_states(KeyStatesType a)
{
  KeyscanDrvObj.key_2.key_states=a;
}
void set_key_3_states(KeyStatesType a)
{
  KeyscanDrvObj.key_3.key_states=a;
}
void set_key_4_states(KeyStatesType a)
{
  KeyscanDrvObj.key_4.key_states=a;
}