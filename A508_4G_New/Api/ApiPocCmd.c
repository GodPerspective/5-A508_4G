#include "ALLHead.h"

const u8 *ucAtPocHead          = "AT+POC=";
const u8 *ucSetIPAndID         = "010000";

const u8 *ucPocOpenConfig       ="000000010101";
const u8 *ucPocOpenConfig2       ="00000001010101";


const u8 *ucStartPTT                  = "0B0000";
const u8 *ucEndPTT                    = "0C0000";
const u8 *ucGroupListInfo             = "0d0000";
const u8 *ucUserListInfo              = "0e00000000";
const u8 *ucSetGPS                    = "110000";
const u8 *ucAlarm1                    = "210000";
const u8 *ucAlarm2                    = "00000000736f7300";
const u8 *ucSetURL                    = "120000";
const u8 *ucPunch_the_clock_gps       = "31000001";//��λ��
const u8 *ucPunch_the_clock_nfc       = "31000000";//NFC��

PocCmdDrv PocCmdDrvobj;
static bool no_online_user(void);

void ApiPocCmd_PowerOnInitial(void)
{
  PocCmdDrvobj.NetState.Msg.Byte=0;
  PocCmdDrvobj.NetState.LoginInfo.Msg.Byte=0;
  memset(PocCmdDrvobj.NetState.LoginInfo.Buf,0,sizeof(PocCmdDrvobj.NetState.LoginInfo.Buf));

  PocCmdDrvobj.States.current_working_status = m_group_mode;
  PocCmdDrvobj.States.PocStatus = OffLine;
  PocCmdDrvobj.States.GroupStats = LeaveGroup;
  PocCmdDrvobj.States.KeyPttState = 0;
  PocCmdDrvobj.States.ReceivedVoicePlayStates = FALSE;
  PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate = FALSE;//����
  PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED = FALSE;
  PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay = ReceivedNone;
  PocCmdDrvobj.States.ToneState = FALSE;
  PocCmdDrvobj.States.ToneState_Intermediate = FALSE;
  PocCmdDrvobj.States.receive_sos_statas= FALSE;
  PocCmdDrvobj.States.first_enter_into_group_flag=FALSE;
  PocCmdDrvobj.States.gps_value_for_display_flag=FALSE;
  
  //Ⱥ��δ����ʼ��
  
  memset(PocCmdDrvobj.ReadBuffer,0,sizeof(PocCmdDrvobj.ReadBuffer));
  memset(PocCmdDrvobj.ReadBuffer2,0,sizeof(PocCmdDrvobj.ReadBuffer2));
  memset(PocCmdDrvobj.NowWorkingGroupNameBuf,0,sizeof(PocCmdDrvobj.NowWorkingGroupNameBuf));
  memset(PocCmdDrvobj.AllGroupNameBuf,0,sizeof(PocCmdDrvobj.AllGroupNameBuf));
  memset(PocCmdDrvobj.AllUserNameBuf,0,sizeof(PocCmdDrvobj.AllUserNameBuf));
  memset(PocCmdDrvobj.SpeakingUserNameBuf,0,sizeof(PocCmdDrvobj.SpeakingUserNameBuf));
  memset(PocCmdDrvobj.LocalUserNameBuf,0,sizeof(PocCmdDrvobj.LocalUserNameBuf));
  memset(PocCmdDrvobj.ReceiveMessagesUserNameBuf,0,sizeof(PocCmdDrvobj.ReceiveMessagesUserNameBuf));
  
  memset(PocCmdDrvobj.NowWorkingGroupNameForVoiceBuf,0,sizeof(PocCmdDrvobj.NowWorkingGroupNameForVoiceBuf));
  memset(PocCmdDrvobj.AllGroupNameForVoiceBuf,0,sizeof(PocCmdDrvobj.AllGroupNameForVoiceBuf));
  memset(PocCmdDrvobj.AllUserNameForVoiceBuf,0,sizeof(PocCmdDrvobj.AllUserNameForVoiceBuf));
  memset(PocCmdDrvobj.LocalUserNameForVoiceBuf,0,sizeof(PocCmdDrvobj.LocalUserNameForVoiceBuf));
  
  PocCmdDrvobj.offline_user_count=0;
  PocCmdDrvobj.all_user_num=0;
  PocCmdDrvobj.group_num_max=0;
  PocCmdDrvobj.GroupXuhao=0;
  PocCmdDrvobj.UserXuhao=0;
  PocCmdDrvobj.group_list_count=0;
  PocCmdDrvobj.user_list_count=0;
  PocCmdDrvobj.getting_group_all_done_flag=0;
  PocCmdDrvobj.getting_user_all_done_flag=0;
  PocCmdDrvobj.getting_info_flag=KEYNONE;
  PocCmdDrvobj.first_exchange_group_flag=FALSE;
  
  memset(PocCmdDrvobj.GroupIdBuf,0,sizeof(PocCmdDrvobj.GroupIdBuf));
  memset(PocCmdDrvobj.UserIdBuf,0,sizeof(PocCmdDrvobj.UserIdBuf));
  memset(PocCmdDrvobj.gps_info_report,0,sizeof(PocCmdDrvobj.gps_info_report));
  
  PocCmdDrvobj.Position.latitude_float=0;
  PocCmdDrvobj.Position.latitude_integer=0;
  PocCmdDrvobj.Position.longitude_float=0;
  PocCmdDrvobj.Position.longitude_integer=0;
  
  FILE_Read(0,200,PocCmdDrvobj.ReadBuffer);//80λ
}


void ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len)
{
  u8 cBuf[4]={0,0,0,0};
  u16 i,temp_value;
  u8 gps_info_buf[25];
  u8 SetParamBuf[3]={0,0,0};
  u8 ip_pwd_buf[100];
  u8 url_buf[70];
  u8 temp_count=0;
  u8 GroupIdBuf_len=0;
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8 *)ucAtPocHead,strlen((char const *)ucAtPocHead));
  switch(id)
  {
  case PocComm_OpenPOC://1
    switch(AtCmdDrvobj.language_set)
    {
    case m_CHINESE:
      DrvGD83_UART_TxCommand((u8*)ucPocOpenConfig, strlen((char const *)ucPocOpenConfig));
      break;
    case m_ENGLISH:
      DrvGD83_UART_TxCommand((u8*)ucPocOpenConfig2, strlen((char const *)ucPocOpenConfig2));
      break;
    }
    break;
  case PocComm_SetParam://�����˺�����
    //ip=111.111.111.111;id=11111111111;pwd=111111;UPL=99999999999999999999999999999999999999999999999999;//�����URL���Ǻ���ûӰ�����ԾͲ�����
    SetParamBuf[0]='0';
    SetParamBuf[1]='0';
    memset(ip_pwd_buf,0,sizeof(ip_pwd_buf));
    for(i=0;i<strlen((char const*)PocCmdDrvobj.ReadBuffer)-7;i++)
    {
      u8 temp_count1=0;
      u8 j=0;
      if(PocCmdDrvobj.ReadBuffer[i+0]=='5'
         &&PocCmdDrvobj.ReadBuffer[i+1]=='5'
           &&PocCmdDrvobj.ReadBuffer[i+2]=='5'
             &&PocCmdDrvobj.ReadBuffer[i+3]=='0'
               &&PocCmdDrvobj.ReadBuffer[i+4]=='4'
                 &&PocCmdDrvobj.ReadBuffer[i+5]=='c'
                   &&PocCmdDrvobj.ReadBuffer[i+6]=='3'
                     &&PocCmdDrvobj.ReadBuffer[i+7]=='d')
      {
        for(temp_count1=0;temp_count1<i;temp_count1++)
        {
          ip_pwd_buf[j] = PocCmdDrvobj.ReadBuffer[temp_count1];
          j++;
        }
        break;
      }
    }
    DrvGD83_UART_TxCommand((u8 *)ucSetIPAndID,strlen((char const *)ucSetIPAndID));
    DrvGD83_UART_TxCommand(ip_pwd_buf, strlen((char const *)ip_pwd_buf));
    DrvGD83_UART_TxCommand(SetParamBuf, strlen((char const *)SetParamBuf));
    break;
  case PocComm_SetURL:
    memset(url_buf,0,sizeof(url_buf));
    for(i=0;i<strlen((char const*)PocCmdDrvobj.ReadBuffer)-7;i++)
    {
      u8 temp_count1=0;
      u8 j=0;
      if(PocCmdDrvobj.ReadBuffer[i+0]=='5'
         &&PocCmdDrvobj.ReadBuffer[i+1]=='5'
           &&PocCmdDrvobj.ReadBuffer[i+2]=='5'
             &&PocCmdDrvobj.ReadBuffer[i+3]=='0'
               &&PocCmdDrvobj.ReadBuffer[i+4]=='4'
                 &&PocCmdDrvobj.ReadBuffer[i+5]=='c'
                   &&PocCmdDrvobj.ReadBuffer[i+6]=='3'
                     &&PocCmdDrvobj.ReadBuffer[i+7]=='d')
      {
        for(temp_count1=i+8;temp_count1<strlen((char const*)PocCmdDrvobj.ReadBuffer)-2;temp_count1++)
        {
          url_buf[j] = PocCmdDrvobj.ReadBuffer[temp_count1];
          j++;
        }
        break;
      }
    }
    DrvGD83_UART_TxCommand((u8*)ucSetURL, strlen((char const *)ucSetURL));
    DrvGD83_UART_TxCommand((u8*)url_buf, strlen((char const *)url_buf));
    SetParamBuf[0]='0';
    SetParamBuf[1]='0';
    DrvGD83_UART_TxCommand(SetParamBuf, strlen((char const *)SetParamBuf));
    //DrvGD83_UART_TxCommand((u8 *)"120000687474703a2f2f736925642e7265616c7074742e636f6d3a32393939392f00",strlen((char const *)"120000687474703a2f2f736925642e7265616c7074742e636f6d3a32393939392f00"));
  case PocComm_Login:
    break;
  case PocComm_Logout:
  case PocComm_Cancel:
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_ModifyPwd:
    break;
  case PocComm_EnterGroup:
    DrvGD83_UART_TxCommand("090000", 6);
    memset(PocCmdDrvobj.GroupIdBuf,0,sizeof(PocCmdDrvobj.GroupIdBuf));
    COML_HexToAsc(PocCmdDrvobj.NameInfo.AllGroupName[groupCallingcount].ID,PocCmdDrvobj.GroupIdBuf);
    GroupIdBuf_len=strlen((char const *)PocCmdDrvobj.GroupIdBuf);
    temp_count=8-GroupIdBuf_len;
    for(i=0;i<temp_count;i++)
    {
      PocCmdDrvobj.GroupIdBuf[i+GroupIdBuf_len]=0x30;
    }
    COML_StringReverse(8,PocCmdDrvobj.GroupIdBuf);
    DrvGD83_UART_TxCommand(PocCmdDrvobj.GroupIdBuf, 8);
    break;
  case PocComm_Invite:
    DrvGD83_UART_TxCommand("0a0000", 6);
    memset(PocCmdDrvobj.UserIdBuf,0,sizeof(PocCmdDrvobj.UserIdBuf));
    COML_HexToAsc(PocCmdDrvobj.NameInfo.AllGroupUserName[PersonalCallingNum].ID,PocCmdDrvobj.UserIdBuf);
    temp_value=strlen((char const*)PocCmdDrvobj.UserIdBuf);
    if(temp_value<8)
    {
      for(i=temp_value;i<8;i++)
      {
        PocCmdDrvobj.UserIdBuf[i]=0x30;
      }
    }
    COML_StringReverse(8,PocCmdDrvobj.UserIdBuf);
    DrvGD83_UART_TxCommand(PocCmdDrvobj.UserIdBuf, 8);
    break;
  case PocComm_StartPTT://3
    DrvGD83_UART_TxCommand((u8*)ucStartPTT,strlen((char const *)ucStartPTT));
    ApiPocCmd_ToneStateSet(TRUE);
    break;
  case PocComm_EndPTT://4
    DrvGD83_UART_TxCommand((u8*)ucEndPTT,strlen((char const *)ucEndPTT));
    ApiPocCmd_ToneStateSet(TRUE);
    break;
  case PocComm_GroupListInfo://5
     DrvGD83_UART_TxCommand((u8*)ucGroupListInfo, strlen((char const *)ucGroupListInfo));
     PocCmdDrvobj.getting_group_all_done_flag=0;
     PocCmdDrvobj.getting_user_all_done_flag=0;
    break;
  case PocComm_UserListInfo://6
#if 1//Test OK
    PocCmdDrvobj.offline_user_count=0;//����0Eָ��ǰ����
    DrvGD83_UART_TxCommand((u8*)ucUserListInfo, strlen((char const *)ucUserListInfo));
    i=GetNowWorkingGroupXuhao()+1;//
    COML_HexToAsc(i,cBuf);
    switch(strlen((char const *)cBuf))
    {
    case 1:
      cBuf[1]='0';
      cBuf[2]='0';
      cBuf[3]='0';
      break;
    case 2:
      cBuf[2]='0';
      cBuf[3]='0';
      break;
    case 3:
      cBuf[3]='0';
      break;
    default:
      break;
    }
    COML_StringReverse(4,cBuf);
    DrvGD83_UART_TxCommand(cBuf,4);
    PocCmdDrvobj.getting_user_all_done_flag=1;
    PocCmdDrvobj.getting_group_all_done_flag=0;
#endif
    break;
  case PocComm_SetGps:
    DrvGD83_UART_TxCommand((u8*)ucSetGPS,strlen((char const *)ucSetGPS));
#if 1 //��γ��С��λ�ϲ����㼰��������
    PocCmdDrvobj.Position.longitude_integer=beidou_longitude_degree();
    PocCmdDrvobj.Position.longitude_float = ((beidou_longitude_minute()*10000+beidou_longitude_minute())*10/6);//С��������
    PocCmdDrvobj.Position.latitude_integer = beidou_latitude_degree();//��
    PocCmdDrvobj.Position.latitude_float = (beidou_latitude_minute()*10000+beidou_latitude_second())*10/6;//С��λ�ϲ�����
#endif
    Digital_TO_CHAR(&gps_info_buf[0],PocCmdDrvobj.Position.latitude_integer,2);
    gps_info_buf[2] = 0x2E;
    Digital_TO_CHAR(&gps_info_buf[3],PocCmdDrvobj.Position.latitude_float,6);//ת����ʽ����һ
    gps_info_buf[9] = 0x2C;
    Digital_TO_CHAR(&gps_info_buf[10],PocCmdDrvobj.Position.longitude_integer,3);
    gps_info_buf[13] = 0x2E;
    Digital_TO_CHAR(&gps_info_buf[14],PocCmdDrvobj.Position.longitude_float,6);//����Longitude����+ת����ʽ����һ
    
    CHAR_TO_DIV_CHAR(gps_info_buf, PocCmdDrvobj.gps_info_report, 20);//20
    PocCmdDrvobj.gps_info_report[41]='0';
    PocCmdDrvobj.gps_info_report[42]='0';
    DrvGD83_UART_TxCommand(PocCmdDrvobj.gps_info_report,strlen((char const *)PocCmdDrvobj.gps_info_report));
    PocCmdDrvobj.States.gps_value_for_display_flag=TRUE;
    break;
  case PocComm_Key://7
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_Alarm:
    DrvGD83_UART_TxCommand((u8*)ucAlarm1,strlen((char const *)ucAlarm1));
    memset(PocCmdDrvobj.GroupIdBuf,0,sizeof(PocCmdDrvobj.GroupIdBuf));
    COML_HexToAsc(PocCmdDrvobj.NameInfo.AllGroupName[groupCallingcount].ID,PocCmdDrvobj.GroupIdBuf);
    GroupIdBuf_len=strlen((char const *)PocCmdDrvobj.GroupIdBuf);
    temp_count=8-GroupIdBuf_len;
    for(i=0;i<temp_count;i++)
    {
      PocCmdDrvobj.GroupIdBuf[i+GroupIdBuf_len]=0x30;
    }
    COML_StringReverse(8,PocCmdDrvobj.GroupIdBuf);
    DrvGD83_UART_TxCommand(PocCmdDrvobj.GroupIdBuf, 8);
    DrvGD83_UART_TxCommand((u8*)ucAlarm2,strlen((char const *)ucAlarm2));
    break;
  case PocComm_Punch_the_clock_gps:
    DrvGD83_UART_TxCommand((u8*)ucPunch_the_clock_gps,strlen((char const *)ucPunch_the_clock_gps));
#if 1 //��γ��С��λ�ϲ����㼰��������
    PocCmdDrvobj.Position.longitude_integer=beidou_longitude_degree();
    PocCmdDrvobj.Position.longitude_float = ((beidou_longitude_minute()*10000+beidou_longitude_minute())*10/6);//С��������
    PocCmdDrvobj.Position.latitude_integer = beidou_latitude_degree();//��
    PocCmdDrvobj.Position.latitude_float = (beidou_latitude_minute()*10000+beidou_latitude_second())*10/6;//С��λ�ϲ�����
#endif
    
    Digital_TO_CHAR(&gps_info_buf[0],PocCmdDrvobj.Position.longitude_integer,3);
    gps_info_buf[3] = 0x2E;
    Digital_TO_CHAR(&gps_info_buf[4],(u32)(PocCmdDrvobj.Position.longitude_float/10),5);//ת����ʽ����һ
    gps_info_buf[9] = 0x3b;
    Digital_TO_CHAR(&gps_info_buf[10],PocCmdDrvobj.Position.latitude_integer,2);
    gps_info_buf[12] = 0x2E;
    Digital_TO_CHAR(&gps_info_buf[13],(u32)(PocCmdDrvobj.Position.latitude_float/10),5);//����Longitude����+ת����ʽ����һ
    
    CHAR_TO_DIV_CHAR(gps_info_buf, PocCmdDrvobj.gps_info_report, 18);//20
    DrvGD83_UART_TxCommand(PocCmdDrvobj.gps_info_report,strlen((char const *)PocCmdDrvobj.gps_info_report));
    break;
  case PocComm_Punch_the_clock_nfc:
    break;
  default:
    break;
  }
  DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(ON);
}

