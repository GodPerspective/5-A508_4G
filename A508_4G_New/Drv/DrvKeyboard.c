#include "AllHead.h"

#define KEYTIME_SHORT  20    //短按的按键去抖动延时的时间
#define KEYTIME_LONG   800     //长按的按键去抖动延时的时间

typedef struct{
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_down;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_up;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_menu;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_cancel;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_ok;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_1;
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
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_5;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_6;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_7;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_8;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_9;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_0;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_star;
  struct{
    u16 key_time_count;
    bool key_lock_flag;
    bool key_short_press_temp_flag;
    KeyStatesType key_states;
  }key_well;
  bool keyboard_press_down_flag_for_backlight;
  bool keyboard_press_down_flag_for_keylock;
}KEYBOARDSCAN_DRV;

static KEYBOARDSCAN_DRV KeyboardscanDrvObj;



bool mcu_key_pad_read(u8 index);
void mcu_key_pad_set(bool inout, u8 index, bool up);
void mcu_key_pad_out(u8 index,bool state);
u32 get_key_value(u8 scan_value);
u8 get_key_name(u32 lKeyCode);

void mcu_key_initial(void)
{
    //GPIO_Init(GPIO_Key_11,GPIO_PIN_Key_11,GPIO_MODE_OUT_PP_HIGH_FAST);
  //不能专心在某件事上的问题和当前状态有很大关系，当前状态不好和之前列的计划任务未完成有关系，也和当下遇到的困难有关系
  //停止抱怨
}

bool mcu_key_pad_read(u8 index)
{
  bool value = TRUE;
  u16 PIN;
  switch (index)
  {
  case 0:
    PIN = GPIO_PIN_Key_5;// K5:PC5
    value = (bool)GPIO_ReadInputPin(GPIO_Key_5, PIN);
    break;
  case 1:
    PIN = GPIO_PIN_Key_6;// K6:PC6
    value = (bool)GPIO_ReadInputPin(GPIO_Key_6, PIN);
    break;
  case 2:
    PIN = GPIO_PIN_Key_7;// K7:PC7
    value = (bool)GPIO_ReadInputPin(GPIO_Key_7, PIN);
    break;
  case 3:
    PIN = GPIO_PIN_Key_8;// K8:PG0
    value = (bool)GPIO_ReadInputPin(GPIO_Key_8, PIN);
    break;
  case 4:
    PIN = GPIO_PIN_Key_9;// K9:PG1
    value = (bool)GPIO_ReadInputPin(GPIO_Key_9, PIN);
    break;
  case 5:
    PIN = GPIO_PIN_Key_10;// K10:PG3
    value = (bool)GPIO_ReadInputPin(GPIO_Key_10, PIN);
    break;
  default:
    break;
  }
  return value;
}

