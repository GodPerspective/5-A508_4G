#include "AllHead.h"

TaskDrv TaskDrvobj;
const u8 *cTxHardwareid         ="at^hardwareid=2,0";
#if 1
const u8 *cTxTLVL         ="AT+TLVL=7";
const u8 *cTxCLVL         ="AT+CLVL=7";
#else////Ĭ�ϲ���TLVL=7 //CLVL=3
const u8 *cTxTLVL         ="AT+TLVL=7";
const u8 *cTxCLVL         ="AT+TLVL=7";
#endif
u8 Key_PersonalCalling_Flag;

void Task_Init(void)
{
  TaskDrvobj.Id = TASK_LOGIN;
  TaskDrvobj.login_set_account=FALSE;
  TaskDrvobj.battery_states=BATTERY_HEALTH;
  TaskDrvobj.login_step=0;
}

void Task_login_progress(void)
{
  UART3_ToMcuMain();//��ȡдƵ����
  switch(TaskDrvobj.login_step)
  {
  case 0:
    //api_lcd_pwr_on_hint(14,2,GBK,"-0");
    if(AtCmdDrvobj.Msg.Bits.bCommunicationTest==1)//�����ϱ��ظ�
    {
      
      
      //ApiAtCmd_WritCommand(ATCOMM_Test,(u8*)ATCOMM_POCID, strlen((char const*)ATCOMM_POCID));
      //ApiAtCmd_WritCommand(ATCOMM_Test,(u8*)cTxHardwareid, strlen((char const*)cTxHardwareid));
      
      //ApiAtCmd_WritCommand(ATCOMM_ATE1,0,0);//����ǰ���˴����Σ����pocʶ��TXָ����ɸ���
      TaskDrvobj.login_step=1;
    }
    break;
  case 1:
    //api_lcd_pwr_on_hint(14,2,GBK,"-1");
    
    if(AtCmdDrvobj.Msg.Bits.bSimCardIn==1)//�Ѳ忨
    {
      VOICE_Play(ABELL);
      ApiAtCmd_WritCommand(ATCOMM_DIALMODE,0,0);//����Ϊ�ֶ�����
      ApiAtCmd_WritCommand(ATCOMM_CGDCONT_SET,0,0);//����APN
      ApiAtCmd_WritCommand(ATCOMM_CGDCONT_READ,0,0);//��ȡ����APN
      TaskDrvobj.login_step=2;
    }
    break;
  case 2:
    //api_lcd_pwr_on_hint(14,2,GBK,"-2");
    if(AtCmdDrvobj.Msg.Bits.bCGDCONT==1)
    {
      ApiAtCmd_WritCommand(ATCOMM_SetNetworkAuto,0,0);//Ĭ���Զ�ģʽ����
      Delay_100ms(5);
      ApiAtCmd_WritCommand(ATCOMM_POWERUP,0,0);//����׼������
      TaskDrvobj.login_step=3;
    }
    break;
  case 3:
    //api_lcd_pwr_on_hint(14,2,GBK,"-3");
    if(AtCmdDrvobj.network_reg.creg==1||AtCmdDrvobj.network_reg.cereg==1||AtCmdDrvobj.network_reg.cereg==5
     ||AtCmdDrvobj.network_reg.cgreg==1||AtCmdDrvobj.network_reg.cgreg==5)
    {
      ApiAtCmd_WritCommand(ATCOMM_CGACT,0,0);//PDP�����ļ���
      TaskDrvobj.login_step=4;
    }
    break;
  case 4:
    //api_lcd_pwr_on_hint(14,2,GBK,"-4");
    if(AtCmdDrvobj.ZGIPDNS==2)//�յ�ZGIPDNS��ʼ����ָ��
    {
      AtCmdDrvobj.ZGIPDNS=0;
      ApiAtCmd_WritCommand(ATCOMM_ZGACT1,0,0);//PDP�����ļ���
      TaskDrvobj.login_step=5;
    }
    break;
  case 5:
    //api_lcd_pwr_on_hint(14,2,GBK,"-5");
    if(AtCmdDrvobj.ZCONSTAT==2)//�յ�ZCONSTAT:1,1���ʾ������·�ɹ�������������
    {
      AtCmdDrvobj.ZCONSTAT=0;
      TaskDrvobj.login_step=6;
    }
    break;
  case 6:
    //api_lcd_pwr_on_hint(14,2,GBK,"-6");
    break;
  case 7:
    //api_lcd_pwr_on_hint(14,2,GBK,"-7");
    //ApiAtCmd_WritCommand(ATCOMM_ATE1,0,0);//����ǰ���˴����Σ����pocʶ��TXָ����ɸ���
    ApiAtCmd_WritCommand(ATCOMM_Test,(u8*)cTxTLVL, strlen((char const*)cTxTLVL));
    ApiAtCmd_WritCommand(ATCOMM_Test,(u8*)cTxCLVL, strlen((char const*)cTxCLVL));
    ApiPocCmd_WritCommand(PocComm_OpenPOC,0,0);//��POCӦ��
    ApiPocCmd_WritCommand(PocComm_SetParam,0,0);//���õ�¼�˺����롢IP
    ApiPocCmd_WritCommand(PocComm_SetURL,0,0);//����URL
    VOICE_Play(LoggingIn);
    DISPLAY_Show(d_LoggingIn);
    TaskDrvobj.login_step=8;
    break;
  default:
    //api_lcd_pwr_on_hint(14,2,GBK,"-8");
    break;
  }
/********���ƹ�������*************************************/
  if(ApiPocCmd_ReceivedVoicePlayStates()==TRUE)
  {
    AUDIO_IOAFPOW(ON);
  }
  else
  {
    AUDIO_IOAFPOW(OFF);
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
      api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
      api_lcd_pwr_on_hint(0,2,GBK,"                ");
      api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());
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
      api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
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
    voice_tone();
    api_disp_icoid_output( eICO_IDTX, TRUE, TRUE);//�����ź�ͼ��
    if(MenuMode_Flag!=0)
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
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    break;
  case 2://2����סPTT״̬
    //api_lcd_pwr_on_hint(0,2,UNICODE,GetSpeakingUserNameForDisplay());//��ʾ�������ڽ���
    //api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    break;
  case 3://3���ɿ�PTT˲��
    ApiPocCmd_SetKeyPttState(0);
    voice_tone();
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    if(get_current_working_status()==m_group_mode)//���ģʽ
    {
      api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());//��ʾ��ǰȺ��
    }
    else
    {
      DISPLAY_Show(d_individualcall);
    }
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
    api_lcd_pwr_on_hint(0,2,GBK,"                ");
    api_lcd_pwr_on_hint(0,2,UNICODE,GetSpeakingUserNameForDisplay());
    voice_tone();
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
    ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedBeingVoice);

    if(MenuMode_Flag!=0)
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
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
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
    voice_tone();
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//�޷����޽��տ�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedNone);
    break;
  default:
    break;
  }
