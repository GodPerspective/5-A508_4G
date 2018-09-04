#define DELLABEL
#include "AllHead.h"

#define DEL10MSLEN		0x0A
#define DEL_IDLE		0x00
#define DEL_RUN			0x01

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
  u8 BacklightTimeBuf[1];//�����ʱ��(��Ҫ���ý���eeprom)
  u8 KeylockTimeBuf[1];//������ʱ��(��Ҫ���ý���eeprom)
}DEL_DRV;

static DEL_DRV	DelDrvObj;
static void DEL_100msProcess(void);
static void DEL_500msProcess(void);
static void DEL_1msProcess(void);
static void DEL_10msProcess(void);
static void DEL_TimerRenew(void);

void DEL_PowerOnInitial(void)//ԭ������Ƭ�C���L�r�g�Mһ���Д�
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
  return;
}

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
    //ApiAtCmd_Get_DateTime_Information();
    ApiGpsCmd_100msRenew();
    if(DelDrvObj.Msg.Bit.b500Alternate == DEL_IDLE)
    {
      DelDrvObj.Msg.Bit.b500Alternate = DEL_RUN;
      ApiAtCmd_100msRenew();
    }
    else
    {
      DelDrvObj.Msg.Bit.b500Alternate = DEL_IDLE;
      //ApiPocCmd_1sRenew();
    }
  }
  return;
}
#if 1//WCDMA ׿�Ǵ�
static void DEL_500msProcess(void)			//delay 500ms process server
{
  if (DelDrvObj.Msg.Bit.b500ms == DEL_RUN) 
  {
    DelDrvObj.Msg.Bit.b500ms = DEL_IDLE;
    VOICE_1sProcess();
    DelDrvObj.Count.TimeCount_Light++;
    DelDrvObj.Count.CSQTimeCount++;
/*********�������SIM��*************/
    if(ApiAtCmd_bStartingUp()==1&&ApiAtCmd_bCardIn()==0)
    {
      DelDrvObj.Count.CIMICount++;
      if(DelDrvObj.Count.CIMICount>2*3)
      {
        ApiAtCmd_WritCommand(ATCOMM_CIMI,0,0);
        DelDrvObj.Count.CIMICount=0;
      }
      //��⵽δ�忨
      if(ApiAtCmd_bNoCard()==1)
      {
        DelDrvObj.Count.NoCardCount++;
        if(DelDrvObj.Count.NoCardCount>2*8)
        {
          DelDrvObj.Count.NoCardCount=0;
          VOICE_Play(NoSimCard);
          DISPLAY_Show(d_NoSimCard);
          
        }
      } 
    }
/*********��ʱ5s��һ��[AT+CSQ?]*************************************************/
    if(DelDrvObj.Count.CSQTimeCount>=2*5)
    {
      DelDrvObj.Count.CSQTimeCount=0;
      ApiAtCmd_WritCommand(ATCOMM_CSQ, (void*)0, 0);
      
      if(GetTaskId()==Task_Start&&task_status_account_config()==FALSE&&ApiAtCmd_CSQValue()<25)//������ڿ���״̬��δд���˺�״̬�������ź�С��25״̬
      {
        VOICE_Play(NetworkSearching);
        DISPLAY_Show(d_NetworkSearching);
      }

      HDRCSQSignalIcons();
    }
/*******���PPP����**********/
    if(ApiAtCmd_bStartingUp()==1&&ApiAtCmd_bCardIn()==1)
    {
      if(ApiAtCmd_bPPPStatusOpen()==0&&ApiAtCmd_CSQValue()>=25)
      {
        DelDrvObj.Count.PPPStatusOpenCount++;
        if(DelDrvObj.Count.PPPStatusOpenCount>2*2)
        {
          ApiAtCmd_WritCommand(ATCOMM_ZPPPOPEN,0,0);
          DelDrvObj.Count.PPPStatusOpenCount=0;
        }
      }
      else
      {
        DelDrvObj.Count.PPPStatusOpenCount=0;
      }
    }
/******���ȿ�����غ���************************/
    if(ApiAtCmd_bZTTSStates_Intermediate()==1)//�������������ӳ�2��ر�
    {
      DelDrvObj.Count.ztts_states_intermediate_count++;
      if(DelDrvObj.Count.ztts_states_intermediate_count>2*2)
      {
        set_ApiAtCmd_bZTTSStates_Intermediate(0);
        set_ApiAtCmd_bZTTSStates(0);
        DelDrvObj.Count.ztts_states_intermediate_count = 0;
      }
    }
    else
    {
      if(ApiAtCmd_bZTTSStates()==1)
      {
        DelDrvObj.Count.ztts_states_count++;
        if(DelDrvObj.Count.ztts_states_count>2*20)
        {
          set_ApiAtCmd_bZTTSStates(0);
          DelDrvObj.Count.ztts_states_count=0;
        }
      }
    }
    
    if(ApiPocCmd_ReceivedVoicePlayStatesIntermediate()==TRUE)//�Խ�����
    {
      DelDrvObj.Count.ReceivedVoicePlayStatesCount++;  
      if(DelDrvObj.Count.ReceivedVoicePlayStatesCount>1)
      {
        ApiPocCmd_ReceivedVoicePlayStatesIntermediateSet(FALSE);
        ApiPocCmd_ReceivedVoicePlayStatesSet(FALSE);
        DelDrvObj.Count.ReceivedVoicePlayStatesCount=0;
      }
    }
    
    if(ApiPocCmd_ToneStateIntermediate()==TRUE||ApiPocCmd_ToneState()==TRUE)//bb��
    {
      DelDrvObj.Count.ToneStateCount++;
      if(DelDrvObj.Count.ToneStateCount>1)
      {
        ApiPocCmd_ToneStateSet(FALSE);
        ApiPocCmd_ToneStateIntermediateSet(FALSE);
        DelDrvObj.Count.ToneStateCount=0;
      }
    }
/****��λ�ɹ���3s���ڲ˵�ģʽ�²��ܲ鿴����γ����Ϣ������ն�λ�ɹ��鿴��γ���쳣�����⣩*********************************************************/
    if(poc_gps_value_for_display_flag()==TRUE)
    {
      DelDrvObj.Count.poc_gps_value_for_display_flag_count++;
      if(DelDrvObj.Count.poc_gps_value_for_display_flag_count>2*8)
      {
        DelDrvObj.Count.poc_gps_value_for_display_flag_count=0;
        DelDrvObj.poc_gps_value_for_display_flag2=TRUE;
      }
    }
    else
    {
      DelDrvObj.Count.poc_gps_value_for_display_flag_count=0;
      DelDrvObj.poc_gps_value_for_display_flag2=FALSE;
    }
/******��¼״̬�µĵ͵籨��**********************************************/
    if(LobatteryTask_StartFlag==TRUE)
    {
      DelDrvObj.Count.LobatteryTask_StartCount++;
      if(DelDrvObj.Count.LobatteryTask_StartCount==1)
      {
        VOICE_Play(PowerLowPleaseCharge);
        DISPLAY_Show(d_PowerLowPleaseCharge);
      }
      if(DelDrvObj.Count.LobatteryTask_StartCount>2*5)
      {
        DelDrvObj.Count.LobatteryTask_StartCount=0;
        LobatteryTask_StartFlag=FALSE;
      }
    }
/*****����дƵ״̬5s��дƵ��־λ����****************/
    if(WriteFreq_Flag==TRUE)
    {
      if(DelDrvObj.Count.WriteFreqTimeCount>=2*6)
      {
        WriteFreq_Flag=FALSE;
        DelDrvObj.Count.WriteFreqTimeCount=0;
      }
      DelDrvObj.Count.WriteFreqTimeCount++;
    }  
/*********������������30s����һ��********************************/
    if(PrimaryLowPower_Flag==TRUE)
    {
      DelDrvObj.Count.PrimaryLowPowerCount++;
      if(DelDrvObj.Count.PrimaryLowPowerCount>=2*30)
      {
        DelDrvObj.Count.PrimaryLowPowerCount=0;
        VOICE_Play(LowBattery);
        PrimaryLowPower_Flag=FALSE;
      }
    }
/**��¼�ɹ������Ⱥ����Ϣ������ȡ�����������»�ȡֱ������**/
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
    //if(poc_first_enter_into_group_flag()==TRUE&&)
/********�յ�0x8a�����һ������********/
    if(poc_receive_sos_statas()==TRUE)
    {
      BEEP_SetOutput(BEEP_IDPowerOff,ON,0x00,TRUE);
      ApiPocCmd_ToneStateSet(TRUE);
      AUDIO_IOAFPOW(ON);
      DelDrvObj.Count.receive_sos_statas_count++;
      if(DelDrvObj.Count.receive_sos_statas_count>=2*5)
      {
        Set_RedLed(LED_OFF);
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
/*******�յ�����ָ���Ļ��ʾ����״̬*******/
    if(GetTaskId()==Task_NormalOperation&&poccmd_states_poc_status()==OffLine)
    {
      DISPLAY_Show(d_status_offline);
    }
/*******�յ�����ָ���1����δ��½����*******/
    if(poccmd_states_poc_status()==OffLine)
    {
      DelDrvObj.Count.poc_status_count++;
      
      if(DelDrvObj.Count.poc_status_count>2*60)
      {
        DelDrvObj.Count.poc_status_count=0;
        main_init();//����
      }
    }
    else
    {
      DelDrvObj.Count.poc_status_count=0;
    }
/****��½�ɹ�һ���Ӻ����дƵ���ܡ������ⲿ��λ�ϱ�ģʽ*********/
    if(GetTaskId()==Task_NormalOperation)
    {
      DelDrvObj.Count.choose_write_freq_or_gps_count++;
      if(DelDrvObj.Count.choose_write_freq_or_gps_count>2*60)
      {
#if 1//���Զ�дƵ������ʱ����
        GPIO_WriteLow(GPIOB,GPIO_PIN_3);//NFC
        GPIO_WriteHigh(GPIOB,GPIO_PIN_4);//����
#endif
        DelDrvObj.Count.choose_write_freq_or_gps_count = 2*60+1;
      }
    }
    else
    {
      DelDrvObj.Count.choose_write_freq_or_gps_count = 0;
    }
/****��λ�ɹ���5s�ϱ�һ�ζ�λ*****/
    if(beidou_valid()==TRUE)
    {
      DelDrvObj.Count.beidou_valid_count++;
      if(DelDrvObj.Count.beidou_valid_count>2*5)
      {
        ApiPocCmd_WritCommand(PocComm_SetGps,0,0);
        DelDrvObj.Count.beidou_valid_count=0;
      }
    }
    else
    {
      DelDrvObj.Count.beidou_valid_count=0;
    }
/***********************************************************/
    FILE_Read(0x236,1,DelDrvObj.BacklightTimeBuf);//����ʱ�䡾�롿
    FILE_Read(0x247,1,DelDrvObj.KeylockTimeBuf);//������ʱ�䡾�롿
    BacklightTimeCount=5*DelDrvObj.BacklightTimeBuf[0];
    if(DelDrvObj.KeylockTimeBuf[0]==0)
      KeylockTimeCount=200*2;//���=200����Զ������
    else
      KeylockTimeCount=5*DelDrvObj.KeylockTimeBuf[0];
    if(DelDrvObj.Count.TimeCount_Light>=2*BacklightTimeCount)//10s
    {
      MCU_LCD_BACKLIGTH(OFF);//�رձ����
      DelDrvObj.Count.TimeCount_Light=2*BacklightTimeCount;
    }
    else
    {
      MCU_LCD_BACKLIGTH(ON);//�򿪱����
    }
    if(NumberKeyboardPressDown_flag==TRUE||LockingState_EnterOK_Flag==TRUE)
    {
      DelDrvObj.Count.TimeCount_Light=0;//����Ƽ���������
      //NumberKeyboardPressDown_flag=FALSE;
    }
    
    if(GetTaskId()==Task_NormalOperation)
    {
      if(KeylockTimeCount==200*2)
      {
        TimeCount=0;
      }
      else
      {
        TimeCount++;
        if(TimeCount>=KeylockTimeCount*2) //��ʱ������
        {
          if(TimeCount==KeylockTimeCount*2)
          {
            LockingState_Flag=TRUE;//��ʱ������־λ
            MenuDisplay(Menu_RefreshAllIco);
            get_screen_display_group_name();//ѡ����ʾ��ǰȺ���ǳƣ�Ⱥ��򵥺���ʱȺ�飩
            api_disp_all_screen_refresh();// ȫ��ͳһˢ��

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
          TimeCount=KeylockTimeCount*2+1;
        }
        else
        {
        }
        if(NumberKeyboardPressDown_flag==TRUE&&TimeCount>=KeylockTimeCount*2)//����10����ٰ�������ʾ����OK���ٰ�*����
        {
          DelDrvObj.Count.TimeCount2++;
          DISPLAY_Show(d_press_ok_then_well);//��ʾ�Ȱ�ok���ٰ�#
          if(DelDrvObj.Count.TimeCount2>=2)//0.5s
          {
            DelDrvObj.Count.TimeCount2=0;
            NumberKeyboardPressDown_flag=FALSE;
            MenuDisplay(Menu_Locking_NoOperation);
          }
        }
        if(LockingState_EnterOK_Flag==TRUE)//�������水��OK��
        {                                                           
          TimeCount3++;//�����ɹ�ҲӦ�����㣬
          //MCU_LCD_BACKLIGTH(ON);//�򿪱����
          if(TimeCount3>=4*2)//3s
          {
            TimeCount3=0;
            //MCU_LCD_BACKLIGTH(OFF);//�رձ����
            LockingState_EnterOK_Flag=FALSE;
            MenuDisplay(Menu_Locking_NoOperation);
          }
        }
      }
      if(NumberKeyboardPressDown_flag==TRUE&&TimeCount<KeylockTimeCount*2)//���������ּ��̰���
      {
        TimeCount=0;//���а������£�����������
        NumberKeyboardPressDown_flag=FALSE;
      }
      }
      
        
       
   //   break;
   // default:
   //   break;
   // }
  }
  return;
}
#else
static void DEL_500msProcess(void)			//delay 500ms process server
{
  u8 i;
  u8 ShowTimeBuf1[6]={0,0,0,0,0,0};
  if (DelDrvObj.Msg.Bit.b500ms == DEL_RUN) 
  {
    DelDrvObj.Msg.Bit.b500ms = DEL_IDLE;
    VOICE_1sProcess();
    DEL_500ms_Count++;
    DEL_500ms_Count2++;
    TimeCount_Light++;
    DelDrvObj.Count.CSQTimeCount++;
    GetAllGroupMemberNameCount++;
/******��������־λ��ʱ����ʾʹ��**********/
    if(KEY_4_Flag==TRUE)
    {
      KEY_4Count++;
      if(KEY_4Count>2*2)
      {
        KEY_4Count=0;
        KEY_4_Flag=FALSE;
      }
    }
    else
    {
      KEY_4Count=0;
    }
/******����Ⱥ��ģʽ5����ʾʱ��*******************/
    if(POC_GetAllGroupNameDone_Flag==TRUE&&
       MenuMode_Flag==0&&
       POC_EnterPersonalCalling_Flag==0&&
       POC_QuitPersonalCalling_Flag==0&&
       POC_AtEnterPersonalCalling_Flag==0&&
       POC_AtQuitPersonalCalling_Flag==0&&
       KEY_4_Flag==FALSE&&
       KeyDownUpChoose_GroupOrUser_Flag==0)
    {
      ShowTimeCount++;
      if(ShowTimeCount>2*5)
      {
        ShowTime_Flag=TRUE;
        ShowTimeCount=11;
        if(Data_Time0()<=0x09&&Data_Time1()<=0x09)
        {
          ShowTimeBuf1[0]='0';
          COML_HexToAsc(Data_Time0(),ShowTimeBuf1+1);
          ShowTimeBuf1[2]=':';
          ShowTimeBuf1[3]='0';
          COML_HexToAsc(Data_Time1(),ShowTimeBuf1+4);
        }
        else if(Data_Time0()<=0x09&&Data_Time1()>0x09)
        {
          ShowTimeBuf1[0]='0';
          COML_HexToAsc(Data_Time0(),ShowTimeBuf1+1);
          ShowTimeBuf1[2]=':';
          COML_HexToAsc(Data_Time1(),ShowTimeBuf1+3);
          COML_StringReverse(2,ShowTimeBuf1+3);
        }
        else if(Data_Time0()>0x09&&Data_Time1()<=0x09)
        {
          COML_HexToAsc(Data_Time0(),ShowTimeBuf1);
          COML_StringReverse(2,ShowTimeBuf1);
          ShowTimeBuf1[2]=':';
          ShowTimeBuf1[3]='0';
          COML_HexToAsc(Data_Time1(),ShowTimeBuf1+4);
        }
        else//
        {
          COML_HexToAsc(Data_Time0(),ShowTimeBuf1);
          COML_StringReverse(2,ShowTimeBuf1);
          ShowTimeBuf1[2]=':';
          COML_HexToAsc(Data_Time1(),ShowTimeBuf1+3);
          COML_StringReverse(2,ShowTimeBuf1+3);
        }
        ShowTimeBuf1[5]='\0';
        api_lcd_pwr_on_hint7(ShowTimeBuf1);
      }
    }
    else
    {
      ShowTime_Flag=FALSE;
      ShowTimeCount=0;
      if(MenuMode_Flag!=1)
      {
        if(NetworkType_2Gor3G_Flag==3)
          api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3Gͼ��
        else
          api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//ͼ�꣺2G
        if(VoiceType_FreehandOrHandset_Flag==0)
          api_disp_icoid_output( eICO_IDTemper, TRUE, TRUE);//����ģʽ
        else
          api_disp_icoid_output( eICO_IDMONITER, TRUE, TRUE);//��Ͳģʽͼ��
        if(PersonCallIco_Flag==0)
          api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
        else
          api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
        if(KeyDownUpChoose_GroupOrUser_Flag==0)
          api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
        else
          api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//ѡ
      }

    }
    
/*****5�����ȿ�����ر�����**************/
    if(ApiAtCmd_ZTTS_Flag==TRUE)
    {
      ApiAtCmd_ZTTSCount++;
      if(ApiAtCmd_ZTTSCount>2*15)
      {
        ApiAtCmd_ZTTS_Flag=FALSE;
        ApiAtCmd_ZTTSCount=0;
      }
    }
    else
    {
      ApiAtCmd_ZTTSCount=0;
    }
/*****������뵥��ģʽ��δ��PTT���쳣״̬���⣬���뵥����ʱ30s�����˳�***************/
    /*if(POC_AtEnterPersonalCalling_Flag==1&&POC_AtQuitPersonalCalling_Flag==1)
    {
      PersonalCallingCount++;
      if(PersonalCallingCount>30*2)
      {
        PersonalCallingCount=0;
      }
    }*/
/******������ȡȺ����Ϣ��2s������Ч***************/
    if(POC_GetAllGroupNameStart_Flag==TRUE)
    {
      POC_GetAllGroupNameDoneCount++;
      if(POC_GetAllGroupNameDoneCount>4)
      {
        POC_GetAllGroupNameStart_Flag=FALSE;
        POC_GetAllGroupNameDoneCount=0;
        POC_GetAllGroupNameDone_Flag=TRUE;
      }
    }
/*******1���ӻ�ȡһ��Ⱥ���Ա**********************************************/
    if(GetAllGroupMemberNameCount>2*60)
    {
      ApiPocCmd_WritCommand(PocComm_UserListInfo,ucRequestUserListInfo,strlen((char const *)ucRequestUserListInfo));
      GetAllGroupMemberNameCount=0;
    }
/**********��ָ����޻�����������״̬**********************************/
   /* if(UpgradeNoATReturn_Flag==TRUE)
    {
      UpgradeNoATReturn_Count++;
      if(UpgradeNoATReturn_Count>=4)
      {
        UpgradeNoATReturn_Count=0;
        UpgradeNoATReturn_Flag2=TRUE;
      }
    }
    else
    {
      UpgradeNoATReturn_Count=0;
    }*/
/*********��������δ��ȡ�����߳�Ա����ʱ�˻����ģʽ*************************/
    if(Key_PersonalCalling_Flag==1&&GettheOnlineMembersDone==FALSE)
    {
      GetNoOnlineMembersCount++;
      if(GetNoOnlineMembersCount>2*3)
      {
        GetNoOnlineMembersCount=0;
        Key_PersonalCalling_Flag=0;//���������־λ
        api_lcd_pwr_on_hint("                ");//����
        //api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
        api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
        MenuMode_Flag=0;
        //����PTT�������¼�����Ĭ��״̬
        KeyUpDownCount=0;
        KeyDownUpChoose_GroupOrUser_Flag=0;//����������������ؼ���OK��PTT����Ļ��ʾ�����BUG
      }
    }
    else
    {
      if(GettheOnlineMembersDone==TRUE)
      {
        GetNoOnlineMembersCount=0;
      }
    }
/*******�����߳�Ա����*******************/
    if(PocNoOnlineMember_Flag==TRUE)
    {
      PocNoOnlineMemberCount++;
      if(PocNoOnlineMemberCount>2*1)
      {
        PocNoOnlineMemberCount=0;
        PocNoOnlineMember_Flag=FALSE;
        PocNoOnlineMember_Flag2=TRUE;
      }
    }
/******��¼״̬�µĵ͵籨��**********************************************/
    if(LobatteryTask_StartFlag==TRUE)
    {
      DelDrvObj.Count.LobatteryTask_StartCount++;
      if(DelDrvObj.Count.LobatteryTask_StartCount==1)
      {
        VOICE_SetOutput(ATVOICE_FreePlay,"f78b45513575",12);//����������
      }
      if(DelDrvObj.Count.LobatteryTask_StartCount>2*5)
      {
        DelDrvObj.Count.LobatteryTask_StartCount=0;
        LobatteryTask_StartFlag=FALSE;
      }
    }
/**********����������쳣�������⣬�����̵�****************************/
    if(POC_ReceivedVoice_Flag==TRUE)
    {
      POC_ReceivedVoiceCount++;
      if(POC_ReceivedVoiceCount>2*30)
      {
        POC_ReceivedVoiceCount=0;
        POC_ReceivedVoice_Flag=FALSE;
        POC_ReceivedVoiceEnd_Flag=2;//0:���� 1���յ����� 2���ս�������
        POC_ReceivedVoiceEndForXTSF_Flag=2;
        POC_ReceivedVoiceStart_Flag=0;//0:���� 1���յ����� 2���տ�ʼ����
      }
    }
    else
    {
      POC_ReceivedVoiceCount=0;
    }
/*********�ܵ�������ָ���ӳ�����ر�******************************************/
#if 0
    //if(GetTaskId()==Task_NormalOperation)
    {
      if(ApiAtCmd_TrumpetVoicePlay_Flag==2)
      {
        ApiAtCmd_TrumpetVoicePlayCount++;
        if(ApiAtCmd_TrumpetVoicePlayCount>2*2)//ԭ��2s�����ڸ�Ϊ0.5s
        {
          ApiAtCmd_TrumpetVoicePlay_Flag=0;
          ApiAtCmd_TrumpetVoicePlayCount=0;
          AUDIO_IOAFPOW(OFF);
        }
      }
    }
#endif
/*******��ʼ��ȥ��ʱ�ö�ʱ**************************/
    if(TaskStartDeleteDelay1==2)//������Ǣ����Խ�
    {
      TaskStartDeleteDelay1Count++;
      if(TaskStartDeleteDelay1Count>=6)
      {
        TaskStartDeleteDelay1Count=0;
        TaskStartDeleteDelay2=1;
      }
    }
    if(TaskStartDeleteDelay3==2)//�첻����
    {
      TaskStartDeleteDelay3Count++;
      if(TaskStartDeleteDelay3Count>=2*10)
      {
        TaskStartDeleteDelay3Count=0;
        TaskStartDeleteDelay3=1;
      }
    }
    if(TaskStartDeleteDelay4==2)//�����˺���Ϣ
    {
      TaskStartDeleteDelay4Count++;
      if(TaskStartDeleteDelay4Count>=2*4)
      {
        TaskStartDeleteDelay4Count=0;
        TaskStartDeleteDelay5=1;
      }
    }
    if(TaskStartDeleteDelay6==0)
    {
      TaskStartDeleteDelay6Count++;
      if(TaskStartDeleteDelay6Count>=2*6)//6�벦һ����������
      {
        TaskStartDeleteDelay6Count=0;
        TaskStartDeleteDelay6=1;
      }
    }
/*******������з���һ�κ��У������������̵Ƶ�����**********************************************/
#if 0//ȥ��
    if(POC_ReceivedNoVoice_Flag==TRUE)
    {
      POC_ReceivedNoVoiceCount++;
      if(POC_ReceivedNoVoiceCount>=2)
      {
        POC_ReceivedNoVoiceCount=2;
        Set_GreenLed(LED_ON);
        api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//�����ź�ͼ��
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      }
    }
    else
    {
      POC_ReceivedNoVoiceCount=0;
    }
#endif

/*****************************************************/
#if 0
    if(UpDownSwitching_Flag==TRUE)
    {
      UpDownSwitchingCount++;
      if(UpDownSwitchingCount>1)
      {
        UpDownSwitchingCount=0;
        UpDownSwitching_Flag=FALSE;
        AUDIO_IOAFPOW(OFF);
      }
    }
#endif
/*****����дƵ״̬5s��дƵ��־λ����****************/
    if(WriteFreq_Flag==TRUE)
    {
      if(DelDrvObj.Count.WriteFreqTimeCount>=10)
      {
        WriteFreq_Flag=FALSE;
        DelDrvObj.Count.WriteFreqTimeCount=0;
      }
      DelDrvObj.Count.WriteFreqTimeCount++;
    }
/***************/
    if(KeyDownUpChoose_GroupOrUser_Flag==3)
    {
      if(EnterKeyTimeCount>=4)
      {
        EnterKeyTimeCount=0;
        EnterKeyTime_2s_Flag=TRUE;
      }
      EnterKeyTimeCount++;
    }
    
/*********������������30s����һ��********************************/
    if(PrimaryLowPower_Flag==TRUE)
    {
      DelDrvObj.Count.PrimaryLowPowerCount++;
      if(DelDrvObj.Count.PrimaryLowPowerCount>=2*30)
      {
        DelDrvObj.Count.PrimaryLowPowerCount=0;
        VOICE_SetOutput(ATVOICE_FreePlay,"3575606c3575cf914e4f0cfff78b45513575",36);//������ص���������
        PrimaryLowPower_Flag=FALSE;
      }
    }
/*********��¼����60s����*********************************/
    if(Task_Landing_Flag==TRUE)
    {
      LandingTimeCount++;
      if(LandingTimeCount>=2*60)
      {
        LandingTimeCount=0;
        Task_Landing_Flag=FALSE;
        //ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(void*)0, 0);
      }
    }
    else
    {
      LandingTimeCount=0;
    }
/*********��ʱ5s��һ��[AT+CSQ?]*************************************************/
    //if(KaiJi_Flag==TRUE)
    //{
      if(DelDrvObj.Count.CSQTimeCount>=2*2)
      {
          DelDrvObj.Count.CSQTimeCount=0;
          if(NetworkType_2Gor3G_Flag==3)//�����3G����HDRCSQ��2G����CSQ
          {
            if(BootProcess_SIMST_Flag!=2)
            {
              //ApiAtCmd_WritCommand(ATCOMM15_HDRCSQ, (void*)0, 0);
            }  
          }
          else
          {
            if(NetworkType_2Gor3G_Flag==2)
            {
              ApiAtCmd_WritCommand(ATCOMM6_CSQ, (void*)0, 0);
            }
          }
          HDRCSQSignalIcons();
      }
   // }

/******************************************************************************/
    if(GetTaskId()==Task_NormalOperation)
    {
      if(HDRCSQValue<=30)
      {
        SignalPoorCount++;
        if(SignalPoorCount==20*2||SignalPoorCount==40*2)
        {
          //���������ź���
          VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7ee14ff753315f",20);
        }
        if(SignalPoorCount>=60*2)//���ź���ʮ�룬60s����һ��
        {
          ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(void*)0, 0);
          SignalPoorCount=0;
        }
      }
      else
      {
        SignalPoorCount=0;
      }
    }

/***********************************************************/
    if(ApiAtCmd_GetLoginState()==TRUE)//��¼�ɹ�
      {
        GpsReconnectionTimeCount++;
#ifdef BEIDOU
        PowerOnCount++;
        if(PowerOnCount>=2*60)//������ʱ����1min�����ڱ���ģʽ
        {
          GPIO_WriteLow(GPIOB,GPIO_PIN_3);//NFC
          GPIO_WriteHigh(GPIOB,GPIO_PIN_4);//����
          PowerOnCount=2*60;
        }
#endif
        if(GpsReconnectionTimeCount==2*10)
        {
          ApiPocCmd_WritCommand(PocComm_UserListInfo,ucRequestUserListInfo,strlen((char const *)ucRequestUserListInfo));
          switch(ApiGpsServerType)
          {
          case GpsServerType_BuBiao:
            NoUseNum=ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)ucGPSUploadTime_5s,strlen((char const *)ucGPSUploadTime_5s));//����GPS��λ��Ϣ5s����һ��
            break;
          case GpsServerType_ZTE:
            break;
          case GpsServerType_BuBiaoAndZTE:
            break;
          }
          GpsReconnectionTimeCount=21;
        }
        if(GpsReconnectionTimeCount>=25)
        {GpsReconnectionTimeCount=21;}
      }
    if(ApiPocCmd_Tone_Flag==TRUE)
    {
      ToneTimeCount++;
      if(ToneTimeCount>1)
      {
        ApiPocCmd_Tone_Flag=FALSE;
        ToneTimeCount=0;
      }
    }
    else
    {
      ToneTimeCount=0;
    }
    
    FILE_Read(0x236,1,DelDrvObj.BacklightTimeBuf);//����ʱ�䡾�롿
    FILE_Read(0x247,1,DelDrvObj.KeylockTimeBuf);//������ʱ�䡾�롿
    BacklightTimeCount=5*DelDrvObj.BacklightTimeBuf[0];
    if(DelDrvObj.KeylockTimeBuf[0]==0)
      KeylockTimeCount=200*2;//���=200����Զ������
    else
      KeylockTimeCount=5*DelDrvObj.KeylockTimeBuf[0];
    if(TimeCount_Light>=2*BacklightTimeCount)//10s
    {
      MCU_LCD_BACKLIGTH(OFF);//�رձ����
      TimeCount_Light=2*BacklightTimeCount;
    }
    else
    {
      MCU_LCD_BACKLIGTH(ON);//�򿪱����
    }
    if(NumberKeyboardPressDown_flag==TRUE||LockingState_EnterOK_Flag==TRUE)
    {
      TimeCount_Light=0;//����Ƽ���������
      //NumberKeyboardPressDown_flag=FALSE;
    }
    
    //if(DEL_500ms_Count>1) DEL_500ms_Count=0;
    //switch(DEL_500ms_Count)
    //{
   // case 1://1s
      if(GetTaskId()==Task_NormalOperation)
      {
        if(KeylockTimeCount==200*2)
        {
          TimeCount=0;
          //NumberKeyboardPressDown_flag=TRUE;
        }
        else
        {
        TimeCount++;

        if(TimeCount>=KeylockTimeCount*2) //��ʱ������
        {
          if(TimeCount==KeylockTimeCount*2)
          {
            LockingState_Flag=TRUE;//��ʱ������־λ
            //���BUG���������Ӱ��һ�������˵���ʾ���ִ���취Ϊ�������˻�Ĭ��Ⱥ��״̬,���в˵���־λ��ʼ��
            //api_lcd_pwr_on_hint3("                ");//����
            MenuDisplay(Menu_RefreshAllIco);
            if(PersonCallIco_Flag==0)
            {
              api_lcd_pwr_on_hint("                ");//����
              
          TASK_PersonalKeyModeSet(FALSE);
          MenuMode_Flag=0;
          api_lcd_pwr_on_hint(0,2,"                ");//����
          api_lcd_pwr_on_hint(0,2,GetNowWorkingGroupNameForDisplay());//��ʾ��ǰȺ���ǳ�
          KeyDownUpChoose_GroupOrUser_Flag=0;
          KeyUpDownCount=0;
              
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              api_disp_all_screen_refresh();// ȫ��ͳһˢ��
            }
            else
            {
              api_lcd_pwr_on_hint("                ");//����
              //api_lcd_pwr_on_hint(HexToChar_MainUserId());//��ʾ��ǰ�û�ID
              //api_lcd_pwr_on_hint(HexToChar_PersonalCallingNum());//��ʾ��ǰ�û�ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainUserName());//��ʾ��ǰ�û��ǳ�
              api_disp_all_screen_refresh();// ȫ��ͳһˢ��
            }

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
          TimeCount=KeylockTimeCount*2+1;
        }
        else
        {
          //MCU_LCD_BACKLIGTH(ON);//�򿪱����
        }
        if(NumberKeyboardPressDown_flag==TRUE&&TimeCount>=KeylockTimeCount*2)//����10����ٰ�������ʾ����OK���ٰ�*����
        {
          DelDrvObj.Count.TimeCount2++;
          api_lcd_pwr_on_hint("��OK��,�ٰ�#��  ");//
          if(DelDrvObj.Count.TimeCount2>=2)//0.5s
          {
            DelDrvObj.Count.TimeCount2=0;
            NumberKeyboardPressDown_flag=FALSE;
            MenuDisplay(Menu_Locking_NoOperation);
          }
        }
        if(LockingState_EnterOK_Flag==TRUE)//�������水��OK��
        {                                                           
          TimeCount3++;//�����ɹ�ҲӦ�����㣬
          //MCU_LCD_BACKLIGTH(ON);//�򿪱����
          if(TimeCount3>=4*2)//3s
          {
            TimeCount3=0;
            //MCU_LCD_BACKLIGTH(OFF);//�رձ����
            LockingState_EnterOK_Flag=FALSE;
            MenuDisplay(Menu_Locking_NoOperation);
          }
        }
      }
      if(NumberKeyboardPressDown_flag==TRUE&&TimeCount<KeylockTimeCount*2)//���������ּ��̰���
      {
        TimeCount=0;//���а������£�����������
        NumberKeyboardPressDown_flag=FALSE;
      }
      }
      
        
       
   //   break;
   // default:
   //   break;
   // }
  }
  return;
}
#endif

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
    DelDrvObj.Msg.Bit.b10ms = DEL_IDLE;
    ApGpsCmd_10msRenew();

      DelDrvObj.Count.alarm_count++;
      if(poc_receive_sos_statas()==TRUE)
      {
        if(DelDrvObj.Count.alarm_count>=3)
      {
        //Test_PWM_LED();//������30ms��һ��
        DelDrvObj.Count.alarm_count = 0;
      }
      }

  }
  return;
}

bool delay_gps_value_for_display_flag2(void)
{
  return DelDrvObj.poc_gps_value_for_display_flag2;
}

u8 read_backlight_time_value(void)
{
  return DelDrvObj.BacklightTimeBuf[0];
}
u8 read_key_lock_time_value(void)
{
  return DelDrvObj.KeylockTimeBuf[0];
}