void mcu_key_pad_set(bool inout, u8 index, bool up)
{
  u16 PIN;
  if (inout)
  {
    if (up)
    {
      switch (index)
      {
      case 0:
        PIN = GPIO_PIN_Key_5;
        GPIO_Init(GPIO_Key_5,PIN,GPIO_MODE_IN_PU_NO_IT);
        break;
      case 1:
        PIN = GPIO_PIN_Key_6;
        GPIO_Init(GPIO_Key_6,PIN,GPIO_MODE_IN_PU_NO_IT);
        break;
      case 2:
        PIN = GPIO_PIN_Key_7;
        GPIO_Init(GPIO_Key_7,PIN,GPIO_MODE_IN_PU_NO_IT);
        break;
      case 3:
        PIN = GPIO_PIN_Key_8;
        GPIO_Init(GPIO_Key_8,PIN,GPIO_MODE_IN_PU_NO_IT);
        break;
      case 4:
        PIN = GPIO_PIN_Key_9;
        GPIO_Init(GPIO_Key_9,PIN,GPIO_MODE_IN_PU_NO_IT);
        break;
      case 5:
        PIN = GPIO_PIN_Key_10;
        GPIO_Init(GPIO_Key_10,PIN,GPIO_MODE_IN_PU_NO_IT);
        break;
      default:
        break;
      }
    }
    else
    {
      switch (index)
      {
      case 0:
        PIN = GPIO_PIN_Key_5;
        GPIO_Init(GPIO_Key_5,PIN,GPIO_MODE_IN_FL_NO_IT);
        break;
      case 1:
        PIN = GPIO_PIN_Key_6;
        GPIO_Init(GPIO_Key_6,PIN,GPIO_MODE_IN_FL_NO_IT);
        break;
      case 2:
        PIN = GPIO_PIN_Key_7;
        GPIO_Init(GPIO_Key_7,PIN,GPIO_MODE_IN_FL_NO_IT);
        break;
      case 3:
        PIN = GPIO_PIN_Key_8;
        GPIO_Init(GPIO_Key_8,PIN,GPIO_MODE_IN_FL_NO_IT);
        break;
      case 4:
        PIN = GPIO_PIN_Key_9;
        GPIO_Init(GPIO_Key_9,PIN,GPIO_MODE_IN_FL_NO_IT);
        break;
      case 5:
        PIN = GPIO_PIN_Key_10;
        GPIO_Init(GPIO_Key_10,PIN,GPIO_MODE_IN_FL_NO_IT);
        break;
      default:
        break;
      }
    }
  }
  else
  {
    switch (index)
    {
    case 0:
      PIN = GPIO_PIN_Key_5;
      GPIO_Init(GPIO_Key_5,PIN,GPIO_MODE_OUT_OD_LOW_FAST);
      break;
    case 1:
      PIN = GPIO_PIN_Key_6;
      GPIO_Init(GPIO_Key_6,PIN,GPIO_MODE_OUT_OD_LOW_FAST);
      break;
    case 2:
      PIN = GPIO_PIN_Key_7;
      GPIO_Init(GPIO_Key_7,PIN,GPIO_MODE_OUT_OD_LOW_FAST);
      break;
    case 3:
      PIN = GPIO_PIN_Key_8;
      GPIO_Init(GPIO_Key_8,PIN,GPIO_MODE_OUT_OD_LOW_FAST);
      break;
    case 4:
      PIN = GPIO_PIN_Key_9;
      GPIO_Init(GPIO_Key_9,PIN,GPIO_MODE_OUT_OD_LOW_FAST);
      break;
    case 5:
      PIN = GPIO_PIN_Key_10;
      GPIO_Init(GPIO_Key_10,PIN,GPIO_MODE_OUT_OD_LOW_FAST);
      break;
    default:
      break;
    }
  }
}

void mcu_key_pad_out(u8 index,bool state)
{
  if(state==FALSE)
  {
    switch (index)
    {
		case 0:
			GPIO_WriteLow(GPIO_Key_5, GPIO_PIN_Key_5);
			break;
		case 1:
			GPIO_WriteLow(GPIO_Key_6, GPIO_PIN_Key_6);
			break;
		case 2:
			GPIO_WriteLow(GPIO_Key_7, GPIO_PIN_Key_7);
			break;
		case 3:
			GPIO_WriteLow(GPIO_Key_8, GPIO_PIN_Key_8);
			break;
		case 4:
			GPIO_WriteLow(GPIO_Key_9, GPIO_PIN_Key_9);
			break;
		case 5:
			GPIO_WriteLow(GPIO_Key_10, GPIO_PIN_Key_10);
			break;
		default:
			break;
    }
  }
  else
  {
    switch (index)
    {
		case 0:
			GPIO_WriteHigh(GPIO_Key_5, GPIO_PIN_Key_5);
			break;
		case 1:
			GPIO_WriteHigh(GPIO_Key_6, GPIO_PIN_Key_6);
			break;
		case 2:
			GPIO_WriteHigh(GPIO_Key_7, GPIO_PIN_Key_7);
			break;
		case 3:
			GPIO_WriteHigh(GPIO_Key_8, GPIO_PIN_Key_8);
			break;
		case 4:
			GPIO_WriteHigh(GPIO_Key_9, GPIO_PIN_Key_9);
			break;
		case 5:
			GPIO_WriteHigh(GPIO_Key_10, GPIO_PIN_Key_10);
			break;
		default:
			break;
    }
  }
	
}

