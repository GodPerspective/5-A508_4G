#define DELLABEL
#include "AllHead.h"

#define DEL10MSLEN		0x0A
#define DEL_IDLE		0x00
#define DEL_RUN			0x01

#if 1//ZM389
typedef struct {
  union {
    struct {
      u16 b1ms  	: 1;
      u16 b10ms 	: 1;
      u16 b100ms	: 1;
      u16 b500ms	: 1;
      u16 b1S		: 1;
      u16 bTimeSet	: 1;
      u16 bTime0	: 1;
      u16 bTime1	: 1;
      u16 b500Alternate : 1;
      u16		: 7;
    }Bit;
    u16 Byte;
  }Msg;
  u8   c10msLen;
  u8   c100msLen;
  u8   c500msLen;
  u8   c1SLen;
  u8   c2SLen;
  u16  iTimer0;
  u16  iTimer1;
  struct{
    u8 alarm_count;
    u8 nosimcard_count;
    u8 lte_no_cell_count;
    u8 csq_count;
    u8 network_search_count;
    u8 poc_status_count;
    u8 poc_first_enter_into_group_flag_count;
    u16 TimeCount_Light;
    u8 TimeCount2;
    u8 receive_sos_statas_count;
    u8 login_step_6_count;
  }Count;
  u8 BacklightTimeBuf[1];//背光灯时间(需要设置进入eeprom)
  u8 KeylockTimeBuf[1];//键盘锁时间(需要设置进入eeprom)
}DEL_DRV;
#else
typedef struct {
  union {
    struct {
      u16 b1ms  	: 1;
      u16 b10ms 	: 1;
      u16 b100ms	: 1;
      u16 b500ms	: 1;
      u16 b1S		: 1;
      u16 bTimeSet	: 1;
      u16 bTime0	: 1;
      u16 bTime1	: 1;
      u16 b500Alternate : 1;
      u16		: 7;
    }Bit;
    u16 Byte;
  }Msg;
  u8   c10msLen;
  u8   c100msLen;
  u8   c500msLen;
  u8   c1SLen;
  u8   c2SLen;
  u16  iTimer0;
  u16  iTimer1;
  struct{
    u8 TimeCount_Light;
    u8 CSQTimeCount;
    u8 WriteFreqTimeCount;
    u8 LobatteryTask_StartCount;
    u8 PrimaryLowPowerCount;
    u8 SignalPoorCount;
    u8 TimeCount2;
    u8 CIMICount;
    u8 NoCardCount;
    u8 PPPStatusOpenCount;
    u8 ToneStateCount;
    u8 ReceivedVoicePlayStatesCount;
    u8 beidou_valid_count;
    u8 poc_status_count;
    u8 choose_write_freq_or_gps_count;
    u8 receive_sos_statas_count;
    u8 ztts_states_intermediate_count;
    u8 ztts_states_count ;
    u8 alarm_count;
    u8 poc_first_enter_into_group_flag_count;
    u8 poc_gps_value_for_display_flag_count;
  }Count;
  bool poc_gps_value_for_display_flag2;
  u8 BacklightTimeBuf[1];//背光灯时间(需要设置进入eeprom)
  u8 KeylockTimeBuf[1];//键盘锁时间(需要设置进入eeprom)
}DEL_DRV;
#endif

u16 TimeCount;//超时时间
u16 TimeCount3;
bool LockingState_Flag;
u8 BacklightTimeCount;//背光灯时间(需要设置进入eeprom)
u16 KeylockTimeCount;//键盘锁时间(需要设置进入eeprom)

static DEL_DRV	DelDrvObj;
static void DEL_100msProcess(void);
static void DEL_500msProcess(void);
static void DEL_1msProcess(void);
static void DEL_10msProcess(void);
static void DEL_TimerRenew(void);