//��EEPROM�ж�ȡ���ݴ���дƵ���
u8 ApiPocCmd_user_info_get(u8 ** pBuf)
{
  u8 Len=0;
  Len = Combine2Hex(PocCmdDrvobj.ReadBuffer, strlen((char const *)PocCmdDrvobj.ReadBuffer), PocCmdDrvobj.ReadBuffer2);
  *pBuf = PocCmdDrvobj.ReadBuffer2;
  return (strlen((char const *)PocCmdDrvobj.ReadBuffer2));
}

//дƵд�����ݴ���EEPROM
bool ApiPocCmd_user_info_set(u8 *pBuf, u8 len)//cTxBufΪ���ip�˺��������Ϣ
{
	bool r;
	u8 i, uRet = 0;
	//ADRLEN_INF	adr;

	for(i = 0; i < len; i++)
	{
		if(pBuf[i] == ';')
		{
			uRet++;
		}
	}
	if(uRet >= 2)
	{
		Dec2Hex(pBuf, len, PocCmdDrvobj.NetState.LoginInfo.Buf);//���յ�����ת��Ϊ�ַ���//LoginInfo.BufΪ���
		PocCmdDrvobj.NetState.LoginInfo.Msg.Bits.Len = len << 0x01;//ΪʲôҪlen*2��
		PocCmdDrvobj.NetState.LoginInfo.Msg.Bits.bSet = ON;
		//adr = CFG_GetCurAdr(ADR_IDLocalUserInfo);
		//FILE_Write(adr.Adr,adr.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                //FILE_Write(0,PocCmdDrvobj.NetState.LoginInfo.Msg.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                FILE_Write(0,200,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
		for(i = 0; i < len; i++)
		{
			PocCmdDrvobj.NetState.LoginInfo.Buf[i] = pBuf[i];
		}
		PocCmdDrvobj.NetState.LoginInfo.Msg.Bits.Len = len;
		
		//SYS_BufReset();
#if 0//WCDMA ׿�Ǵ�
		PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = 0;
#endif
		PocCmdDrvobj.NetState.Msg.Bits.bUserInfo = 3;
		PocCmdDrvobj.NetState.Msg.Bits.bUserWrite = ON;
		r = TRUE;
	}
	else
	{
		r = FALSE;
	}
	return r;
}
void ApiPocCmd_10msRenew(void)
{
  u8 ucId, Len,i,temp_id;
  u8 * pBuf;
  u32 ucNameId;
  u32 ucUserId;
  u8 temp_GroupXuhao=0;
  u8 temp_UserXuhao=0;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x02://��ȡPOC����
      break;
    case 0x09://����һ��Ⱥ��
      ucUserId =  COML_AscToHex(pBuf+2, 0x06);
      if(ucUserId==0x000000)
      {
        PocCmdDrvobj.States.current_working_status=m_group_mode;
      }
      break;
    case 0x0A://����ĳ�û�
      ucUserId =  COML_AscToHex(pBuf+2, 0x06);
      if(ucUserId==0x000000)
      {
        PocCmdDrvobj.States.current_working_status=m_personal_mode;
      }
      break;
    case 0x0B://����PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        PocCmdDrvobj.States.KeyPttState=1;//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
        PocCmdDrvobj.States.ToneState_Intermediate=TRUE;//-----------------------�ӳ�0.5s�ر�����
      }
      break;
    case 0x0C://�ж��ɿ�PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        PocCmdDrvobj.States.KeyPttState=3;//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
        PocCmdDrvobj.States.ToneState_Intermediate=TRUE;
      }
      break;
    case 0x0D://��ȡȺ����Ϣ
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(PocCmdDrvobj.getting_group_all_done_flag==1)
      {
        PocCmdDrvobj.group_num_max=ucId-1;
        PocCmdDrvobj.getting_group_all_done_flag=2;
      }

      break;
    case 0x0e://��ȡ���Ա��Ϣ
      if(PocCmdDrvobj.getting_user_all_done_flag==2)//�����0eʱ���������
      {
        PocCmdDrvobj.all_user_num=COML_AscToHex(pBuf+10,0x02);
        PocCmdDrvobj.getting_user_all_done_flag=3;
        if(no_online_user()==TRUE)
        {
          VOICE_Play(NoOnlineUser);//�����߳�Ա
          return_group_and_clear_flag();//������б�־λ����Ĭ��Ⱥ��״̬
        }
      }
      else if(PocCmdDrvobj.getting_user_all_done_flag==1)
      {
        if(PocCmdDrvobj.all_user_num==0)
        {
          PocCmdDrvobj.getting_user_all_done_flag=0;
          VOICE_Play(NoOnlineUser);//�����߳�Ա
          return_group_and_clear_flag();//������б�־λ����Ĭ��Ⱥ��״̬
        }
      }
      else
      {
        
      }

      break;
    case 0x11://�ϱ���γ��
      break;
    case 0x12://����URL����
      break;
    case 0x13://��ȡ���Ա����
      break;
    case 0x16://���ú���ʱ������
      break;
    case 0x28://��ȡ����ʱ��
      break;
    case 0x80://Ⱥ���б�
      ucNameId=COML_AscToHex(pBuf+12,0x08);
      if(ucNameId==0xffff)
      {}
      else
      {
        PocCmdDrvobj.getting_group_all_done_flag=1;
        PocCmdDrvobj.GroupXuhao=COML_AscToHex(pBuf+8,0x04);
        switch(PocCmdDrvobj.getting_info_flag)
        {
        case KEYUP:
          PocCmdDrvobj.group_list_count=(u8)(GroupCallingNum/APIPOC_Group_Num);//�õ�������Ӧ�û�ȡ�����б�(0:1-5 1:6-10 2:11-15)
          break;
        case KEYDOWN:
          if(GroupCallingNum==GetAllGroupNum()&&GetAllGroupNum()%APIPOC_Group_Num!=0)//��������Ⱥ�������ܱ�����ʱ����ȡȺ�б��������
          {
            PocCmdDrvobj.group_list_count=(u8)(GroupCallingNum/APIPOC_Group_Num);//�õ�������Ӧ�û�ȡ�����б�(0:1-5 1:6-10 2:11-15)
          }
          else
          {
            PocCmdDrvobj.group_list_count=(u8)(GroupCallingNum/APIPOC_Group_Num)-1;//�õ�������Ӧ�û�ȡ�����б�(0:1-5 1:6-10 2:11-15)
          }
          break;
        default:
          break;
        }
        
        
        if(PocCmdDrvobj.GroupXuhao>PocCmdDrvobj.group_list_count*APIPOC_Group_Num
         &&PocCmdDrvobj.GroupXuhao<=(PocCmdDrvobj.group_list_count+1)*APIPOC_Group_Num)//���������Ҫ��Χ�ڣ��򱣴�Ⱥ����
        {
          temp_GroupXuhao=PocCmdDrvobj.GroupXuhao-PocCmdDrvobj.group_list_count*APIPOC_Group_Num;//temp_GroupXuhao��Χ0-5
          switch(AtCmdDrvobj.language_set)
          {
          case m_CHINESE:
            PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].ID=ucNameId;//����Ⱥ��ID����[0]��ʼ��
            if(Len >= 24)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen= (Len-24);//
              if(PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen > APIPOC_GroupName_Len)
              {
                PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen = APIPOC_GroupName_Len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen = 0x00;
            }
            for(i = 0x00; i<PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen; i++)
            {
              PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].Name[i] = pBuf[i+24];//�����ȡ��Ⱥ����
            }
            break;
          case m_ENGLISH:
            PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].ID=ucNameId;//����Ⱥ��ID����[0]��ʼ��
            if(Len >= 24)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen= (Len-24)/2;//Ӣ���ַ�ֻ��һ��
              if(PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen > APIPOC_GroupName_Len)
              {
                PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen = APIPOC_GroupName_Len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen = 0x00;
            }
            for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].NameLen; i++)
            {
              PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].Name[2*i] = pBuf[4*i+24];//�����ȡ��Ⱥ����
              PocCmdDrvobj.NameInfo.AllGroupName[temp_GroupXuhao-1].Name[2*i+1] = pBuf[4*i+1+24];
            }
            break;
          }
        }
      }
      break;
    case 0x81://���Ա�б�
      ucId=COML_AscToHex(pBuf+2, 0x02);
      PocCmdDrvobj.getting_user_all_done_flag=2;
      if(ucId==0x01)//�����Ա�������򲻻�ȡȺ����
      {
        PocCmdDrvobj.offline_user_count++;
      }
      else
      {
        ucUserId=COML_AscToHex(pBuf+12,0x08);
        PocCmdDrvobj.UserXuhao=COML_AscToHex(pBuf+8,0x04);
        switch(PocCmdDrvobj.getting_info_flag)
        {
        case KEYUP:
          PocCmdDrvobj.user_list_count=(u8)(PersonalCallingNum/APIPOC_User_Num);//�õ�������Ӧ�û�ȡ�����б�(0:1-5 1:6-10 2:11-15)
          break;
        case KEYDOWN:
          if((PersonalCallingNum==GetAllUserNum())&&(GetAllUserNum()%APIPOC_User_Num!=0))//��������Ⱥ�������ܱ�����ʱ����ȡȺ�б��������
          {
            PocCmdDrvobj.user_list_count=(u8)(PersonalCallingNum/APIPOC_User_Num);//�õ�������Ӧ�û�ȡ�����б�(0:1-5 1:6-10 2:11-15)
          }
          else
          {
            PocCmdDrvobj.user_list_count=(u8)(PersonalCallingNum/APIPOC_User_Num)-1;//�õ�������Ӧ�û�ȡ�����б�(0:1-5 1:6-10 2:11-15)
          }
          break;
        default:
          break;
        }
        if(PocCmdDrvobj.UserXuhao>=PocCmdDrvobj.user_list_count*APIPOC_User_Num
         &&PocCmdDrvobj.UserXuhao<(PocCmdDrvobj.user_list_count+1)*APIPOC_User_Num)//���������Ҫ��Χ�ڣ��򱣴�Ⱥ����
        {
          temp_UserXuhao=PocCmdDrvobj.UserXuhao-PocCmdDrvobj.user_list_count*APIPOC_User_Num;//temp_UserXuhao ��Χ0-5
        
          PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].ID=ucUserId;//����Ⱥ��ID����[0]��ʼ��
          switch(AtCmdDrvobj.language_set)
          {
          case m_CHINESE:

            if(Len >= 20)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen= (Len-20);//
              if(PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen > APIPOC_UserName_Len)
              {
                PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen = APIPOC_UserName_Len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen = 0x00;
            }
            for(i = 0x00; i<PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen; i++)
            {
              PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].Name[i] = pBuf[i+20];//�����ȡ��Ⱥ����
            }
            break;
          case m_ENGLISH:

            if(Len >= 20)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen= (Len-20)/2;//Ӣ���ַ�ֻ��һ��
              if(PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen > APIPOC_UserName_Len)
              {
                PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen = APIPOC_UserName_Len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen = 0x00;
            }
            for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].NameLen; i++)
            {
              PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].Name[2*i] = pBuf[4*i+20];//�����ȡ��Ⱥ����
              PocCmdDrvobj.NameInfo.AllGroupUserName[temp_UserXuhao].Name[2*i+1] = pBuf[4*i+1+20];
            }
            break;
          }
          
        }
      }
      break;
    case 0x82://��¼״̬�������û���
      ucId = COML_AscToHex(pBuf+2, 0x02);
      switch(ucId)
      {
      case 0x00://����
        PocCmdDrvobj.States.PocStatus=OffLine;
        break;
      case 0x01://��½��
        PocCmdDrvobj.States.PocStatus=Landing;
        break;
      case 0x02://��¼�ɹ�
        PocCmdDrvobj.States.PocStatus=LandSuccess;
        switch(AtCmdDrvobj.language_set)
        {
        case m_CHINESE:
          if(Len >= 12)//���汾���û���
          {
            PocCmdDrvobj.NameInfo.LocalUserName.NameLen= (Len-12);//
            if(PocCmdDrvobj.NameInfo.LocalUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.LocalUserName.NameLen = APIPOC_UserName_Len;
            }
          }
          else//��Ⱥ����
          {
            PocCmdDrvobj.NameInfo.LocalUserName.NameLen = 0x00;
          }
          for(i = 0x00; i<PocCmdDrvobj.NameInfo.LocalUserName.NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.LocalUserName.Name[i] = pBuf[i+12];//�����ȡ��Ⱥ����
          }
          break;
        case m_ENGLISH:
          if(Len >= 12)//���汾���û���
          {
            PocCmdDrvobj.NameInfo.LocalUserName.NameLen= (Len-12)/2;//Ӣ���ַ�ֻ��һ��
            if(PocCmdDrvobj.NameInfo.LocalUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.LocalUserName.NameLen = APIPOC_UserName_Len;
            }
          }
          else//��Ⱥ����
          {
            PocCmdDrvobj.NameInfo.LocalUserName.NameLen = 0x00;
          }
          for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.LocalUserName.NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.LocalUserName.Name[2*i] = pBuf[4*i+12];//�����ȡ��Ⱥ����
            PocCmdDrvobj.NameInfo.LocalUserName.Name[2*i+1] = pBuf[4*i+1+12];
          }
          break;
        }
        break;
      case 0x03://ע����
        PocCmdDrvobj.States.PocStatus=Logout;
        break;
      default:
        break;
      }
      break;
    case 0x83://���ؽ����û���Ϣ
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        //�Լ��޷�����
      }
      else
      {
        //�Լ������жϽ����˽���
      }
      
      ucUserId=COML_AscToHex(pBuf+12,0x08);
      if(ucUserId==0xffffffff)
      {
        
      }
      else
      {
        switch(AtCmdDrvobj.language_set)
        {
        case m_CHINESE:
          if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen= (Len-12);//
            if(PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen = APIPOC_UserName_Len;
            }
          }
          else//��Ⱥ����
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen = 0x00;
          }
          for(i = 0x00; i<PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.Name[i] = pBuf[i+12];//����
          }
          break;
        case m_ENGLISH:
          if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen= (Len-12)/2;//Ӣ���ַ�ֻ��һ��
            if(PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen = APIPOC_UserName_Len;
            }
          }
          else//��Ⱥ����
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen = 0x00;
          }
          for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.Name[2*i] = pBuf[4*i+12];//����
            PocCmdDrvobj.NameInfo.SpeakingUserName.Name[2*i+1] = pBuf[4*i+1+12];
          }
          break;
        }
      }
      break;
    case 0x84://������ʾ��Ϣ
      break;
    case 0x85://֪ͨ��������Ա��Ϣ
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        //ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
      }
      if(ucId==0x01)
      {
        //ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
      }
      break;
    case 0x86://֪ͨ�û�����Ⱥ����Ϣ
      PocCmdDrvobj.States.first_enter_into_group_flag=TRUE;
      temp_id = COML_AscToHex(pBuf+2, 0x02);
      if(temp_id==0x01)
      {
        PocCmdDrvobj.States.current_working_status=m_personal_mode;
      }
      else
      {
        PocCmdDrvobj.States.current_working_status=m_group_mode;
      }
      
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0xff)
      {
        PocCmdDrvobj.States.GroupStats=LeaveGroup;
      }
      else
      {
        PocCmdDrvobj.States.GroupStats=EnterGroup;
        if(PocCmdDrvobj.States.current_working_status==m_group_mode)
        {
          switch(AtCmdDrvobj.language_set)
          {
          case m_CHINESE:
            PocCmdDrvobj.NameInfo.NowWorkingGroupName.ID=COML_AscToHex(pBuf+4,0x08);//����Ⱥ��ID����[0]��ʼ��
            if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen= (Len-12);
              if(PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen > APIPOC_GroupName_Len)
              {
                PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen = APIPOC_GroupName_Len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen = 0x00;
            }
            for(i = 0x00; i<PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen; i++)
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[i] = pBuf[i+12];//�����ȡ��Ⱥ����
            }
            break;
          case m_ENGLISH:
            PocCmdDrvobj.NameInfo.NowWorkingGroupName.ID=COML_AscToHex(pBuf+4,0x08);//����Ⱥ��ID����[0]��ʼ��
            if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen= (Len-12)/2;//Ӣ���ַ�ֻ��һ��
              if(PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen > APIPOC_GroupName_Len)
              {
                PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen = APIPOC_GroupName_Len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen = 0x00;
            }
            for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen; i++)
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[2*i] = pBuf[4*i+12];//�����ȡ��Ⱥ����
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[2*i+1] = pBuf[4*i+1+12];
            }
            break;
          }
        }
      }
      //MenuDisplay(Menu_RefreshAllIco);
      break;
    case 0x87://֪ͨ�û�������Ϣ
      break;
    case 0x88://֪ͨ����Ⱥ����Ϣ
      break;
    case 0x8A://֪ͨ���յ���Ϣ
      switch(AtCmdDrvobj.language_set)
      {
      case m_CHINESE:
        if(Len >= 20)//���Ⱥ��id���滹��Ⱥ����
        {
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen= (Len-20);//
          if(PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen = APIPOC_UserName_Len;
            }
        }
        else//��Ⱥ����
        {
            PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen = 0x00;
        }
        for(i = 0x00; i<PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen; i++)
        {
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[i] = pBuf[i+20];//����
        }
        break;
      case m_ENGLISH:
        if(Len >= 20)//���Ⱥ��id���滹��Ⱥ����
        {
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen= (Len-20)/2;//Ӣ���ַ�ֻ��һ��
          if(PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen = APIPOC_UserName_Len;
            }
        }
        else//��Ⱥ����
        {
            PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen = 0x00;
        }
        for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen; i++)
        {
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[2*i] = pBuf[4*i+20];//����
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[2*i+1] = pBuf[4*i+1+20];
        }
        break;
      }
      PocCmdDrvobj.States.receive_sos_statas = TRUE;
      api_lcd_pwr_on_hint(0,2,GBK,"                ");
      api_lcd_pwr_on_hint(0,2,UNICODE,GetReceiveMessagesUserNameForDisplay());
      break;
    case 0x8B://֪ͨ��Ƶ����״̬
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x02)//����tts
      {
        PocCmdDrvobj.States.ReceivedVoicePlayStates=TRUE;//���ȿ���
        PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate=FALSE;//���������ptt���ڶ������Ȳ�����
        //PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED=TRUE;//ָʾ��ʹ��
        PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay=ReceivedStartTTS;//����ͼ��/��ʾ�����û���/ʹ��
      }
      else if(ucId==0x01)
      {
        PocCmdDrvobj.States.ReceivedVoicePlayStates=TRUE;//���ȿ���
        PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate=FALSE;//���������ptt���ڶ������Ȳ�����
        PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED=TRUE;//ָʾ��ʹ��
        PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay=ReceivedStartVoice;//����ͼ��/��ʾ�����û���/ʹ��
      }
      else if(ucId==0x00)
      {
        
        PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate=TRUE;//���ȿ���ʹ��
        PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED=FALSE;//ָʾ��ʹ��
        if(PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay==ReceivedStartVoice||PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay==ReceivedBeingVoice)
          PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay=ReceivedEndVoice;//���ȿ���/����ͼ��/��ʾ�����û���/ʹ��
        else if(PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay==ReceivedStartTTS||PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay==ReceivedBeingTTS)
          PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay=ReceivedEndTTS;//���ȿ���/����ͼ��/��ʾ�����û���/ʹ��
      }
      else
      {}
      break;
    case 0x8C://֪ͨ���������ն˷�������Ϣ
      break;
    case 0x9D://�򿨻ظ�(������ϢΪ�򿨳ɹ�ԭ���ʧ��ԭ��)
      AtCmdDrvobj.punch_the_clock_gps_key_press_flag=FALSE;
      if(Len >= 4)//���Ⱥ��id���滹��Ⱥ����
      {
        PocCmdDrvobj.NameInfo.ReceiveMessagesPunchTheClock.NameLen= (Len-4);//
        if(PocCmdDrvobj.NameInfo.ReceiveMessagesPunchTheClock.NameLen > APIPOC_PunchTheClock_Messages_Len)
        {
          PocCmdDrvobj.NameInfo.ReceiveMessagesPunchTheClock.NameLen = APIPOC_PunchTheClock_Messages_Len;
        }
      }
      else//��Ⱥ����
      {
        PocCmdDrvobj.NameInfo.ReceiveMessagesPunchTheClock.NameLen = 0x00;
      }
      for(i = 0x00; i<PocCmdDrvobj.NameInfo.ReceiveMessagesPunchTheClock.NameLen; i++)
      {
        PocCmdDrvobj.NameInfo.ReceiveMessagesPunchTheClock.Name[i] = pBuf[i+4];//�����ȡ��Ⱥ����
      }
      ReceiveMessagesPunchTheClock_UTF8_to_UNICODE();
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)punch_the_clock_buf_for_Voice(),strlen((char const*)punch_the_clock_buf_for_Voice()));
      api_lcd_pwr_on_hint(0,2,UNICODE,punch_the_clock_buf_for_display());
      AtCmdDrvobj.ready_return_to_default_state_flag=TRUE;
    default:
      break;
    }
  }
}