/**********************************************************************
Description     :  none
Input           :  none
Output          :  none
Return          :  none
Others          :  
					OK	DN	UP Cancel
					14	24	34	44

					1	2	3	*
					13	23 33	43

					4	5	6	0
					12	22	32	42

					7	8	9	#
					11	21	31	41
**********************************************************************/
u8 key_scan_board(u8 keycnt)
{
	u8 value = 0,i;

	switch (keycnt)
	{
		case 0:
			for (i=0; i<5; i++)
			{
				if (mcu_key_pad_read(i) == FALSE)
				{
					switch (i)
					{
						case 0:
							value = 0x14;
							break;
						case 1:
							value = 0x24;
							break;
						case 2:
							value = 0x34;
							break;
						case 3:
							value = 0x44;
							break;
						case 4:
							value = 0x54;
							break;
						default:
							break;
					}
				}
			}		
			break;
		case 1:
			for (i=0; i<5; i++)
			{
				if (mcu_key_pad_read(i) == FALSE)
				{
					switch (i)
					{
						case 0:
							value = 0x11;
							break;
						case 1:
							value = 0x21;
							break;
						case 2:
							value = 0x31;
							break;
						case 3:
							value = 0x41;
							break;
						case 4:
							value = 0x42;
							break;
						default:
							break;
					}
				}
			}	
			break;
		case 2:
			for (i=0; i<4; i++)
			{
				if (mcu_key_pad_read(i) == FALSE)
				{
					switch (i)
					{
						case 0:
							value = 0x12;
							break;
						case 1:
							value = 0x22;
							break;
						case 2:
							value = 0x32;
							break;
						case 3:
							value = 0x43;
							break;
						default:
							break;
					}
					
					break;
				}
			}
			break;
		case 3:
			for (i=0; i<3; i++)
			{
				if (mcu_key_pad_read(i) == FALSE)
				{
					switch (i)
					{
						case 0:
							value = 0x13;// 
							break;
						case 1:
							value = 0x23;
							break;
						case 2:
							value = 0x33;
							break;
						default:
							break;
					}
				}
			}	
			break;
		case 4:
			
			break;
		case 5:
			
			break;
		default:
			break;
	}

	return value;
}

