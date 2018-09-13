#include "ALLHead.h"

typedef struct{
  struct{
   bool PersonalKeyMode;
  }status;
}KeyCmdDrv;

static KeyCmdDrv KeyCmdDrvObj;

//将APIKeyCMD修改为按键处理程序
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
      Set_RedLed(ON);
      Delay_100ms(10);
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
      Set_GreenLed(ON);
      Delay_100ms(10);
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

bool KEYCMD_PersonalKeyMode(void)
{
  return KeyCmdDrvObj.status.PersonalKeyMode;
}
void KEYCMD_PersonalKeyModeSet(bool a)
{
  KeyCmdDrvObj.status.PersonalKeyMode=a;
}