#if 1//ZM389
void DEL_PowerOnInitial(void)//原瑞撒單片機多長時間進一次中斷
{
  Tim3_Timer_Init();
  //C_TEST_OUT_SET();
  DelDrvObj.Msg.Byte 	= 0x00;
  DelDrvObj.c10msLen  = DEL10MSLEN;
  DelDrvObj.c100msLen = 0x0A;
  DelDrvObj.c500msLen = 0x05;
  DelDrvObj.c1SLen    = 0x01;
  DelDrvObj.c2SLen    = 0x02;
  
  DelDrvObj.Count.alarm_count=0;
  DelDrvObj.Count.csq_count=0;
  DelDrvObj.Count.lte_no_cell_count=0;
  DelDrvObj.Count.nosimcard_count=0;
}
#else
void DEL_PowerOnInitial(void)//原瑞撒單片機多長時間進一次中斷
{
  Tim3_Timer_Init();
  C_TEST_OUT_SET();
  DelDrvObj.Msg.Byte 	= 0x00;
  DelDrvObj.c10msLen  = DEL10MSLEN;
  DelDrvObj.c100msLen = 0x0A;
  DelDrvObj.c500msLen = 0x05;
  DelDrvObj.c1SLen    = 0x01;
  DelDrvObj.c2SLen    = 0x02;
  
  DelDrvObj.Count.TimeCount_Light = 0;
  DelDrvObj.Count.CSQTimeCount = 0;
  DelDrvObj.Count.WriteFreqTimeCount = 0;
  DelDrvObj.Count.LobatteryTask_StartCount = 0;
  DelDrvObj.Count.PrimaryLowPowerCount = 0;
  DelDrvObj.Count.SignalPoorCount = 0;
  DelDrvObj.Count.TimeCount2 = 0;
  DelDrvObj.Count.CIMICount = 0;
  DelDrvObj.Count.NoCardCount = 0;
  DelDrvObj.Count.PPPStatusOpenCount = 0;
  DelDrvObj.Count.ToneStateCount = 0;
  DelDrvObj.Count.ReceivedVoicePlayStatesCount = 0;
  DelDrvObj.Count.beidou_valid_count = 0;
  DelDrvObj.Count.poc_status_count = 0;
  DelDrvObj.Count.choose_write_freq_or_gps_count = 0;
  DelDrvObj.Count.receive_sos_statas_count = 0;
  DelDrvObj.Count.ztts_states_intermediate_count = 0;
  DelDrvObj.Count.ztts_states_count = 0;
  DelDrvObj.Count.alarm_count = 0;
  DelDrvObj.Count.poc_first_enter_into_group_flag_count = 0;
  DelDrvObj.Count.poc_gps_value_for_display_flag_count=0;
}
#endif

void DEL_Interrupt(void)
{
  DelDrvObj.c10msLen--;
  DelDrvObj.Msg.Bit.b1ms = DEL_RUN;
  //enableInterrupts();
  if (DelDrvObj.c10msLen == 0x00) //10ms interrupt process
  {
    DEL_TimerRenew();//delay timer renew process
    DelDrvObj.Msg.Bit.b10ms = DEL_RUN;
    DelDrvObj.c10msLen = DEL10MSLEN;
    DelDrvObj.c100msLen--;
    if (DelDrvObj.c100msLen == 0x00) //100ms interrupt process
    {
      DelDrvObj.Msg.Bit.b100ms = DEL_RUN;
      DelDrvObj.c100msLen = 0x0A;
      DelDrvObj.c500msLen--;
      if (DelDrvObj.c500msLen == 0x00) //500ms interrupt process
      {	
        DelDrvObj.Msg.Bit.b500ms = DEL_RUN;
        DelDrvObj.c500msLen = 0x05;
        DelDrvObj.c1SLen--;
        if (DelDrvObj.c1SLen == 0x00) //1s interrupt process
        {
          DelDrvObj.Msg.Bit.b1S = DEL_RUN;
          DelDrvObj.c1SLen = 0x02;	
          DelDrvObj.c2SLen--;
          if (DelDrvObj.c2SLen == 0x00)	//2s interrupt process
          {
            //DelDrvObj.Msg.Bit.b2S = DEL_RUN;
            DelDrvObj.c2SLen = 0x02;
          }
        }
      }
    }
  }
  return;
}

void DEL_Renew(void) 
{
  DEL_1msProcess();
  DEL_10msProcess();
  DEL_100msProcess();
  DEL_500msProcess();
  return;
}

void Delay_100ms(u8 T)
{
  u16 i;
  u8 j,k,l;
    for(j = 0; j < 83; j++)
    for(l = 0; l < 10; l++)
        for(k = 0; k < 100; k++)
          for(i = 0; i < T; i++)
      {
        nop();
      }
  return;
}

void DEL_Soft1ms(u16 iLen) 
{
  u16 i;
  for(; iLen > 0; iLen--)
  {
    for( i = 0 ; i < 250; i++)
    {
      nop();
      nop();
      nop();
      nop();
      nop();
    }
  }
  return;
}

