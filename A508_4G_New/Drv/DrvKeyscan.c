#include "AllHead.h"

#define KEYTIME_SHORT  20    //短按的按键去抖动延时的时间
#define KEYTIME_LONG   800     //长按的按键去抖动延时的时间

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



void key_scan()//按键扫描程序放在定时中断里
{
/*******PTT***********/
  if(ReadInput_KEY_PTT==0)
  {
    if(KeyscanDrvObj.key_ptt.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyscanDrvObj.key_ptt.key_time_count++; //累加定时中断次数
      if(KeyscanDrvObj.key_ptt.key_time_count>KEYTIME_SHORT)
      {
        KeyscanDrvObj.key_ptt.key_time_count=0;
        KeyscanDrvObj.key_ptt.key_lock_flag=TRUE;//自锁按键置位,避免一直触发
        KeyscanDrvObj.key_ptt.key_states=m_key_press_moment;
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyscanDrvObj.key_ptt.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyscanDrvObj.key_ptt.key_time_count=0;//按键去抖计数器清零
    if(KeyscanDrvObj.key_ptt.key_states==m_key_press_state)
    {
      KeyscanDrvObj.key_ptt.key_states=m_key_loosen_moment;
    }
  }
/*******KEY2***********/
  if(ReadInput_KEY_2==0)
  {
    if(KeyscanDrvObj.key_2.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyscanDrvObj.key_2.key_time_count++; //累加定时中断次数
      if(KeyscanDrvObj.key_2.key_time_count>KEYTIME_SHORT)
      {
        KeyscanDrvObj.key_2.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyscanDrvObj.key_2.key_time_count>KEYTIME_LONG)
      {
        KeyscanDrvObj.key_2.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyscanDrvObj.key_2.key_time_count=0;
        KeyscanDrvObj.key_2.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyscanDrvObj.key_2.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyscanDrvObj.key_2.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyscanDrvObj.key_2.key_time_count=0;//按键去抖计数器清零
    if(KeyscanDrvObj.key_2.key_short_press_temp_flag==TRUE)
    {
      KeyscanDrvObj.key_2.key_short_press_temp_flag=FALSE;
      KeyscanDrvObj.key_2.key_states=m_key_short_press;//触发一号键的短按
    }
  }
/*******KEY3***********/
  if(ReadInput_KEY_3==0)
  {
    if(KeyscanDrvObj.key_3.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyscanDrvObj.key_3.key_time_count++; //累加定时中断次数
      if(KeyscanDrvObj.key_3.key_time_count>KEYTIME_SHORT)
      {
        KeyscanDrvObj.key_3.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyscanDrvObj.key_3.key_time_count>KEYTIME_LONG)
      {
        KeyscanDrvObj.key_3.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyscanDrvObj.key_3.key_time_count=0;
        KeyscanDrvObj.key_3.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyscanDrvObj.key_3.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyscanDrvObj.key_3.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyscanDrvObj.key_3.key_time_count=0;//按键去抖计数器清零
    if(KeyscanDrvObj.key_3.key_short_press_temp_flag==TRUE)
    {
      KeyscanDrvObj.key_3.key_short_press_temp_flag=FALSE;
      KeyscanDrvObj.key_3.key_states=m_key_short_press;//触发一号键的短按
    }
  }
/*******KEY4***********/
  if(ReadInput_KEY_4==0)
  {
    if(KeyscanDrvObj.key_4.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyscanDrvObj.key_4.key_time_count++; //累加定时中断次数
      if(KeyscanDrvObj.key_4.key_time_count>KEYTIME_SHORT)
      {
        KeyscanDrvObj.key_4.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyscanDrvObj.key_4.key_time_count>KEYTIME_LONG)
      {
        KeyscanDrvObj.key_4.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyscanDrvObj.key_4.key_time_count=0;
        KeyscanDrvObj.key_4.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyscanDrvObj.key_4.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyscanDrvObj.key_4.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyscanDrvObj.key_4.key_time_count=0;//按键去抖计数器清零
    if(KeyscanDrvObj.key_4.key_short_press_temp_flag==TRUE)
    {
      KeyscanDrvObj.key_4.key_short_press_temp_flag=FALSE;
      KeyscanDrvObj.key_4.key_states=m_key_short_press;//触发一号键的短按
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