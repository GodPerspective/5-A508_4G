#include "AllHead.h"

TaskDrv TaskDrvobj;

u8 Key_PersonalCalling_Flag;

void Task_Init(void)
{
  TaskDrvobj.Id = TASK_LOGIN;
  TaskDrvobj.login_set_account=FALSE;
}

void Task_login_progress(void)
{
  UART3_ToMcuMain();//��ȡдƵ����
  switch(TaskDrvobj.login_step)
  {
  case 0:
    if(AtCmdDrvobj.Msg.Bits.bCommunicationTest==1)//�����ϱ��ظ�
    {
      ApiAtCmd_WritCommand(ATCOMM_ATE1,0,0);
      TaskDrvobj.login_step=1;
    }
    break;
  case 1:
    if(AtCmdDrvobj.Msg.Bits.bSimCardIn==1)//�Ѳ忨
    {
      ApiAtCmd_WritCommand(ATCOMM_DIALMODE,0,0);//����Ϊ�ֶ�����
      ApiAtCmd_WritCommand(ATCOMM_CGDCONT,0,0);//����APN����δ���ã�ֻ�Ƕ�ȡ��
      TaskDrvobj.login_step=2;
    }
    break;
  case 2:
    if(AtCmdDrvobj.Msg.Bits.bCGDCONT==1)
    {
      ApiAtCmd_WritCommand(ATCOMM_POWERUP,0,0);//����׼������
      TaskDrvobj.login_step=3;
    }
    break;
  case 3:
    if(AtCmdDrvobj.network_reg.cgreg==1||AtCmdDrvobj.network_reg.cereg==1)
    {
      ApiAtCmd_WritCommand(ATCOMM_CGACT,0,0);//PDP�����ļ���
      TaskDrvobj.login_step=4;
    }
    break;
  case 4:
    if(AtCmdDrvobj.Msg.Bits.bZGIPDNS==1)//�յ�ZGIPDNS��ʼ����ָ��
    {
      ApiAtCmd_WritCommand(ATCOMM_ZGACT,0,0);//PDP�����ļ���
      TaskDrvobj.login_step=5;
    }
    break;
  case 5:
    if(AtCmdDrvobj.Msg.Bits.bZCONSTAT==1)//�յ�ZCONSTAT:1,1���ʾ������·�ɹ�������������
    {
      TaskDrvobj.login_step=6;
    }
    break;
  case 6:
    break;
  case 7:
    ApiPocCmd_WritCommand(PocComm_OpenPOC,0,0);//��POCӦ��
    //ApiPocCmd_WritCommand(PocComm_SetParam,0,0);//���õ�¼�˺����롢IP
    //ApiPocCmd_WritCommand(PocComm_SetURL,0,0);//����URL
    TaskDrvobj.login_step=8;
    break;
  default:
    break;
  }
}



void Task_normal_progress(void)
{
  UART3_ToMcuMain();//��ȡдƵ����
  keyboard_process();
  key_process();

/***********�ж��������飻�����˳���;������ģʽ���˳�����ģʽ��������ʼ�����������������˳�����*************/
  if(get_current_working_status()==m_group_mode)//���ģʽ
  {
    if(ApiPocCmd_GroupStates()==EnterGroup)
    {
      api_lcd_pwr_on_hint(0,2,"                ");
      api_lcd_pwr_on_hint4(GetNowWorkingGroupNameForDisplay());
      ApiPocCmd_GroupStatesSet(InGroup);
    }
    else if(ApiPocCmd_GroupStates()==InGroup)
    {
    }
    else//LeaveGroup
    {
    }
  }
  else //����ģʽ
  {
    if(ApiPocCmd_GroupStates()==EnterGroup)
    {
      DISPLAY_Show(d_individualcall);
      ApiPocCmd_GroupStatesSet(InGroup);
    }
    else if(ApiPocCmd_GroupStates()==InGroup)
    {
    }
    else//LeaveGroup
    {
    }
  }
  
/*********��PTT�������ʱ����ʾ����ʾ****************************************************************************************************************************/
  switch(ApiPocCmd_KeyPttState())//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
  {
  case 0://δ��PTT
    break;
  case 1://1:����ptt˲��
    ApiPocCmd_SetKeyPttState(2);
    api_disp_icoid_output( eICO_IDTX, TRUE, TRUE);//�����ź�ͼ��
    api_lcd_pwr_on_hint(0,2,"                ");
    //api_lcd_pwr_on_hint4(GetSpeakingUserNameForDisplay());//��ʾ�������ڽ���
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    break;
  case 2://2����סPTT״̬
    api_lcd_pwr_on_hint4(GetSpeakingUserNameForDisplay());//��ʾ�������ڽ���
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    break;
  case 3://3���ɿ�PTT˲��
    ApiPocCmd_SetKeyPttState(0);
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetNowWorkingGroupNameForDisplay());//��ʾ��ǰȺ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    break;
  default:
    break;
  }
  
  switch(ApiPocCmd_ReceivedVoicePlayStatesForDisplay())
  {
  case ReceivedNone:
    break;
  case ReceivedStartVoice:
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetSpeakingUserNameForDisplay());
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedBeingVoice);

#if 0 //�������򻻺�״̬�£������к�PTT��OK�����л�Ⱥ�飬�����ǻظ��ո�˵���˵�����
    if(MenuMode_Flag!=0)
    {
      MenuDisplay(Menu_RefreshAllIco);
      MenuMode_Flag = 0;
    }
    KeyDownUpChoose_GroupOrUser_Flag=0;
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//Sѡ���Ӧ��ͼ��
    KeyUpDownCount=0;
#endif
    break;
  case ReceivedBeingVoice:
    break;
  case ReceivedEndVoice:
    get_screen_display_group_name();
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//�޷����޽��տ�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedNone);
    break;
  default:
    break;
  }
/********���ƹ�������*************************************/
#if 0
if(ApiPocCmd_ReceivedVoicePlayStates()==TRUE)
{
  AUDIO_IOAFPOW(ON);
}
else
{
  if(poc_receive_sos_statas()==TRUE)
  {
     AUDIO_IOAFPOW(ON);
  }
  else
  {
    if(ApiAtCmd_bZTTSStates()==1)//���������ʱ���쳣�ر����ȣ������������ٵ�����
    {
      AUDIO_IOAFPOW(ON);
    }
    else
    {
      if(ApiPocCmd_ToneState()==TRUE)
      {
        AUDIO_IOAFPOW(ON);
      }
      else
      {
        AUDIO_IOAFPOW(OFF);
      }
    }
  }

}
#endif
}

void Task_low_battery_progress(void)
{
  UART3_ToMcuMain();//��ȡдƵ����
}

void Task_write_freq_progress(void)
{
  UART3_ToMcuMain();//��ȡдƵ����
  
}