#include "ALLHead.h"

typedef struct{
  struct{
   bool PersonalKeyMode;
  }status;
}KeyCmdDrv;

static KeyCmdDrv KeyCmdDrvObj;
static void Key3_PlayVoice(void);


void ApiKeyCmd_PowerOnInitial(void)
{
  KeyCmdDrvObj.status.PersonalKeyMode=FALSE;
}

void key_process(void)
{
/*********按键PTT*****************************************************************************/
    if(get_key_ptt_states()==m_key_press_moment)//PTT按下瞬间
    {
      Set_RedLed(ON);
      ApiPocCmd_WritCommand(PocComm_StartPTT,0,0);
      set_key_ptt_states(m_key_press_state);
    }
    else if(get_key_ptt_states()==m_key_press_state)//PTT处于按下状态
    {
     Set_RedLed(ON);
    }
    else if(get_key_ptt_states()==m_key_loosen_moment)//PTT松开瞬间
    {
      Set_RedLed(OFF);
      ApiPocCmd_WritCommand(PocComm_EndPTT,0,0);
      set_key_ptt_states(m_key_idle);
    }
    else
    {
      
    }
/*********按键Key2*****************************************************************************/
    if(get_key_2_states()==m_key_short_press)//个呼键
    {
      PersonalCallingNum=0;//清空个呼计数位
      KeyCmdDrvObj.status.PersonalKeyMode=TRUE;
      VOICE_Play(PersonalMode);
      DISPLAY_Show(d_PersonalMode);
      ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
      KeyDownUpChoose_GroupOrUser_Flag=2;
      api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//S选择图标
      set_key_2_states(m_key_idle);
    }
    else if(get_key_2_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
    {
      KeyCmdDrvObj.status.PersonalKeyMode=FALSE;
      VOICE_Play(GroupMode);
      //DISPLAY_Show(d_PersonalMode);
      //ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
      KeyDownUpChoose_GroupOrUser_Flag=1;
      api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//S选择图标
      set_key_2_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
/*********按键Key3*****************************************************************************/
    if(get_key_3_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      if(get_current_working_status()==m_personal_mode)//单呼状态按返回键
      {
        DEL_SetTimer(0,40);
        while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
        ApiPocCmd_WritCommand(PocComm_EnterGroup,0,0);
      }
      else
      {
        if(TheMenuLayer_Flag!=0)//解决组呼键影响菜单界面信息显示，现在只要按组呼键就会退出菜单
        {
          MenuDisplay(Menu_RefreshAllIco);
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
        Key3_PlayVoice();
      }
      set_key_3_states(m_key_idle);
    }
    else if(get_key_3_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(10);
      set_key_3_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
/*********按键Key4*****************************************************************************/
    if(get_key_4_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      ApiPocCmd_WritCommand(PocComm_Alarm,0,0);
      set_poc_receive_sos_statas(TRUE);
      set_key_4_states(m_key_idle);
    }
    else if(get_key_4_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
    {
      //报警时长按退出
      set_poc_receive_sos_statas(FALSE);
      BEEP_SetOutput(BEEP_IDPowerOff,OFF,0x00,TRUE);
      ApiPocCmd_ToneStateSet(FALSE);
      AUDIO_IOAFPOW(OFF);  
      set_key_4_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
}

void Key3_PlayVoice(void)
{
  switch(AtCmdDrvobj.Key3Option)
  {
  case Key3_OptionZero://播报本机账号、当前群组、电池电量
    //当前用户：
    api_lcd_pwr_on_hint(0,2,GBK,"                ");
    api_lcd_pwr_on_hint(0,2,UNICODE,GetLocalUserNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetLocalUserNameForVoice(),strlen((char const *)GetLocalUserNameForVoice()));//播报本机用户
    DEL_SetTimer(0,200);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    //当前群组
    api_lcd_pwr_on_hint(0,2,GBK,"                ");
    api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetNowWorkingGroupNameForVoice(),strlen((char const *)GetNowWorkingGroupNameForVoice()));//播报当前用户手机号
    DEL_SetTimer(0,260);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    //电量播报
    KeyBatteryReport();
    break;
  case Key3_OptionOne://播报本机账号、电池电量
    //当前用户：
    api_lcd_pwr_on_hint(0,2,GBK,"                ");
    api_lcd_pwr_on_hint(0,2,UNICODE,GetLocalUserNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetLocalUserNameForVoice(),strlen((char const *)GetLocalUserNameForVoice()));//播报本机用户
    DEL_SetTimer(0,200);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    //当前群组
    api_lcd_pwr_on_hint(0,2,GBK,"                ");//显示当前群组昵称
    api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());
    //电量播报
    KeyBatteryReport();
    break;
  case Key3_OptionTwo://播报本机账号
    //当前用户：
    api_lcd_pwr_on_hint(0,2,GBK,"                ");
    api_lcd_pwr_on_hint(0,2,UNICODE,GetLocalUserNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetLocalUserNameForVoice(),strlen((char const *)GetLocalUserNameForVoice()));//播报本机用户
    DEL_SetTimer(0,200);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    //当前群组
    api_lcd_pwr_on_hint(0,2,GBK,"                ");
    api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());
    break;
  case Key3_OptionThree://播报当前群组
    //当前群组
    api_lcd_pwr_on_hint(0,2,GBK,"                ");
    api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetNowWorkingGroupNameForVoice(),strlen((char const *)GetNowWorkingGroupNameForVoice()));//播报当前用户手机号
    DEL_SetTimer(0,20);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    break;
  case Key3_OptionFour://播报电池电量
    //电量播报
    KeyBatteryReport();
    DEL_SetTimer(0,20);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    break;
  default:
    break;
  }
}

bool KEYCMD_PersonalKeyMode(void)
{
  return KeyCmdDrvObj.status.PersonalKeyMode;
}
void KEYCMD_PersonalKeyModeSet(bool a)
{
  KeyCmdDrvObj.status.PersonalKeyMode=a;
}