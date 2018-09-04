#include "AllHead.h"

typedef struct{
  struct{
    bool CommunicationTest;
    bool SimCard;
    bool PersonalKeyMode;
  }status;
}TaskDrv;
static TaskDrv TaskDrvobj;

bool EnterPttMoment_Flag=FALSE;
bool LoosenPttMoment_Flag=FALSE;
u8 EnterPttMomentCount=0;
u8 LoosenPttMomentCount=0;

void Key3_PlayVoice(void);

void Task_Init(void)
{
  TaskDrvobj.status.AccountConfig = FALSE;
  TaskDrvobj.status.BootPrompt    = FALSE;
  TaskDrvobj.status.PersonalKeyMode = FALSE;
  EnterPttMoment_Flag = FALSE;
  LoosenPttMoment_Flag = FALSE;
  EnterPttMomentCount = 0;
  LoosenPttMomentCount = 0;
}


void Task_LoginMode(void)
{
  UART3_ToMcuMain();
  if()
    
  if(ApiAtCmd_bStartingUp()==1)//�����ɹ�
  {
    if(TaskDrvobj.status.BootPrompt==FALSE)
    {
      VOICE_Play(ABELL);
      TaskDrvobj.status.BootPrompt=TRUE;
    }
    if(ApiAtCmd_bCardIn()==1)//��⵽��
    {
      if(ApiAtCmd_CSQValue()>=25)
      {
        if(ApiAtCmd_bPPPStatusOpen()==1)
        {
          if(TaskDrvobj.status.AccountConfig==FALSE)
          {
            ApiAtCmd_WritCommand(ATCOMM_Test,ucSetLanguage,strlen((char const *)ucSetLanguage));//��������
            ApiAtCmd_WritCommand(ATCOMM_Test,ucCODECCTL,strlen((char const *)ucCODECCTL));//������������
            ApiAtCmd_WritCommand(ATCOMM_Test,(u8 *)ucRXFILTER,strlen((char const *)ucRXFILTER));//���ӳ�����T1-ͦ�� ��Х��
            TaskDrvobj.status.AccountConfig=TRUE;
            ApiPocCmd_WritCommand(PocComm_OpenPOC,0,0);//��POCӦ��
            ApiPocCmd_WritCommand(PocComm_SetParam,0,0);//���õ�¼�˺����롢IP
            ApiPocCmd_WritCommand(PocComm_SetURL,0,0);//����URL
            VOICE_Play(LoggingIn);
            DISPLAY_Show(d_LoggingIn);
            
          }
        }
      }
    }
    else
    {
      if(ApiAtCmd_bNoCard()==1)//δ��⵽��
      {
      }
    }
  }
}