GroupStatsType ApiPocCmd_GroupStates(void)
{
  return PocCmdDrvobj.States.GroupStats;
}
void ApiPocCmd_GroupStatesSet(GroupStatsType a)
{
  PocCmdDrvobj.States.GroupStats=a;
}
u8 ApiPocCmd_KeyPttState(void)
{
  return PocCmdDrvobj.States.KeyPttState;
}
void ApiPocCmd_SetKeyPttState(u8 i)
{
  PocCmdDrvobj.States.KeyPttState=i;
}

bool ApiPocCmd_ReceivedVoicePlayStates(void)
{
  return PocCmdDrvobj.States.ReceivedVoicePlayStates;
}
void ApiPocCmd_ReceivedVoicePlayStatesSet(bool a)
{
  PocCmdDrvobj.States.ReceivedVoicePlayStates=a;
}

ReceivedVoicePlayStatesType ApiPocCmd_ReceivedVoicePlayStatesForDisplay(void)
{
  return PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay;
}
void ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedVoicePlayStatesType a)
{
  PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay=a;
}

bool ApiPocCmd_ReceivedVoicePlayStatesIntermediate(void)//�м����
{
  return PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate;
}
void ApiPocCmd_ReceivedVoicePlayStatesIntermediateSet(bool a)//�м����
{
  PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate=a;
}