/********���ƹ�������*************************************/
#if 1
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
#if 1 //MCU Tone��
        if(AtCmdDrvobj.voice_tone_play==TRUE)//���ز���tone��
        {
          AUDIO_IOAFPOW(ON);
        }
        else
        {
          AUDIO_IOAFPOW(OFF);
        }
#else //ģ��Tone��
      if(ApiPocCmd_ToneState()==TRUE)
      {
        AUDIO_IOAFPOW(ON);
      }
      else
      {
        AUDIO_IOAFPOW(OFF);
      }
#endif
  }
}
#endif
}

void Task_low_battery_progress(void)
{
  VOICE_Play(PowerLowPleaseCharge);
  DISPLAY_Show(d_PowerLowPleaseCharge);
  DEL_SetTimer(0,1000);
  while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
  BEEP_Time(10);
}

void Task_PowerOff(void)
{
  api_lcd_pwr_on_hint(0,0,GBK,"                ");
  api_lcd_pwr_on_hint(0,2,GBK,"                ");
  set_power_off(OFF);//�ر�ģ���Դ
  Set_GreenLed(OFF);//�ص�
  Set_RedLed(OFF);
  AUDIO_IOAFPOW(OFF);//�ر�����
  MCU_LCD_BACKLIGTH(OFF);//�رձ����
  asm("HALT");
  while(1)
  {
    
  }
}