u8 drv_keypad_scan(void)
{
	u8 key = 0;
	u8 scankeycnt;

	for (scankeycnt=0; scankeycnt<4; scankeycnt++)
	{
		switch (scankeycnt)
		{
			case 0://OK、L、R、C、MENU、
				mcu_key_pad_set(TRUE, 0, TRUE);//输入上拉 	//K5
				mcu_key_pad_set(TRUE, 1, TRUE);			//K6	
				mcu_key_pad_set(TRUE, 2, TRUE);			//K7
				mcu_key_pad_set(TRUE, 3, TRUE);			//K8
				mcu_key_pad_set(TRUE, 4, TRUE);			//K9		
				mcu_key_pad_set(TRUE, 5, TRUE);			//K10	
						
				break;
			case 1:// 扫描按键7 8 9 # 0
				mcu_key_pad_set(TRUE, 0, TRUE);//输入下拉
				mcu_key_pad_set(TRUE, 1, TRUE);			
				mcu_key_pad_set(TRUE, 2, TRUE);
				mcu_key_pad_set(TRUE, 3, TRUE);
				mcu_key_pad_set(TRUE, 4, TRUE);
				mcu_key_pad_set(FALSE,5, FALSE);//输出
				mcu_key_pad_out(5, FALSE);//输出高电平
			
				break;
			case 2:// 扫描按键4 5 6 *
				mcu_key_pad_set(TRUE, 0, TRUE);
				mcu_key_pad_set(TRUE, 1, TRUE);	
				mcu_key_pad_set(TRUE, 2, TRUE);
				mcu_key_pad_set(TRUE, 3, TRUE);
				mcu_key_pad_set(FALSE, 4, FALSE);//输出
				mcu_key_pad_out(4, FALSE);
				mcu_key_pad_set(FALSE,5, FALSE);
				break;
			case 3:// 扫描按键1 2 3
				mcu_key_pad_set(TRUE, 0, TRUE);
				mcu_key_pad_set(TRUE, 1, TRUE);
				mcu_key_pad_set(TRUE, 2, TRUE);
				mcu_key_pad_set(FALSE, 3, FALSE);//输出
				mcu_key_pad_out(3, FALSE);
				mcu_key_pad_set(FALSE,4, FALSE);
				mcu_key_pad_set(FALSE,5, FALSE);
				break;
			case 4:

				break;
			case 5:

				break;
			default:
				break;
		}
		key = key_scan_board(scankeycnt);
		mcu_key_pad_set(FALSE, 0, FALSE);	 	//K5
		mcu_key_pad_set(FALSE, 1, FALSE);			//K6	
		mcu_key_pad_set(FALSE, 2, FALSE);			//K7
		mcu_key_pad_set(FALSE, 3, FALSE);			//K8
		mcu_key_pad_set(FALSE, 4, FALSE);			//K9		
		mcu_key_pad_set(FALSE, 5, FALSE);			//K10	
		mcu_key_pad_out(0, FALSE);
		mcu_key_pad_out(1, FALSE);
		mcu_key_pad_out(2, FALSE);
		mcu_key_pad_out(3, FALSE);
		mcu_key_pad_out(4, FALSE);
		mcu_key_pad_out(5, FALSE);	
		if (key != 0)
		{
			if ( key != 0x14 && key != 0x24 && key != 0x34 && key != 0x44 && key != 0x54)
			{
				//scankeycnt = 0;
			}
			break;
		}
	}

	return key;
}

u32 get_key_value(u8 scan_value)
{
  u32 value = 0;
  switch (scan_value)
  {
  case 0x11://"7"
    value = 0x00002000;
    break;
  case 0x12://"4"
    value = 0x00000080;
    break;
  case 0x13://"1"
    value = 0x00000002;
    break;
  case 0x14://OK
    value = 0x00000010;
    break;
  case 0x21://"8"
    value = 0x00004000;
    break;
  case 0x22://"5"
    value = 0x00000100;
    break;
  case 0x23://"2"
    value = 0x00000004;
    break;
  case 0x24://"DN"
    value = 0x00010000;
    break;
  case 0x31://"9"
    value = 0x00008000;
    break;
  case 0x32://"6"
    value = 0x00000200;
    break;
  case 0x33://"3"
    value = 0x00000008;
    break;
  case 0x34://"UP"
    value = 0x00000400;
    break;
  case 0x41://"#"
    value = 0x00200000;
    break;
  case 0x42://"0"
    value = 0x00100000;
    break;
  case 0x43://"*"
    value = 0x00080000;
    break;
  case 0x44://"Cancel"
    value = 0x00400000;
    break;
  case 0x54://"Menu"
    value = 0x00800000;
    break;
  default:
    break;
  }
  return value;
}