bool DEL_SetTimer(u8 cId,u16 iLen)
{
  DelDrvObj.Msg.Bit.bTimeSet = 0x01;
  switch (cId)
  {
  case 0x00:
    if(iLen == 0x00)
    {
      DelDrvObj.Msg.Bit.bTime0 = DEL_IDLE;
    }
    else
    {
      DelDrvObj.Msg.Bit.bTime0 = DEL_RUN;
    }
    DelDrvObj.iTimer0 = iLen;
    break;
  case 0x01:
    if(iLen == 0x00)
    {
      DelDrvObj.Msg.Bit.bTime1 = DEL_IDLE;
    }
    else
    {
      DelDrvObj.Msg.Bit.bTime1 = DEL_RUN;
    }
    DelDrvObj.iTimer1 = iLen;
    break;
  default:
    return FALSE;
  }
  DelDrvObj.Msg.Bit.bTimeSet = 0x00;
  return TRUE;
}

bool DEL_GetTimer(u8 cId)
{
	bool r;

	r = FALSE;
	//1
        switch (cId)
	{
	case 0x00:
		if ((DelDrvObj.Msg.Bit.bTime0 == DEL_RUN) && (DelDrvObj.iTimer0 == 0x00))
		{
			r = TRUE;
			DelDrvObj.Msg.Bit.bTime0 = DEL_IDLE;
		}
		break;
	case 0x01:
		if ((DelDrvObj.Msg.Bit.bTime1 == DEL_RUN) && (DelDrvObj.iTimer1 == 0x00))
		{
			r = TRUE;
			DelDrvObj.Msg.Bit.bTime1 = DEL_IDLE;			
		}
		break;
	default:
		break;
	}
	return r;
}

static void DEL_TimerRenew(void)
{
  if(DelDrvObj.Msg.Bit.bTimeSet == 0x00)
  {
    if (DelDrvObj.iTimer0 != 0x00)
    {
      DelDrvObj.iTimer0--;
    }
    if (DelDrvObj.iTimer1 != 0x00)
    {
      DelDrvObj.iTimer1--;
    }
  }
  return;
}

static void DEL_100msProcess(void)
{
  if (DelDrvObj.Msg.Bit.b100ms == DEL_RUN)
  {
    DelDrvObj.Msg.Bit.b100ms = DEL_IDLE;
    LED_IntOutputRenew();//LED output renew process
#ifdef BEIDOU
    ApiBeidou_Get_location_Information();
#else
    //ApiAtCmd_Get_location_Information();
#endif
    if(DelDrvObj.Msg.Bit.b500Alternate == DEL_IDLE)
    {
      DelDrvObj.Msg.Bit.b500Alternate = DEL_RUN;
      ApiAtCmd_100msRenew();
    }
    else
    {
      DelDrvObj.Msg.Bit.b500Alternate = DEL_IDLE;
    }

/******背光灯定时*********************************/
    FILE_Read(0x236,1,DelDrvObj.BacklightTimeBuf);//背光时间【秒】
    FILE_Read(0x247,1,DelDrvObj.KeylockTimeBuf);//键盘锁时间【秒】
    BacklightTimeCount=5*DelDrvObj.BacklightTimeBuf[0];
    DelDrvObj.Count.TimeCount_Light++;
    if(DelDrvObj.Count.TimeCount_Light>=10*BacklightTimeCount)//10s
    {
      MCU_LCD_BACKLIGTH(OFF);//关闭背光灯
      DelDrvObj.Count.TimeCount_Light=10*BacklightTimeCount;
    }
    else
    {
      MCU_LCD_BACKLIGTH(ON);//打开背光灯
    }
    
    if(get_keyboard_key_states()==m_key_loosen_moment)
    {
      DelDrvObj.Count.TimeCount_Light=0;//背光灯计数器清零
    }
/*******键盘锁定时**********/
    if(DelDrvObj.KeylockTimeBuf[0]==0)
      KeylockTimeCount=200*10;//如果=200则永远不锁屏
    else
      KeylockTimeCount=5*DelDrvObj.KeylockTimeBuf[0];
    
    if(TaskDrvobj.Id==TASK_NORMAL)
    {
      if(KeylockTimeCount==200*10)
      {
        TimeCount=0;
      }
      else
      {
        TimeCount++;
        if(TimeCount>=KeylockTimeCount*10) //超时则锁屏
        {
          if(TimeCount==KeylockTimeCount*10)
          {
            LockingState_Flag=TRUE;//超时锁定标志位
            MenuDisplay(Menu_RefreshAllIco);
            get_screen_display_group_name();//选择显示当前群组昵称（群组或单呼临时群组）
            api_disp_all_screen_refresh();// 全屏统一刷新

            MenuModeCount=1;
            TheMenuLayer_Flag=0;
            MenuMode_Flag=0;
            ApiMenu_SwitchGroup_Flag=0;
            ApiMenu_SwitchCallUser_Flag=0;
            ApiMenu_SwitchOnlineUser_Flag=0;
            ApiMenu_GpsInfo_Flag=0;
            ApiMenu_BacklightTimeSet_Flag=0;
            ApiMenu_KeylockTimeSet_Flag=0;
            ApiMenu_NativeInfo_Flag=0;
            ApiMenu_BeiDouOrWritingFrequency_Flag=0;
          }
          TimeCount=KeylockTimeCount*10+1;
        }
        else
        {
        }
        
        if(LockingState_EnterOK_Flag==TRUE)//锁定界面按下OK键
        {
          TimeCount3++;
          if(TimeCount3>=4*10)//4s
          {
            TimeCount3=0;
            LockingState_EnterOK_Flag=FALSE;
            MenuDisplay(Menu_Locking_NoOperation);
          }
        }
        else
        {
          if(get_keyboard_key_states()==m_key_loosen_moment&&TimeCount>=KeylockTimeCount*10)//超过10秒后再按按键提示“按OK键再按*键”
          {
            DelDrvObj.Count.TimeCount2++;
            DISPLAY_Show(d_press_ok_then_well);//显示先按ok键再按#
            if(DelDrvObj.Count.TimeCount2>=1*10)//0.5s
            {
              DelDrvObj.Count.TimeCount2=0;
              set_keyboard_key_states(m_key_idle);
              MenuDisplay(Menu_Locking_NoOperation);
            }
          }
        }
      }
      if(get_keyboard_key_states()==m_key_loosen_moment&&TimeCount<KeylockTimeCount*10)//当数字数字键盘按下
      {
        TimeCount=0;//当有按键按下，计数器清零
        set_keyboard_key_states(m_key_idle);
      }
    }
  }
  return;
}