bool ApiPocCmd_ReceivedVoicePlayStatesForLED(void)
{
  return PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED;
}

bool ApiPocCmd_ToneStateIntermediate(void)//�м����
{
  return PocCmdDrvobj.States.ToneState_Intermediate;
}
void ApiPocCmd_ToneStateIntermediateSet(bool a)//�м����
{
  PocCmdDrvobj.States.ToneState_Intermediate=a;
}
bool ApiPocCmd_ToneState(void)
{
  return PocCmdDrvobj.States.ToneState;
}
void ApiPocCmd_ToneStateSet(bool a)
{
  PocCmdDrvobj.States.ToneState=a;
}

/*****Ⱥ���û�����ص��ú���****************/

u8 *GetNowWorkingGroupNameForDisplay(void)//��ǰȺ��������ʾ��
{
  u8 i;
  u8 temp_buf[APIPOC_GroupName_Len];
  
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    memset(temp_buf,0,APIPOC_GroupName_Len);
    memset(PocCmdDrvobj.NowWorkingGroupNameBuf,0,sizeof(PocCmdDrvobj.NowWorkingGroupNameBuf));
    for(i=0;4*i<PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen;i++)
    {
      temp_buf[4*i+0]=PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[4*i+2];
      temp_buf[4*i+1]=PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[4*i+3];
      temp_buf[4*i+2]=PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[4*i+0];
      temp_buf[4*i+3]=PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[4*i+1];
      PocCmdDrvobj.NowWorkingGroupNameBuf[2*i+0]=COML_AscToHex(temp_buf+(4*i), 0x02);
      PocCmdDrvobj.NowWorkingGroupNameBuf[2*i+1]=COML_AscToHex(temp_buf+(4*i)+2, 0x02);
    }
    break;
  case m_ENGLISH:
    memset(PocCmdDrvobj.NowWorkingGroupNameBuf,0,sizeof(PocCmdDrvobj.NowWorkingGroupNameBuf));
    for(i=0;2*i<=PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen;i++)
    {
      PocCmdDrvobj.NowWorkingGroupNameBuf[i]=COML_AscToHex(PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name+(2*i), 0x02);
    }
    break;
  }
  return PocCmdDrvobj.NowWorkingGroupNameBuf;
}