void keyboard_scan(void)
{
  u8 scanvalue = 0;
  u32 ulAllKeyID = 0x00000000;

  ulAllKeyID = 0x00000000;
  scanvalue = drv_keypad_scan();
  ulAllKeyID = get_key_value(scanvalue);
/********判断是否有按键按下************/
  if(ulAllKeyID!=0x00000000)
  {
    if(KeyboardscanDrvObj.key.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key.key_time_count=0;
        KeyboardscanDrvObj.key.key_lock_flag=TRUE;//自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key.key_states=m_key_press_moment;
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key.key_states==m_key_press_moment)
    {
      KeyboardscanDrvObj.key.key_states=m_key_loosen_moment;
    }
  }
/*******键盘down****************************/
  if(ulAllKeyID==0x00010000)//
  {
    if(KeyboardscanDrvObj.key_down.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_down.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_down.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_down.key_time_count=0;
        KeyboardscanDrvObj.key_down.key_lock_flag=TRUE;//自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_down.key_states=m_key_press_moment;
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_down.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_down.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_down.key_states==m_key_press_moment)
    {
      KeyboardscanDrvObj.key_down.key_states=m_key_loosen_moment;
    }
  }
  
  
/*******键盘up****************************/
  if(ulAllKeyID==0x00000400)//up
  {
    if(KeyboardscanDrvObj.key_up.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_up.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_up.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_up.key_time_count=0;
        KeyboardscanDrvObj.key_up.key_lock_flag=TRUE;//自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_up.key_states=m_key_press_moment;
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_up.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_up.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_up.key_states==m_key_press_moment)
    {
      KeyboardscanDrvObj.key_up.key_states=m_key_loosen_moment;
    }
  }
  
/*******键盘menu****************************/
  if(ulAllKeyID==0x00800000)
  {
    if(KeyboardscanDrvObj.key_menu.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_menu.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_menu.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_menu.key_time_count=0;
        KeyboardscanDrvObj.key_menu.key_lock_flag=TRUE;//自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_menu.key_states=m_key_press_moment;
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_menu.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_menu.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_menu.key_states==m_key_press_moment)
    {
      KeyboardscanDrvObj.key_menu.key_states=m_key_loosen_moment;
    }
  }
  
/*******键盘cancel****************************/
  if(ulAllKeyID==0x00400000)
  {
    if(KeyboardscanDrvObj.key_cancel.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_cancel.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_cancel.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_cancel.key_time_count=0;
        KeyboardscanDrvObj.key_cancel.key_lock_flag=TRUE;//自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_cancel.key_states=m_key_press_moment;
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_cancel.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_cancel.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_cancel.key_states==m_key_press_moment)
    {
      KeyboardscanDrvObj.key_cancel.key_states=m_key_loosen_moment;
    }
  }
/*******键盘ok****************************/
  if(ulAllKeyID==0x00000010)
  {
    if(KeyboardscanDrvObj.key_ok.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_ok.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_ok.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_ok.key_time_count=0;
        KeyboardscanDrvObj.key_ok.key_lock_flag=TRUE;//自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_ok.key_states=m_key_press_moment;
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_ok.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_ok.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_ok.key_states==m_key_press_moment)
    {
      KeyboardscanDrvObj.key_ok.key_states=m_key_loosen_moment;
    }
  }
/*******键盘1****************************/
  if(ulAllKeyID==0x00000002)//
  {
    if(KeyboardscanDrvObj.key_1.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_1.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_1.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_1.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_1.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_1.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_1.key_time_count=0;
        KeyboardscanDrvObj.key_1.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_1.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_1.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_1.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_1.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_1.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_1.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  
/*******键盘2****************************/
  if(ulAllKeyID==0x00000004)//
  {
    if(KeyboardscanDrvObj.key_2.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_2.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_2.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_2.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_2.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_2.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_2.key_time_count=0;
        KeyboardscanDrvObj.key_2.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_2.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_2.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_2.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_2.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_2.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_2.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘3****************************/
  if(ulAllKeyID==0x00000008)
  {
    if(KeyboardscanDrvObj.key_3.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_3.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_3.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_3.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_3.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_3.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_3.key_time_count=0;
        KeyboardscanDrvObj.key_3.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_3.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_3.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_3.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_3.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_3.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_3.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘4****************************/
  if(ulAllKeyID==0x00000080)
  {
    if(KeyboardscanDrvObj.key_4.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_4.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_4.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_4.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_4.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_4.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_4.key_time_count=0;
        KeyboardscanDrvObj.key_4.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_4.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_4.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_4.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_4.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_4.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_4.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘5****************************/
  if(ulAllKeyID==0x00000100)
  {
    if(KeyboardscanDrvObj.key_5.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_5.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_5.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_5.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_5.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_5.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_5.key_time_count=0;
        KeyboardscanDrvObj.key_5.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_5.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_5.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_5.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_5.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_5.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_5.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘6****************************/
  if(ulAllKeyID==0x00000200)
  {
    if(KeyboardscanDrvObj.key_6.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_6.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_6.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_6.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_6.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_6.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_6.key_time_count=0;
        KeyboardscanDrvObj.key_6.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_6.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_6.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_6.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_6.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_6.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_6.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘7****************************/
  if(ulAllKeyID==0x00002000)
  {
    if(KeyboardscanDrvObj.key_7.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_7.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_7.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_7.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_7.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_7.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_7.key_time_count=0;
        KeyboardscanDrvObj.key_7.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_7.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_7.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_7.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_7.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_7.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_7.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘8****************************/
  if(ulAllKeyID==0x00004000)
  {
    if(KeyboardscanDrvObj.key_8.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_8.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_8.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_8.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_8.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_8.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_8.key_time_count=0;
        KeyboardscanDrvObj.key_8.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_8.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_8.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_8.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_8.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_8.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_8.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘9****************************/
  if(ulAllKeyID==0x00008000)
  {
    if(KeyboardscanDrvObj.key_9.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_9.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_9.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_9.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_9.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_9.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_9.key_time_count=0;
        KeyboardscanDrvObj.key_9.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_9.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_9.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_9.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_9.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_9.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_9.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘0****************************/
  if(ulAllKeyID==0x00100000)
  {
    if(KeyboardscanDrvObj.key_0.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_0.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_0.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_0.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_0.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_0.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_0.key_time_count=0;
        KeyboardscanDrvObj.key_0.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_0.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_0.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_0.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_0.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_0.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_0.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘star*****************************/
  if(ulAllKeyID==0x00080000)
  {
    if(KeyboardscanDrvObj.key_star.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_star.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_star.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_star.key_short_press_temp_flag=TRUE;   //激活按键短按的有效标志  
      }
      if(KeyboardscanDrvObj.key_star.key_time_count>80)//长按800ms
      {
        KeyboardscanDrvObj.key_star.key_short_press_temp_flag=FALSE;  //清除按键短按的有效标志
        KeyboardscanDrvObj.key_star.key_time_count=0;
        KeyboardscanDrvObj.key_star.key_lock_flag=TRUE;  //自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_star.key_states=m_key_long_press; //触发1号键的长按
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_star.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_star.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_star.key_short_press_temp_flag==TRUE)
    {
      KeyboardscanDrvObj.key_star.key_short_press_temp_flag=FALSE;
      KeyboardscanDrvObj.key_star.key_states=m_key_short_press;//触发一号键的短按
    }
  }
  /*******键盘#****************************/
  if(ulAllKeyID==0x00200000)//well
  {
    if(KeyboardscanDrvObj.key_well.key_lock_flag==FALSE)//有按键按下，且是第一次被按下
    {
      KeyboardscanDrvObj.key_well.key_time_count++; //累加定时中断次数
      if(KeyboardscanDrvObj.key_well.key_time_count>2)//短按20ms
      {
        KeyboardscanDrvObj.key_well.key_time_count=0;
        KeyboardscanDrvObj.key_well.key_lock_flag=TRUE;//自锁按键置位,避免一直触发
        KeyboardscanDrvObj.key_well.key_states=m_key_press_moment;
      }
    }
  }
  else//按键未被按下，及时清零相关标志位
  {
    KeyboardscanDrvObj.key_well.key_lock_flag=FALSE;//按键自锁标志位清零
    KeyboardscanDrvObj.key_well.key_time_count=0;//按键去抖计数器清零
    if(KeyboardscanDrvObj.key_well.key_states==m_key_press_moment)
    {
      KeyboardscanDrvObj.key_well.key_states=m_key_loosen_moment;
    }
  }
}

KeyStatesType get_keyboard_key_states(void)
{
  return KeyboardscanDrvObj.key.key_states;
}
KeyStatesType get_keyboard_down_states(void)
{
  return KeyboardscanDrvObj.key_down.key_states;
}
KeyStatesType get_keyboard_up_states(void)
{
  return KeyboardscanDrvObj.key_up.key_states;
}
KeyStatesType get_keyboard_menu_states(void)
{
  return KeyboardscanDrvObj.key_menu.key_states;
}
KeyStatesType get_keyboard_cancel_states(void)
{
  return KeyboardscanDrvObj.key_cancel.key_states;
}
KeyStatesType get_keyboard_ok_states(void)
{
  return KeyboardscanDrvObj.key_ok.key_states;
}
KeyStatesType get_keyboard_1_states(void)
{
  return KeyboardscanDrvObj.key_1.key_states;
}
KeyStatesType get_keyboard_2_states(void)
{
  return KeyboardscanDrvObj.key_2.key_states;
}
KeyStatesType get_keyboard_3_states(void)
{
  return KeyboardscanDrvObj.key_3.key_states;
}
KeyStatesType get_keyboard_4_states(void)
{
  return KeyboardscanDrvObj.key_4.key_states;
}
KeyStatesType get_keyboard_5_states(void)
{
  return KeyboardscanDrvObj.key_5.key_states;
}
KeyStatesType get_keyboard_6_states(void)
{
  return KeyboardscanDrvObj.key_6.key_states;
}
KeyStatesType get_keyboard_7_states(void)
{
  return KeyboardscanDrvObj.key_7.key_states;
}
KeyStatesType get_keyboard_8_states(void)
{
  return KeyboardscanDrvObj.key_8.key_states;
}
KeyStatesType get_keyboard_9_states(void)
{
  return KeyboardscanDrvObj.key_9.key_states;
}
KeyStatesType get_keyboard_0_states(void)
{
  return KeyboardscanDrvObj.key_0.key_states;
}
KeyStatesType get_keyboard_star_states(void)
{
  return KeyboardscanDrvObj.key_star.key_states;
}
KeyStatesType get_keyboard_well_states(void)
{
  return KeyboardscanDrvObj.key_well.key_states;
}
void set_keyboard_key_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key.key_states=a;
}
void set_keyboard_down_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_down.key_states=a;
}
void set_keyboard_up_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_up.key_states=a;
}
void set_keyboard_menu_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_menu.key_states=a;
}
void set_keyboard_cancel_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_cancel.key_states=a;
}
void set_keyboard_ok_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_ok.key_states=a;
}
void set_keyboard_1_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_1.key_states=a;
}
void set_keyboard_2_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_2.key_states=a;
}
void set_keyboard_3_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_3.key_states=a;
}
void set_keyboard_4_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_4.key_states=a;
}
void set_keyboard_5_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_5.key_states=a;
}
void set_keyboard_6_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_6.key_states=a;
}
void set_keyboard_7_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_7.key_states=a;
}
void set_keyboard_8_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_8.key_states=a;
}
void set_keyboard_9_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_9.key_states=a;
}
void set_keyboard_0_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_0.key_states=a;
}
void set_keyboard_star_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_star.key_states=a;
}
void set_keyboard_well_states(KeyStatesType a)
{
  KeyboardscanDrvObj.key_well.key_states=a;
}