static void DEL_500msProcess(void)			//delay 500ms process server
{
  if (DelDrvObj.Msg.Bit.b500ms == DEL_RUN) 
  {
    DelDrvObj.Msg.Bit.b500ms = DEL_IDLE;
    VOICE_1sProcess();
/********播报搜索网络*********/
   /* if(AtCmdDrvobj.Msg.Bits.bZGIPDNS==0)
    {
      DelDrvObj.Count.network_search_count++;
      if(DelDrvObj.Count.network_search_count>5*2)
      {
        DelDrvObj.Count.network_search_count=0;
        VOICE_Play(NetworkSearching);
        DISPLAY_Show(d_NetworkSearching);
      }
    }
    else
    {
      DelDrvObj.Count.network_search_count=0;
    }*/
/*********开机检测SIM卡*************/
    if(AtCmdDrvobj.Msg.Bits.bNoSimCard==1)
    {
        DelDrvObj.Count.nosimcard_count++;
        if(DelDrvObj.Count.nosimcard_count>2*8)
        {
          DelDrvObj.Count.nosimcard_count=0;
          //VOICE_Play(NoSimCard);
          //DISPLAY_Show(d_NoSimCard);
        }
    }
    else
    {
      DelDrvObj.Count.nosimcard_count=0;
    }
/*********卡异常*****************/
    if(AtCmdDrvobj.Msg.Bits.bZLTENOCELL==1)
    {
      DelDrvObj.Count.lte_no_cell_count++;
        if(DelDrvObj.Count.lte_no_cell_count>2*8)
        {
          DelDrvObj.Count.lte_no_cell_count=0;
          //VOICE_Play(SimCardError);
          //DISPLAY_Show(d_NoSimCard);
        }
    }
/*********定时5s发一次[AT+CSQ?]*************************************************/
    DelDrvObj.Count.csq_count++;
    if(DelDrvObj.Count.csq_count>=2*9)
    {
      DelDrvObj.Count.csq_count=0;
      ApiAtCmd_WritCommand(ATCOMM_CSQ,0, 0);
      //VOICE_Play(NetworkSearching);
      //DISPLAY_Show(d_NoSimCard);
     /* if(GetTaskId()==Task_Start&&task_status_account_config()==FALSE&&ApiAtCmd_CSQValue()<25)//如果处于开机状态、未写入账号状态、网络信号小于25状态
      {
        
        DISPLAY_Show(d_NetworkSearching);
      }

      HDRCSQSignalIcons();*/
    }
/*******收到离线指令，屏幕提示离线状态*******/
    if(TaskDrvobj.Id==TASK_NORMAL&&poccmd_states_poc_status()==OffLine)
    {
      DISPLAY_Show(d_status_offline);
    }
/*******收到离线指令过1分钟未登陆重启*******/
    if(poccmd_states_poc_status()==OffLine)
    {
      DelDrvObj.Count.poc_status_count++;
      
      if(DelDrvObj.Count.poc_status_count>2*60)
      {
        DelDrvObj.Count.poc_status_count=0;
        //main_init();//重启
      }
    }
    else
    {
      DelDrvObj.Count.poc_status_count=0;
    }
/**************/
    if(TaskDrvobj.login_step==6)//收到ZCONSTAT联网指示后延迟2s登录POC
    {
      DelDrvObj.Count.login_step_6_count++;
      if(DelDrvObj.Count.login_step_6_count>2*2)
      {
        TaskDrvobj.login_step=7;
        DelDrvObj.Count.login_step_6_count=0;
      }
    }
    else
    {
      DelDrvObj.Count.login_step_6_count=0;
    }
    
/**登录成功后更新群组信息，若获取不完整则重新获取直到完整**/
    if(poc_first_enter_into_group_flag()==TRUE)
    {
      DelDrvObj.Count.poc_first_enter_into_group_flag_count++;
      if(DelDrvObj.Count.poc_first_enter_into_group_flag_count==1)
      {
        ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
      }
      if(DelDrvObj.Count.poc_first_enter_into_group_flag_count>=2)
      {
        DelDrvObj.Count.poc_first_enter_into_group_flag_count=2;
      }
    }
/********收到0x8a则进入一键报警********/
    if(poc_receive_sos_statas()==TRUE)
    {
      BEEP_SetOutput(BEEP_IDPowerOff,ON,0x00,TRUE);
      ApiPocCmd_ToneStateSet(TRUE);
      AUDIO_IOAFPOW(ON);
      DelDrvObj.Count.receive_sos_statas_count++;
      if(DelDrvObj.Count.receive_sos_statas_count>=2*5)
      {
        Set_RedLed(OFF);
        set_poc_receive_sos_statas(FALSE);
        DelDrvObj.Count.receive_sos_statas_count=0;
        BEEP_SetOutput(BEEP_IDPowerOff,OFF,0x00,TRUE);
        ApiPocCmd_ToneStateSet(FALSE);
        AUDIO_IOAFPOW(OFF);
      }
    }
    else
    {
      DelDrvObj.Count.receive_sos_statas_count=0;
    }
/******喇叭控制相关函数************************/

/****定位成功后3s后在菜单模式下才能查看到经纬度信息（解决刚定位成功查看经纬度异常的问题）*********************************************************/

/******登录状态下的低电报警**********************************************/

/*****进入写频状态5s后将写频标志位清零****************/

/*********初级电量报警30s播报一次********************************/


/********收到0x8a则进入一键报警********/

/*******收到离线指令，屏幕提示离线状态*******/

/*******收到离线指令过1分钟未登陆重启*******/

/****登陆成功一分钟后禁用写频功能、开启外部定位上报模式*********/

/****定位成功后5s上报一次定位*****/

/***********************************************************/
  }
}


static void DEL_1msProcess(void)
{
  //if (DelDrvObj.Msg.Bit.b1ms == DEL_RUN)
  {
    //DelDrvObj.Msg.Bit.b1ms = DEL_IDLE;
    //ApiPocCmd_83_1msRenew();
    ApiPocCmd_10msRenew();
    ApiCaretCmd_10msRenew();
    ApiAtCmd_10msRenew();
    
  }
  return;
}

static void DEL_10msProcess(void)
{
  if (DelDrvObj.Msg.Bit.b10ms == DEL_RUN) 
  {
    keyboard_scan();
    DelDrvObj.Msg.Bit.b10ms = DEL_IDLE;
    //ApGpsCmd_10msRenew();

      DelDrvObj.Count.alarm_count++;
      if(poc_receive_sos_statas()==TRUE)
      {
        if(DelDrvObj.Count.alarm_count>=3)
      {
        //Test_PWM_LED();//报警音30ms进一次
        DelDrvObj.Count.alarm_count = 0;
      }
      }

  }
  return;
}

u8 read_backlight_time_value(void)
{
  return DelDrvObj.BacklightTimeBuf[0];
}
u8 read_key_lock_time_value(void)
{
  return DelDrvObj.KeylockTimeBuf[0];
}