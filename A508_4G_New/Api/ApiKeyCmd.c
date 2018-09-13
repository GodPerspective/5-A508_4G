#include "ALLHead.h"

typedef struct{
  struct{
   bool PersonalKeyMode;
  }status;
}KeyCmdDrv;

static KeyCmdDrv KeyCmdDrvObj;

//��APIKeyCMD�޸�Ϊ�����������
void key_process(void)
{
/*********����PTT*****************************************************************************/
    if(get_key_ptt_states()==m_key_press_moment)//PTT����˲��
    {
      Set_RedLed(ON);
      ApiPocCmd_WritCommand(PocComm_StartPTT,0,0);
      set_key_ptt_states(m_key_press_state);
    }
    else if(get_key_ptt_states()==m_key_press_state)//PTT���ڰ���״̬
    {
     Set_RedLed(ON);
    }
    else if(get_key_ptt_states()==m_key_loosen_moment)//PTT�ɿ�˲��
    {
      Set_RedLed(OFF);
      ApiPocCmd_WritCommand(PocComm_EndPTT,0,0);
      set_key_ptt_states(m_key_idle);
    }
    else
    {
      
    }
/*********����Key2*****************************************************************************/
    if(get_key_2_states()==m_key_short_press)//������
    {
      KeyCmdDrvObj.status.PersonalKeyMode=TRUE;
      VOICE_Play(PersonalMode);
      DISPLAY_Show(d_PersonalMode);
      ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
      KeyDownUpChoose_GroupOrUser_Flag=2;
      api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//Sѡ��ͼ��
      set_key_2_states(m_key_idle);
    }
    else if(get_key_2_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
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
/*********����Key3*****************************************************************************/
    if(get_key_3_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(10);
      set_key_3_states(m_key_idle);
    }
    else if(get_key_3_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
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
/*********����Key4*****************************************************************************/
    if(get_key_4_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      ApiPocCmd_WritCommand(PocComm_Alarm,0,0);
      set_poc_receive_sos_statas(TRUE);
      set_key_4_states(m_key_idle);
    }
    else if(get_key_4_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      //����ʱ�����˳�
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