u8 *GetAllGroupNameForDisplay(u8 a)//����Ⱥ�飺��ʾ��
{
  u8 i;
  u8 temp_buf[APIPOC_GroupName_Len];
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    memset(temp_buf,0,APIPOC_GroupName_Len);
    memset(PocCmdDrvobj.AllGroupNameBuf,0,sizeof(PocCmdDrvobj.AllGroupNameBuf));
    for(i=0;4*i<PocCmdDrvobj.NameInfo.AllGroupName[a].NameLen;i++)
    {
      temp_buf[4*i+0]=PocCmdDrvobj.NameInfo.AllGroupName[a].Name[4*i+2];
      temp_buf[4*i+1]=PocCmdDrvobj.NameInfo.AllGroupName[a].Name[4*i+3];
      temp_buf[4*i+2]=PocCmdDrvobj.NameInfo.AllGroupName[a].Name[4*i+0];
      temp_buf[4*i+3]=PocCmdDrvobj.NameInfo.AllGroupName[a].Name[4*i+1];
      PocCmdDrvobj.AllGroupNameBuf[2*i+0]=COML_AscToHex(temp_buf+(4*i), 0x02);
      PocCmdDrvobj.AllGroupNameBuf[2*i+1]=COML_AscToHex(temp_buf+(4*i)+2, 0x02);
    }
    break;
  case m_ENGLISH:
    memset(PocCmdDrvobj.AllGroupNameBuf,0,sizeof(PocCmdDrvobj.AllGroupNameBuf));
    for(i=0;2*i<=PocCmdDrvobj.NameInfo.AllGroupName[a].NameLen;i++)
    {
      PocCmdDrvobj.AllGroupNameBuf[i]=COML_AscToHex(PocCmdDrvobj.NameInfo.AllGroupName[a].Name+(2*i), 0x02);
    }
    break;
  }
  return PocCmdDrvobj.AllGroupNameBuf;
}



u8 *GetSpeakingUserNameForDisplay(void)//˵�����û�����ʾ��
{
  u8 i;
  u8 temp_buf[APIPOC_UserName_Len];
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    memset(temp_buf,0,APIPOC_UserName_Len);
    memset(PocCmdDrvobj.SpeakingUserNameBuf,0,sizeof(PocCmdDrvobj.SpeakingUserNameBuf));
    for(i=0;4*i<PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen;i++)
    {
      temp_buf[4*i+0]=PocCmdDrvobj.NameInfo.SpeakingUserName.Name[4*i+2];
      temp_buf[4*i+1]=PocCmdDrvobj.NameInfo.SpeakingUserName.Name[4*i+3];
      temp_buf[4*i+2]=PocCmdDrvobj.NameInfo.SpeakingUserName.Name[4*i+0];
      temp_buf[4*i+3]=PocCmdDrvobj.NameInfo.SpeakingUserName.Name[4*i+1];
      PocCmdDrvobj.SpeakingUserNameBuf[2*i+0]=COML_AscToHex(temp_buf+(4*i), 0x02);
      PocCmdDrvobj.SpeakingUserNameBuf[2*i+1]=COML_AscToHex(temp_buf+(4*i)+2, 0x02);
    }
    break;
  case m_ENGLISH:
    memset(PocCmdDrvobj.SpeakingUserNameBuf,0,sizeof(PocCmdDrvobj.SpeakingUserNameBuf));
    for(i=0;2*i<=PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen;i++)
    {
      PocCmdDrvobj.SpeakingUserNameBuf[i]=COML_AscToHex(PocCmdDrvobj.NameInfo.SpeakingUserName.Name+(2*i), 0x02);
    }
    break;
  }

  return PocCmdDrvobj.SpeakingUserNameBuf;
}

u8 *GetAllUserNameForDisplay(u8 a)//�����û�����ʾ��
{
  u8 i;
  u8 temp_buf[APIPOC_UserName_Len];
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    memset(temp_buf,0,APIPOC_UserName_Len);
    memset(PocCmdDrvobj.AllUserNameBuf,0,sizeof(PocCmdDrvobj.AllUserNameBuf));
    for(i=0;4*i<PocCmdDrvobj.NameInfo.AllGroupUserName[a].NameLen;i++)
    {
      temp_buf[4*i+0]=PocCmdDrvobj.NameInfo.AllGroupUserName[a].Name[4*i+2];
      temp_buf[4*i+1]=PocCmdDrvobj.NameInfo.AllGroupUserName[a].Name[4*i+3];
      temp_buf[4*i+2]=PocCmdDrvobj.NameInfo.AllGroupUserName[a].Name[4*i+0];
      temp_buf[4*i+3]=PocCmdDrvobj.NameInfo.AllGroupUserName[a].Name[4*i+1];
      PocCmdDrvobj.AllUserNameBuf[2*i+0]=COML_AscToHex(temp_buf+(4*i), 0x02);
      PocCmdDrvobj.AllUserNameBuf[2*i+1]=COML_AscToHex(temp_buf+(4*i)+2, 0x02);
    }
    break;
  case m_ENGLISH:
    memset(PocCmdDrvobj.AllUserNameBuf,0,sizeof(PocCmdDrvobj.AllUserNameBuf));
    for(i=0;2*i<=PocCmdDrvobj.NameInfo.AllGroupUserName[a].NameLen;i++)
    {
      PocCmdDrvobj.AllUserNameBuf[i]=COML_AscToHex(PocCmdDrvobj.NameInfo.AllGroupUserName[a].Name+(2*i), 0x02);
    }
    break;
  }
  return PocCmdDrvobj.AllUserNameBuf;
}