#if 1//WCDMA ׿�Ǵ�
void Task_NormalMode(void)
{
  Keyboard_Test();
  UART3_ToMcuMain();
//���дƵʱ��Ӱ����������ʹ�ã������������ڽ���״̬��
  if(WriteFreq_Flag==TRUE)//���дƵʱ��Ⱥ������������һֱ�еεεε�����
  {
    ApiPocCmd_WritCommand(PocComm_EndPTT,0,0);
  }
/***********PTT״̬���*************************************************************************************************************************/
  if(ReadInput_KEY_PTT==0)//�жϰ���PTT��˲��
  {
    EnterPttMomentCount++;
    if(EnterPttMomentCount==1)
      EnterPttMoment_Flag=TRUE;
    else
    {
      EnterPttMoment_Flag=FALSE;
      EnterPttMomentCount=3;
    }
    LoosenPttMoment_Flag=FALSE;
    LoosenPttMomentCount=0;
  }
  else
  {
    EnterPttMomentCount=0;
    EnterPttMoment_Flag=FALSE;
    LoosenPttMomentCount++;
    if(LoosenPttMomentCount==1)
      LoosenPttMoment_Flag=TRUE;
    else
    {
      LoosenPttMoment_Flag=FALSE;
      LoosenPttMomentCount=3;
    }
  }

  switch(ApiPocCmd_KeyPttState())//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
  {
  case 0://δ��PTT
        if(WriteFreq_Flag==FALSE)//���дƵʱ��Ⱥ������������һֱ�еεεε�����
        {
          if(EnterPttMoment_Flag==TRUE)
          {
            ApiPocCmd_WritCommand(PocComm_StartPTT,0,0);
          }
        }
        break;
  case 1://1:����ptt˲��
    ApiPocCmd_SetKeyPttState(2);
    if(LoosenPttMoment_Flag==TRUE)//����ɿ�PTT˲�䣬����endPTTָ��
    {
      ApiPocCmd_WritCommand(PocComm_EndPTT,0,0);
      //Set_RedLed(LED_OFF);
    }
    break;
  case 2://2����סPTT״̬
    Set_RedLed(LED_ON);
    Set_GreenLed(LED_OFF);
    if(TheMenuLayer_Flag!=0)//�������ʱӰ��˵�������Ϣ��ʾ������ֻҪ��PTT�ͻ��˳��˵�
    {
        MenuDisplay(Menu_RefreshAllIco);
        api_lcd_pwr_on_hint(0,2,"                ");//����
        //api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
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
    api_disp_icoid_output( eICO_IDTX, TRUE, TRUE);//�����ź�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      
    if(LoosenPttMoment_Flag==TRUE)//����ɿ�PTT˲�䣬����endPTTָ��
    {
      ApiPocCmd_WritCommand(PocComm_EndPTT,0,0);
      Set_RedLed(LED_OFF);
    }
    break;
  case 3://3���ɿ�PTT˲��
    ApiPocCmd_SetKeyPttState(0);
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    if(EnterPttMoment_Flag==TRUE)
    {
      ApiPocCmd_WritCommand(PocComm_StartPTT,0,0);
    }
    break;
  default:
    break;
  }
  if(ReadInput_KEY_PTT==0)
  {
    switch(KeyDownUpChoose_GroupOrUser_Flag)
    {
    case 0://Ĭ��PTT״̬
      break;
    case 1://=1������ĳȺ��
      VOICE_Play(GroupSelected);
      DEL_SetTimer(0,40);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      ApiPocCmd_WritCommand(PocComm_EnterGroup,0,0);
      KeyDownUpChoose_GroupOrUser_Flag=0;
      api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//Sѡ���Ӧ��ͼ��
      KeyUpDownCount=0;
      break;
    case 2://=2,����ĳ�û�
      VOICE_Play(GroupSelected);
      DEL_SetTimer(0,40);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      ApiPocCmd_WritCommand(PocComm_Invite,0,0);
      KeyDownUpChoose_GroupOrUser_Flag=0;
      api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//Sѡ���Ӧ��ͼ��
      KeyPersonalCallingCount=0;
      break;
    case 3:
      break;
    case 4:
      break;
    default:
      break;
    }
  }
  else
  {
    //Set_RedLed(LED_OFF);

  }
/*******�����״̬���***********************************************************************************************************************************/
#if 1//WCDMA ׿�Ǵ�
  if(ReadInput_KEY_3==0)//�����
  {
    if(get_current_working_status()==m_personal_mode)//����״̬�����ؼ�
    {
      DEL_SetTimer(0,40);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      ApiPocCmd_WritCommand(PocComm_EnterGroup,0,0);
    }
    else
    {
      if(TheMenuLayer_Flag!=0)//��������Ӱ��˵�������Ϣ��ʾ������ֻҪ��������ͻ��˳��˵�
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
  }
#endif
/*******������״̬���***************************************************************************************************************************************/
  if(ReadInput_KEY_2==0)//������
  {
    TaskDrvobj.status.PersonalKeyMode=TRUE;
    VOICE_Play(PersonalMode);
    DISPLAY_Show(d_PersonalMode);
    DEL_SetTimer(0,120);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
    KeyDownUpChoose_GroupOrUser_Flag=2;
    api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//Sѡ��ͼ��
  }
/*******������״̬���********************************************************************************************************************************************/
  if(ReadInput_KEY_4==0)//������
  {
    switch(key_top_option)
    {
    case REMOTE_AND_LOCAL_ALARM:
      ApiPocCmd_WritCommand(PocComm_Alarm,0,0);
      set_poc_receive_sos_statas(TRUE);
      break;
    case REMOTE_ALARM_ONLY:
      ApiPocCmd_WritCommand(PocComm_Alarm,0,0);
      break;
    case LOCAL_ALARM_ONLY:
      set_poc_receive_sos_statas(TRUE);
      break;
    default:
      break;
    }
    DEL_SetTimer(0,100);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
  }
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
/*********�жϷ������ͼ��״̬****************************************************************************************************************************/
#if 1//WCDMA
  switch(ApiPocCmd_ReceivedVoicePlayStatesForDisplay())
  {
  case ReceivedVoiceNone:
    break;
  case ReceivedVoiceStart:
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetSpeakingUserNameForDisplay());
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedVoiceBeing);
#if 1 //�������򻻺�״̬�£������к�PTT��OK�����л�Ⱥ�飬�����ǻظ��ո�˵���˵�����
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
  case ReceivedVoiceBeing:
    break;
  case ReceivedVoiceEnd:
    get_screen_display_group_name();
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//�޷����޽��տ�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedVoiceNone);
    break;
  default:
    break;
  }
#else
  if(PersonCallIco_Flag==1)
{
  if(POC_ReceivedVoiceStart_Flag==2)//�ս�������״̬
  {
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//�����˵��ڱ�����״̬��δˢ�µ�BUG
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    POC_ReceivedVoiceStart_Flag=1;//��������״̬
  }
  else//0����״̬��1����״̬
  {
    if(POC_ReceivedVoiceEnd_Flag==2)//����״̬
    {
      api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      POC_ReceivedVoiceEnd_Flag=0;//Ĭ��������״̬
    }
    else//����״̬
    {}
  }
}
else
{
  if(POC_ReceivedVoiceStart_Flag==2)//�ս�������״̬
  {
    api_lcd_pwr_on_hint("                ");//����
    POC_ReceivedVoiceStart_Flag=1;//��������״̬
    KeyDownUpChoose_GroupOrUser_Flag=0;
    KeyUpDownCount=0;
    //�޸�BUG���ڲ˵����棬B����A������ʾ����ʾ���ҵ����⣨��Ϊ����A���˳��˵���
    if(TheMenuLayer_Flag!=0)
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
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//�����˵��ڱ�����״̬��δˢ�µ�BUG
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��

  }
  else if(POC_ReceivedVoiceStart_Flag==1)//0����״̬��1����״̬//���Խ����������
  {
    if(POC_ReceivedVoiceEnd_Flag==2)//����״̬
    {
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    api_lcd_pwr_on_hint("                ");//����
    //api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�--3
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    POC_ReceivedVoiceStart_Flag=0;//�����յ�ff�����㣬���յ�endFLAG���������
    POC_ReceivedVoiceEnd_Flag=0;//Ĭ��������״̬
    Key_PersonalCalling_Flag=0;//��������������󣬰����¼���Ȼ���л�������Ա
    }
    else//����״̬
    {}
  }
  else
  {
  }
}
#endif
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

/*****���û�����߳�Ա******************************************/
}
#else //CDMA ����
void Task_RunNormalOperation(void)
{
  if(POC_GetAllGroupNameDone_Flag==TRUE)
  {
    Keyboard_Test();
  }
  UART3_ToMcuMain();
/***********PTT״̬���*************************************************************************************************************************/
  if(ReadInput_KEY_PTT==0)//�жϰ���PTT��˲��
  {
    EnterPttMomentCount++;
    if(EnterPttMomentCount==1)
      EnterPttMoment_Flag=TRUE;
    else
    {
      EnterPttMoment_Flag=FALSE;
      EnterPttMomentCount=3;
    }
    LoosenPttMoment_Flag=FALSE;
    LoosenPttMomentCount=0;
  }
  else
  {
    EnterPttMomentCount=0;
    EnterPttMoment_Flag=FALSE;
    LoosenPttMomentCount++;
    if(LoosenPttMomentCount==1)
      LoosenPttMoment_Flag=TRUE;
    else
    {
      LoosenPttMoment_Flag=FALSE;
      LoosenPttMomentCount=3;
    }
  }
//���дƵʱ��Ӱ����������ʹ�ã������������ڽ���״̬��
  if(WriteFreq_Flag==TRUE)//���дƵʱ��Ⱥ������������һֱ�еεεε�����
  {
    ApiPocCmd_WritCommand(PocComm_EndPTT,0,0);
  }
//������������ǣ���סPTT������ѭ���ղ���ָ�������
  
  if(KeyDownUpChoose_GroupOrUser_Flag==3)
  {KeyDownUpChoose_GroupOrUser_Flag=0;}

  switch(KeyPttState)//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
  {
  case 0://δ��PTT
    POC_ReceivedVoiceEndForXTSF_Flag=0;
#if 1
    if(KeyDownUpChoose_GroupOrUser_Flag==0)
    {
      //if(POC_ReceivedVoice_Flag==FALSE)
      {
        if(WriteFreq_Flag==FALSE)//���дƵʱ��Ⱥ������������һֱ�еεεε�����
        {
          if(EnterPttMoment_Flag==TRUE)
          {
            ApiPocCmd_WritCommand(PocComm_StartPTT,0,0);
          }
        }
      }
    }
#else 
    if(KeyDownUpChoose_GroupOrUser_Flag==0)
    {
      if(POC_ReceivedVoice_Flag==TRUE)//����Է�˵��ʱ��PTT���������쳣������
      {
        if(EnterPttMoment_Flag==TRUE)
        {
          VOICE_SetOutput(ATVOICE_FreePlay,"8179d153",8);//����
        }
      }
      else
      {
        if(WriteFreq_Flag==FALSE)//���дƵʱ��Ⱥ������������һֱ�еεεε�����
        {
          if(EnterPttMoment_Flag==TRUE)
          {
            ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
          }
        }
      }
    }
#endif
    break;
  case 1://1:����ptt˲��
    KeyPttState=2;
    if(LoosenPttMoment_Flag==TRUE)//����ɿ�PTT˲�䣬����endPTTָ��
    {
      ApiPocCmd_WritCommand(PocComm_EndPTT,0,0);
      Set_RedLed(LED_OFF);
    }
    break;
  case 2://2����סPTT״̬
    if(POC_ReceivedVoice_Flag==TRUE)//������ڽ�������
    {
    }
    else
    {
      //�������ʱ�䵽ʱ��������ϵͳ�ͷš���������ֹͣ���䡣����ʾ��������Ų���ʧ����ɫָʾ�Ʋ�Ϩ��
      if(POC_ReceivedVoiceEndForXTSF_Flag==2)
      {
        Set_RedLed(LED_OFF);
        KeyDownUpChoose_GroupOrUser_Flag=3;
      }
      else
      {
        Set_RedLed(LED_ON);
        Set_GreenLed(LED_OFF);
      if(TheMenuLayer_Flag!=0)//�������ʱӰ��˵�������Ϣ��ʾ������ֻҪ��PTT�ͻ��˳��˵�
      {
        MenuDisplay(Menu_RefreshAllIco);
        api_lcd_pwr_on_hint("                ");//����
        //api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
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
      api_disp_icoid_output( eICO_IDTX, TRUE, TRUE);//�����ź�ͼ��
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      }
    }
    if(LoosenPttMoment_Flag==TRUE)//����ɿ�PTT˲�䣬����endPTTָ��
    {
      ApiPocCmd_WritCommand(PocComm_EndPTT,0,0);
      
      Set_RedLed(LED_OFF);
    }
    break;
  case 3://3���ɿ�PTT˲��
    POC_ReceivedVoiceEndForXTSF_Flag=0;
    KeyPttState=0;
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
#if 1//������ٰ�����PTT�쳣������
    if(EnterPttMoment_Flag==TRUE)
    {
      ApiPocCmd_WritCommand(PocComm_StartPTT,0,0);
    }
#endif
    break;
  default:
    break;
  }
  
  if(ReadInput_KEY_PTT==0)
  {
    switch(KeyDownUpChoose_GroupOrUser_Flag)
    {
    case 0://Ĭ��PTT״̬
      break;
    case 1://=1������ĳȺ��
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
      DEL_SetTimer(0,40);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      //ApiPocCmd_WritCommand(PocComm_EnterGroup,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      KeyDownUpChoose_GroupOrUser_Flag=3;
      EnterKeyTimeCount=0;
      KeyUpDownCount=0;
      break;
    case 2://=2,����ĳ�û�
      if(GettheOnlineMembersDone==TRUE)
      {
        //GettheOnlineMembersDone=FALSE;
        VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//������ѡ��
        DEL_SetTimer(0,60);
        while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
        //ApiPocCmd_WritCommand(PocComm_Invite,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
        KeyDownUpChoose_GroupOrUser_Flag=3;
        TASK_Ptt_StartPersonCalling_Flag=TRUE;//�ж���������״̬��0a��
        EnterKeyTimeCount=0;
      }
      break;
    case 3:
      break;
    case 4:
      break;
    default:
      break;
    }
  }
  else
  {
    //Set_RedLed(LED_OFF);

  }
/*******�����״̬���***********************************************************************************************************************************/
#if 0//WCDMA ׿�Ǵ�
if(ReadInput_KEY_3==0)//�����
  {
    if(GetPersonalCallingMode()==1)//����ǵ���ģʽ�����˳�����
    {
      api_lcd_pwr_on_hint("    �˳�����    ");
      Delay_100ms(5);
      ApiPocCmd_WritCommand(PocComm_Cancel,(u8 *)ucQuitPersonalCalling,strlen((char const *)ucQuitPersonalCalling));
      api_lcd_pwr_on_hint("Ⱥ��:   ���ģʽ");
    }
    else
    {
      if(TheMenuLayer_Flag!=0)//��������Ӱ��˵�������Ϣ��ʾ������ֻҪ��������ͻ��˳��˵�
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
  }
#endif
/*******������״̬���***************************************************************************************************************************************/
  if(ReadInput_KEY_2==0)//������
  {
    if(POC_EnterPersonalCalling_Flag==1)//�������״̬�£�����������Ч��Ӧ���Ǳ���״̬�£��ø�����ʧЧ��
    {
      VOICE_SetOutput(ATVOICE_FreePlay,"ab887c542d4e",12);//������
      DEL_SetTimer(0,50);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    }
    else
    {
      //GettheOnlineMembersDone=FALSE;//����������������¼��߼��������⣬����������ֱ��������һ����Ա��ſ��԰����¼��л�������Ա
      if(TheMenuLayer_Flag!=0)//���������Ӱ��˵�������Ϣ��ʾ������ֻҪ���������ͻ��˳��˵�
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
      api_lcd_pwr_on_hint("  ������Աѡ��  ");
      PersonalCallingNum=0;//�����������ֱ��ѡ�У������û������ǲ������û�
      Key_PersonalCalling_Flag=1;
      VOICE_SetOutput(ATVOICE_FreePlay,"2a4e7c542000106258540990e962",28);//������Աѡ��
      DEL_SetTimer(0,150);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      //VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(0),ApiAtCmd_GetUserNameLen(0));//�״λ�ȡ���ڳ�Ա������һ����Ա
      api_lcd_pwr_on_hint4("                ");//����
      //api_lcd_pwr_on_hint4(UnicodeForGbk_AllUserName(0));//��ʾ��ǰѡ�е�Ⱥ����
      ApiPocCmd_WritCommand(PocComm_UserListInfo,"0E000000000001",strlen((char const *)"0E000000000001"));
      KeyDownUpChoose_GroupOrUser_Flag=2;
      KeyPersonalCallingCount=0;//�������ģʽ�����¼���Ա������˳�򣬵�һ����Ա���л�ʱ��ڶ�������������
    }
  }
/*******������״̬���********************************************************************************************************************************************/
  if(ReadInput_KEY_4==0)//������
  {
  }
  
/***********�ж��������飻�����˳���;������ģʽ���˳�����ģʽ��������ʼ�����������������˳�����*************/
  if(POC_EnterPersonalCalling_Flag==2)//1�ձ���
  {
    MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint("                ");//����
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
    //api_lcd_pwr_on_hint4(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
    api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
    PersonCallIco_Flag=1;
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    POC_EnterPersonalCalling_Flag=1;//�ڵ���ģʽ
    POC_EnterGroupCalling_Flag=1;
  }
  else if(POC_EnterPersonalCalling_Flag==1)//2����״̬
  {
    //api_lcd_pwr_on_hint("   2����״̬     ");
  }
  else//���顢���ڡ����顢��ʼ�����������С���������
  {
    if(POC_EnterGroupCalling_Flag==2)//1����
    {
      if(POC_AtEnterPersonalCalling_Flag==2)//������ʼ����ģʽ
      {
      }
      else if(POC_AtEnterPersonalCalling_Flag==1)//������
      {
      }
      else
      {
        MenuDisplay(Menu_RefreshAllIco);
        api_lcd_pwr_on_hint("                ");//����
        api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
        api_disp_icoid_output( BatteryLevel, TRUE, TRUE);
        api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
//        api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�--2
        PersonCallIco_Flag=0;
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��//���ܻ��POC����PoCָ��ʶ����Ӱ��
      }
      POC_EnterGroupCalling_Flag=1;//��������
    }
    else if(POC_EnterGroupCalling_Flag==1)//2����
    {
      if(POC_AtEnterPersonalCalling_Flag==2)//1������
      {
        MenuDisplay(Menu_RefreshAllIco);
        api_lcd_pwr_on_hint("                ");//����
        api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
        api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
        //api_lcd_pwr_on_hint4(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
        PersonCallIco_Flag=1;
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��//���ܻ��POC����PoCָ��ʶ����Ӱ��
        POC_AtEnterPersonalCalling_Flag=1;
      }
      else if(POC_AtEnterPersonalCalling_Flag==1)//2������
      {
      }
      else//3��������
      {
      }
    }
    else//����
    {
      if(POC_QuitGroupCalling_Flag==2)
      {
        if(POC_QuitPersonalCalling_Flag==2)//����ģʽ����
        {
          POC_QuitPersonalCalling_Flag=0;
          Key_PersonalCalling_Flag=0;
        }
        if(POC_AtQuitPersonalCalling_Flag==2)//����ģʽ����
        {
          POC_AtQuitPersonalCalling_Flag=0;
          Key_PersonalCalling_Flag=0;
        }
        
        if(TASK_Ptt_StartPersonCalling_Flag==TRUE)//����л�����,��PTTȷ�ϣ���������ģʽʱ���м䲻Ӧ��ʾһ�������Ϣ������ʾ����
        {
          //api_lcd_pwr_on_hint("   ����BUG     ");
        }
        else
        {
          api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
          PersonCallIco_Flag=0;
        }
        POC_QuitGroupCalling_Flag=1;
      }
    }

  }
/*********�жϷ������ͼ��״̬****************************************************************************************************************************/
if(PersonCallIco_Flag==1)
{
  if(POC_ReceivedVoiceStart_Flag==2)//�ս�������״̬
  {
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//�����˵��ڱ�����״̬��δˢ�µ�BUG
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    POC_ReceivedVoiceStart_Flag=1;//��������״̬
  }
  else//0����״̬��1����״̬
  {
    if(POC_ReceivedVoiceEnd_Flag==2)//����״̬
    {
      api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
      api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      POC_ReceivedVoiceEnd_Flag=0;//Ĭ��������״̬
    }
    else//����״̬
    {}
  }
}
else
{
  if(POC_ReceivedVoiceStart_Flag==2)//�ս�������״̬
  {
    api_lcd_pwr_on_hint("                ");//����
    //api_lcd_pwr_on_hint4(UnicodeForGbk_SpeakerRightnowName());//��ʾ��ǰ˵���˵��ǳ�
    //api_lcd_pwr_on_hint4("1234567890123");//��ʾ��ǰ˵���˵��ǳ�
    POC_ReceivedVoiceStart_Flag=1;//��������״̬
    //�޸�BUG�� A������״̬��B����A����A����PTTȴ�ǻ��飨������A��Ӧ�÷���Ĭ��״̬����
    KeyDownUpChoose_GroupOrUser_Flag=0;
    KeyUpDownCount=0;
    //�޸�BUG���ڲ˵����棬B����A������ʾ����ʾ���ҵ����⣨��Ϊ����A���˳��˵���
    if(TheMenuLayer_Flag!=0)
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
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//�����˵��ڱ�����״̬��δˢ�µ�BUG
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��

  }
  else if(POC_ReceivedVoiceStart_Flag==1)//0����״̬��1����״̬//���Խ����������
  {
    if(POC_ReceivedVoiceEnd_Flag==2)//����״̬
    {
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
    api_lcd_pwr_on_hint("                ");//����
    //api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�--3
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
    POC_ReceivedVoiceStart_Flag=0;//�����յ�ff�����㣬���յ�endFLAG���������
    POC_ReceivedVoiceEnd_Flag=0;//Ĭ��������״̬
    Key_PersonalCalling_Flag=0;//��������������󣬰����¼���Ȼ���л�������Ա
    }
    else//����״̬
    {}
  }
  else
  {
  }
}

/********���ƹ�������*************************************/
#if 1
if(ApiPocCmd_PlayReceivedVoice_Flag==TRUE)
{
  AUDIO_IOAFPOW(ON);
}
else
{
  if(ApiAtCmd_ZTTS_Flag==TRUE)
  {
    AUDIO_IOAFPOW(ON);
  }
  else
  {
    if(ApiPocCmd_Tone_Flag==TRUE)
    {
      AUDIO_IOAFPOW(ON);
    }
    else
    {
      AUDIO_IOAFPOW(OFF);
    }
  }
}

#else
/*if(KeyPttState==2)//��ʱ�����ptt�����Ŵ򿪵�����
{
  AUDIO_IOAFPOW(OFF);
}
else*/
{
  if(ApiPocCmd_Tone_Flag==TRUE)//8b0003 �����PTT����ʾ��������
  {
    AUDIO_IOAFPOW(ON);
  }
  else
  {
    if(UpDownSwitching_Flag==TRUE)//�����¼����黻��״̬
    {
      AUDIO_IOAFPOW(ON);
    }
    else
    {
      if(ApiAtCmd_TrumpetVoicePlay_Flag==1)
      {
        AUDIO_IOAFPOW(ON);//��VOICE_SetOutput()���˴򿪣���ʶ��POC:91���˹��Ŵ�;PTT��
      }
      else if(ApiAtCmd_TrumpetVoicePlay_Flag==2)
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

/*****���û�����߳�Ա******************************************/
if(PocNoOnlineMember_Flag2==TRUE)
{
  PocNoOnlineMember_Flag2=FALSE;
  MenuMode_Flag=0;
  api_lcd_pwr_on_hint("                ");//����
  //api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
  //����PTT�������¼�����Ĭ��״̬
  KeyDownUpChoose_GroupOrUser_Flag=0;
  KeyUpDownCount=0;
  Key_PersonalCalling_Flag=0;//���������־λ
  KeyDownUpChoose_GroupOrUser_Flag=0;//����������������ؼ���OK��PTT����Ļ��ʾ�����BUG
}
}
#endif

void TASK_WriteFreq(void)
{
  UART3_ToMcuMain();
}
void TASK_RunLoBattery(void)
{
#if 1
  VOICE_Play(PowerLowPleaseCharge);
  DISPLAY_Show(d_PowerLowPleaseCharge);
  DEL_SetTimer(0,1000);
  while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
  BEEP_Time(10);
#else
  ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)"at+GPSFUNC=0",strlen((char const *)"at+GPSFUNC=0"));//
  ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)"at+pwroff",strlen((char const *)"at+pwroff"));//
#endif
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

void Key3_PlayVoice(void)
{
  switch(Key3Option)
  {
  case Key3_OptionZero://���������˺š���ǰȺ�顢��ص���
    //��ǰ�û���
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetLocalUserNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetLocalUserNameForVoice(),strlen((char const *)GetLocalUserNameForVoice()));//���������û�
    DEL_SetTimer(0,100);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    //��ǰȺ��
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetNowWorkingGroupNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetNowWorkingGroupNameForVoice(),strlen((char const *)GetNowWorkingGroupNameForVoice()));//������ǰ�û��ֻ���
    DEL_SetTimer(0,120);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    //��������
    KeyBatteryReport();
    break;
  case Key3_OptionOne://���������˺š���ص���
    //��ǰ�û���
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetLocalUserNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetLocalUserNameForVoice(),strlen((char const *)GetLocalUserNameForVoice()));//���������û�
    DEL_SetTimer(0,100);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    //��ǰȺ��
    api_lcd_pwr_on_hint(0,2,"                ");//��ʾ��ǰȺ���ǳ�
    api_lcd_pwr_on_hint4(GetNowWorkingGroupNameForDisplay());
    //��������
    KeyBatteryReport();
    break;
  case Key3_OptionTwo://���������˺�
    //��ǰ�û���
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetLocalUserNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetLocalUserNameForVoice(),strlen((char const *)GetLocalUserNameForVoice()));//���������û�
    DEL_SetTimer(0,100);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    //��ǰȺ��
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetNowWorkingGroupNameForDisplay());
    break;
  case Key3_OptionThree://������ǰȺ��
    //��ǰȺ��
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetNowWorkingGroupNameForDisplay());
    VOICE_SetOutput(ATVOICE_FreePlay,GetNowWorkingGroupNameForVoice(),strlen((char const *)GetNowWorkingGroupNameForVoice()));//������ǰ�û��ֻ���
    DEL_SetTimer(0,20);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    break;
  case Key3_OptionFour://������ص���
    //��������
    KeyBatteryReport();
    DEL_SetTimer(0,20);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    break;
  default:
    break;
  }
}

bool TASK_PersonalKeyMode(void)
{
  return TaskDrvobj.status.PersonalKeyMode;
}
void TASK_PersonalKeyModeSet(bool a)
{
  TaskDrvobj.status.PersonalKeyMode=a;
}
bool task_status_account_config(void)
{
  return TaskDrvobj.status.AccountConfig;
}