u8 *GetLocalUserNameForDisplay(void)//�����û�����ʾ��
{
  u8 i;
  u8 temp_buf[APIPOC_UserName_Len];
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    memset(temp_buf,0,APIPOC_UserName_Len);
    memset(PocCmdDrvobj.LocalUserNameBuf,0,sizeof(PocCmdDrvobj.LocalUserNameBuf));
    for(i=0;4*i<PocCmdDrvobj.NameInfo.LocalUserName.NameLen;i++)
    {
      temp_buf[4*i+0]=PocCmdDrvobj.NameInfo.LocalUserName.Name[4*i+2];
      temp_buf[4*i+1]=PocCmdDrvobj.NameInfo.LocalUserName.Name[4*i+3];
      temp_buf[4*i+2]=PocCmdDrvobj.NameInfo.LocalUserName.Name[4*i+0];
      temp_buf[4*i+3]=PocCmdDrvobj.NameInfo.LocalUserName.Name[4*i+1];
      PocCmdDrvobj.LocalUserNameBuf[2*i+0]=COML_AscToHex(temp_buf+(4*i), 0x02);
      PocCmdDrvobj.LocalUserNameBuf[2*i+1]=COML_AscToHex(temp_buf+(4*i)+2, 0x02);
    }
    break;
  case m_ENGLISH:
    memset(PocCmdDrvobj.LocalUserNameBuf,0,sizeof(PocCmdDrvobj.LocalUserNameBuf));
    for(i=0;2*i<=PocCmdDrvobj.NameInfo.LocalUserName.NameLen;i++)
    {
      PocCmdDrvobj.LocalUserNameBuf[i]=COML_AscToHex(PocCmdDrvobj.NameInfo.LocalUserName.Name+(2*i), 0x02);
    }
    break;
  }
  return PocCmdDrvobj.LocalUserNameBuf;
}

u8 *GetReceiveMessagesUserNameForDisplay(void)//���Ͷ��ŵ��û�����ʾ��
{
  u8 i,ReceiveMessagesUserNameBufLen;
  u8 temp_buf[APIPOC_UserName_Len];

  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    memset(PocCmdDrvobj.ReceiveMessagesUserNameBuf,0,sizeof(PocCmdDrvobj.ReceiveMessagesUserNameBuf));
    for(i=0;4*i<PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen;i++)
    {
      temp_buf[4*i+0]=PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[4*i+2];
      temp_buf[4*i+1]=PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[4*i+3];
      temp_buf[4*i+2]=PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[4*i+0];
      temp_buf[4*i+3]=PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[4*i+1];
      PocCmdDrvobj.ReceiveMessagesUserNameBuf[2*i+0]=COML_AscToHex(temp_buf+(4*i), 0x02);
      PocCmdDrvobj.ReceiveMessagesUserNameBuf[2*i+1]=COML_AscToHex(temp_buf+(4*i)+2, 0x02);
    }
    ReceiveMessagesUserNameBufLen=strlen((char const*)PocCmdDrvobj.ReceiveMessagesUserNameBuf);
    if(PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen]==0
       &&PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen+1]!=0
         &&PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen+2]!=0
           &&PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen+3]!=0)//����û������滹��SOS�Ļ���0x00���0x20���ո�
    {
      PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen]=0x20;
    }
    break;
  case m_ENGLISH:
    memset(PocCmdDrvobj.ReceiveMessagesUserNameBuf,0,sizeof(PocCmdDrvobj.ReceiveMessagesUserNameBuf));
    for(i=0;2*i<=PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen;i++)
    {
      PocCmdDrvobj.ReceiveMessagesUserNameBuf[i]=COML_AscToHex(PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name+(2*i), 0x02);
    }
    ReceiveMessagesUserNameBufLen=strlen((char const*)PocCmdDrvobj.ReceiveMessagesUserNameBuf);
    if(PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen]==0
       &&PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen+1]!=0
         &&PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen+2]!=0
           &&PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen+3]!=0)//����û������滹��SOS�Ļ���0x00���0x20���ո�
    {
      PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen]=0x20;
    }
    break;
  }

  return PocCmdDrvobj.ReceiveMessagesUserNameBuf;
}

u8 GetNowWorkingGroupNameLenForDisplay(void)
{
  return PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen;
}





u8 *GetNowWorkingGroupNameForVoice(void)//��ǰȺ�飺����
{
  u8 i;
    memset(PocCmdDrvobj.NowWorkingGroupNameForVoiceBuf,0,APIPOC_GroupName_Len);
    switch(AtCmdDrvobj.language_set)
    {
    case m_CHINESE:
      for(i=0;i<PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen;i++)
      {
        PocCmdDrvobj.NowWorkingGroupNameForVoiceBuf[i]    = PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[i];
      }
      break;
    case m_ENGLISH:
      for(i=0;2*i<PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen;i++)
      {
        PocCmdDrvobj.NowWorkingGroupNameForVoiceBuf[4*i]    = PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[2*i];
        PocCmdDrvobj.NowWorkingGroupNameForVoiceBuf[4*i+1]  = PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[2*i+1];
        PocCmdDrvobj.NowWorkingGroupNameForVoiceBuf[4*i+2]  = 0x30;
        PocCmdDrvobj.NowWorkingGroupNameForVoiceBuf[4*i+3]  = 0x30;
      }
      break;
    }
  return PocCmdDrvobj.NowWorkingGroupNameForVoiceBuf;
}

u8 *GetAllGroupNameForVoice(u8 a)//����Ⱥ�飺����
{
  u8 i;
  memset(PocCmdDrvobj.AllGroupNameForVoiceBuf,0,APIPOC_GroupName_Len);
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    for(i=0;i<PocCmdDrvobj.NameInfo.AllGroupName[a].NameLen;i++)
    {
      PocCmdDrvobj.AllGroupNameForVoiceBuf[i]    = PocCmdDrvobj.NameInfo.AllGroupName[a].Name[i];
    }
    break;
  case m_ENGLISH:
    for(i=0;2*i<PocCmdDrvobj.NameInfo.AllGroupName[a].NameLen;i++)
    {
      PocCmdDrvobj.AllGroupNameForVoiceBuf[4*i]    = PocCmdDrvobj.NameInfo.AllGroupName[a].Name[2*i];
      PocCmdDrvobj.AllGroupNameForVoiceBuf[4*i+1]  = PocCmdDrvobj.NameInfo.AllGroupName[a].Name[2*i+1];
      PocCmdDrvobj.AllGroupNameForVoiceBuf[4*i+2]  = 0x30;
      PocCmdDrvobj.AllGroupNameForVoiceBuf[4*i+3]  = 0x30;
    }
    break;
  }
  return PocCmdDrvobj.AllGroupNameForVoiceBuf;
}

u8 *GetAllUserNameForVoice(u8 a)//�����û�������
{
  u8 i;
  memset(PocCmdDrvobj.AllUserNameForVoiceBuf,0,APIPOC_UserName_Len);
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    for(i=0;i<PocCmdDrvobj.NameInfo.AllGroupUserName[a].NameLen;i++)
    {
      PocCmdDrvobj.AllUserNameForVoiceBuf[i]    = PocCmdDrvobj.NameInfo.AllGroupUserName[a].Name[i];
    }
    break;
  case m_ENGLISH:
    for(i=0;2*i<PocCmdDrvobj.NameInfo.AllGroupUserName[a].NameLen;i++)
    {
      PocCmdDrvobj.AllUserNameForVoiceBuf[4*i]    = PocCmdDrvobj.NameInfo.AllGroupUserName[a].Name[2*i];
      PocCmdDrvobj.AllUserNameForVoiceBuf[4*i+1]  = PocCmdDrvobj.NameInfo.AllGroupUserName[a].Name[2*i+1];
      PocCmdDrvobj.AllUserNameForVoiceBuf[4*i+2]  = 0x30;
      PocCmdDrvobj.AllUserNameForVoiceBuf[4*i+3]  = 0x30;
    }
    break;
  }
  return PocCmdDrvobj.AllUserNameForVoiceBuf;
}

u8 *GetLocalUserNameForVoice(void)//�����û�������
{
  u8 i;
  memset(PocCmdDrvobj.LocalUserNameForVoiceBuf,0,APIPOC_UserName_Len);
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    for(i=0;i<PocCmdDrvobj.NameInfo.LocalUserName.NameLen;i++)
    {
      PocCmdDrvobj.LocalUserNameForVoiceBuf[i]    = PocCmdDrvobj.NameInfo.LocalUserName.Name[i];
    }
    break;
  case m_ENGLISH:
    for(i=0;2*i<PocCmdDrvobj.NameInfo.LocalUserName.NameLen;i++)
    {
      PocCmdDrvobj.LocalUserNameForVoiceBuf[4*i]    = PocCmdDrvobj.NameInfo.LocalUserName.Name[2*i];
      PocCmdDrvobj.LocalUserNameForVoiceBuf[4*i+1]  = PocCmdDrvobj.NameInfo.LocalUserName.Name[2*i+1];
      PocCmdDrvobj.LocalUserNameForVoiceBuf[4*i+2]  = 0x30;
      PocCmdDrvobj.LocalUserNameForVoiceBuf[4*i+3]  = 0x30;
    }
    break;
  }
  return PocCmdDrvobj.LocalUserNameForVoiceBuf;
}

//����Ⱥ��ID��ȡ��ǰ���������
u16 GetNowWorkingGroupXuhao(void)
{
  u8 i;
  
  for(i=0;i<=PocCmdDrvobj.GroupXuhao;i++)
  {
    if(PocCmdDrvobj.NameInfo.NowWorkingGroupName.ID==PocCmdDrvobj.NameInfo.AllGroupName[i].ID)
    {
      return i;
    }
  }
  return 0;
}
//��ȡȺ������������Ⱥ�����
u16 GetAllGroupNum(void)
{
  return PocCmdDrvobj.group_num_max;
}
//��ȡȺ����������������ǰ����Ⱥ��������ʱ������5
u16 GetAllGroupXuhao(void)
{
  return PocCmdDrvobj.GroupXuhao;
}

//��ȡ�û���������������û�����
u16 GetAllUserNum(void)
{
  return PocCmdDrvobj.all_user_num;
}

//��ȡ�û���������������û�����
u16 GetAllOnlineUserNum(void)
{
  return (PocCmdDrvobj.all_user_num-PocCmdDrvobj.offline_user_count);
}

//�ж��Ƿ�Ⱥ���Ա����
static bool no_online_user(void)
{
  if(PocCmdDrvobj.all_user_num!=0)
  {
    if(PocCmdDrvobj.all_user_num==PocCmdDrvobj.offline_user_count)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  return FALSE;
}

//�жϵ�ǰ����ģʽ�������������
/************************/
working_status_type get_current_working_status(void)
{
  return PocCmdDrvobj.States.current_working_status;
}

//ѡ��ǰ��ʾ������������ǵ�����ʱȺ����
void get_screen_display_group_name(void)
{
 if(PocCmdDrvobj.States.current_working_status==m_group_mode)//���ģʽ
  {
    api_lcd_pwr_on_hint(0,2,GBK,"                ");
    api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());
  }
  else //����ģʽ
  {
    DISPLAY_Show(d_individualcall);
  }
}

//���յ��Ĵ򿨸�����Ϣ��UTF-8ת��ΪUNICODE
void ReceiveMessagesPunchTheClock_UTF8_to_UNICODE(void)
{
  u8 i;
  //int num;
  char temp_buf1[APIPOC_PunchTheClock_Messages_Len/2];
  char temp_buf2[APIPOC_PunchTheClock_Messages_Len/2];
  u8 temp_buf3[5];
  memset(temp_buf1,0,sizeof(temp_buf1));
  memset(temp_buf2,0,sizeof(temp_buf2));
  
  for(i=0;2*i<PocCmdDrvobj.NameInfo.ReceiveMessagesPunchTheClock.NameLen;i++)
  {
    temp_buf1[i]=COML_AscToHex(PocCmdDrvobj.NameInfo.ReceiveMessagesPunchTheClock.Name+2*i,2);
  }
  Utf8ToUnicode(temp_buf1, temp_buf2);
  for(i=0;i<strlen(temp_buf2);i++)
  {
    memset(temp_buf3,0,sizeof(temp_buf3));
    COML_HexToAsc(temp_buf2[i],temp_buf3);
    COML_StringReverse(2,temp_buf3);
    if(temp_buf3[0]==0x00)
    {
      temp_buf3[0]='0';
    }
    if(temp_buf3[1]==0x00)
    {
      temp_buf3[0]='0';
    }
    PocCmdDrvobj.unicode_punch_the_clock_buf[2*i+0]=temp_buf3[0];
    PocCmdDrvobj.unicode_punch_the_clock_buf[2*i+1]=temp_buf3[1];
  }
}

u8 *punch_the_clock_buf_for_display(void)//����Ϣ��UTF8ת��ΪUNICODE����תΪ��ʾ����ʾ����
{
  u8 i,unicode_punch_the_clock_buf_for_display_len;
  u8 temp_buf[APIPOC_PunchTheClock_Messages_Len];
  
  memset(PocCmdDrvobj.unicode_punch_the_clock_buf_for_display,0,sizeof(PocCmdDrvobj.unicode_punch_the_clock_buf_for_display));//��ʼ������
  for(i=0;4*i<strlen((char const *)PocCmdDrvobj.unicode_punch_the_clock_buf);i++)
  {
      temp_buf[4*i+0]=PocCmdDrvobj.unicode_punch_the_clock_buf[4*i+2];
      temp_buf[4*i+1]=PocCmdDrvobj.unicode_punch_the_clock_buf[4*i+3];
      temp_buf[4*i+2]=PocCmdDrvobj.unicode_punch_the_clock_buf[4*i+0];
      temp_buf[4*i+3]=PocCmdDrvobj.unicode_punch_the_clock_buf[4*i+1];
      PocCmdDrvobj.unicode_punch_the_clock_buf_for_display[2*i+0]=COML_AscToHex(temp_buf+(4*i), 0x02);
      PocCmdDrvobj.unicode_punch_the_clock_buf_for_display[2*i+1]=COML_AscToHex(temp_buf+(4*i)+2, 0x02);
  }
    unicode_punch_the_clock_buf_for_display_len=strlen((char const*)PocCmdDrvobj.unicode_punch_the_clock_buf_for_display);
    if(PocCmdDrvobj.unicode_punch_the_clock_buf_for_display[unicode_punch_the_clock_buf_for_display_len]==0
       &&PocCmdDrvobj.unicode_punch_the_clock_buf_for_display[unicode_punch_the_clock_buf_for_display_len+1]!=0
         &&PocCmdDrvobj.unicode_punch_the_clock_buf_for_display[unicode_punch_the_clock_buf_for_display_len+2]!=0
           &&PocCmdDrvobj.unicode_punch_the_clock_buf_for_display[unicode_punch_the_clock_buf_for_display_len+3]!=0)//����û������滹��SOS�Ļ���0x00���0x20���ո�
    {
      PocCmdDrvobj.unicode_punch_the_clock_buf_for_display[unicode_punch_the_clock_buf_for_display_len]=0x20;
    }

  return PocCmdDrvobj.unicode_punch_the_clock_buf_for_display;
}

u8 *punch_the_clock_buf_for_Voice(void)
{
  return PocCmdDrvobj.unicode_punch_the_clock_buf;
}

PocStatesType poccmd_states_poc_status(void)
{
  return PocCmdDrvobj.States.PocStatus;
}

u32 poc_longitude_integer(void)
{
  return PocCmdDrvobj.Position.longitude_integer;
}

u32 poc_longitude_float(void)
{
  return PocCmdDrvobj.Position.longitude_float;
}

u32 poc_latitude_integer(void)
{
  return PocCmdDrvobj.Position.latitude_integer;
}

u32 poc_latitude_float(void)
{
  return PocCmdDrvobj.Position.latitude_float;
}

bool poc_receive_sos_statas(void)
{
  return PocCmdDrvobj.States.receive_sos_statas;
}

void set_poc_receive_sos_statas(bool a)
{
  PocCmdDrvobj.States.receive_sos_statas = a;
}

bool poc_first_enter_into_group_flag(void)
{
  return PocCmdDrvobj.States.first_enter_into_group_flag;
}

bool poc_gps_value_for_display_flag(void)
{
  return PocCmdDrvobj.States.gps_value_for_display_flag;
}