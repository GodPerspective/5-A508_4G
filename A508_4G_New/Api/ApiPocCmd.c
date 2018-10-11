#include "ALLHead.h"



const u8 *ucAtPocHead          = "AT+POC=";
const u8 *ucSetIPAndID         = "010000";

const u8 *ucPocOpenConfig       ="000000010101";
const u8 *ucPocOpenConfig2       ="00000001010101";


u8 *ucStartPTT                  = "0B0000";
u8 *ucEndPTT                    = "0C0000";
u8 *ucGroupListInfo             = "0d0000";
u8 *ucUserListInfo              = "0e00000000";
u8 *ucSetGPS                    = "110000";
u8 *ucAlarm1                    = "210000";
u8 *ucAlarm2                    = "00000000736f7300";
u8 *ucSetURL                    = "120000";


PocCmdDrv PocCmdDrvobj;
static bool no_online_user(void);

void ApiPocCmd_PowerOnInitial(void)
{
  PocCmdDrvobj.States.current_working_status = m_group_mode;
  PocCmdDrvobj.States.PocStatus = OffLine;
  PocCmdDrvobj.States.GroupStats = LeaveGroup;
  PocCmdDrvobj.States.KeyPttState = 0;
  PocCmdDrvobj.States.ReceivedVoicePlayStates = FALSE;
  PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate = FALSE;//喇叭
  PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED = FALSE;
  PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay = ReceivedNone;
  PocCmdDrvobj.States.ToneState = FALSE;
  PocCmdDrvobj.States.ToneState_Intermediate = FALSE;
  PocCmdDrvobj.States.first_enter_into_group_flag=FALSE;
  PocCmdDrvobj.States.gps_value_for_display_flag=FALSE;
  
  PocCmdDrvobj.offline_user_count=0;
  PocCmdDrvobj.all_user_num=0;
  PocCmdDrvobj.GroupXuhao=0;
  PocCmdDrvobj.UserXuhao=0;
  
  PocCmdDrvobj.NetState.Msg.Byte = 0x00;
  memset(PocCmdDrvobj.ReadBuffer,0,sizeof(PocCmdDrvobj.ReadBuffer));
  FILE_Read(0,200,PocCmdDrvobj.ReadBuffer);//80位
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
  case PocComm_SetParam://设置账号密码
    //ip=111.111.111.111;id=11111111111;pwd=111111;UPL=99999999999999999999999999999999999999999999999999;//会加上URL但是好像没影响所以就不管了
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
    COML_HexToAsc(PocCmdDrvobj.NameInfo.AllGroupName[GroupCallingNum].ID,PocCmdDrvobj.GroupIdBuf);
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
    DrvGD83_UART_TxCommand(ucStartPTT,strlen((char const *)ucStartPTT));
    ApiPocCmd_ToneStateSet(TRUE);
    break;
  case PocComm_EndPTT://4
    DrvGD83_UART_TxCommand(ucEndPTT,strlen((char const *)ucEndPTT));
    ApiPocCmd_ToneStateSet(TRUE);
    break;
  case PocComm_GroupListInfo://5
     DrvGD83_UART_TxCommand(ucGroupListInfo, strlen((char const *)ucGroupListInfo));
    break;
  case PocComm_UserListInfo://6
#if 1//Test OK
    PocCmdDrvobj.offline_user_count=0;//发射0E指令前清零
    DrvGD83_UART_TxCommand(ucUserListInfo, strlen((char const *)ucUserListInfo));
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
#endif
    break;
  case PocComm_SetGps:
    DrvGD83_UART_TxCommand(ucSetGPS,strlen((char const *)ucSetGPS));
#if 0 //经纬度小数位合并换算及参数传递
    PocCmdDrvobj.Position.longitude_integer=beidou_longitude_degree();
    PocCmdDrvobj.Position.longitude_float = ((beidou_longitude_minute()*10000+beidou_longitude_minute())*10/6);//小数点后的数
    PocCmdDrvobj.Position.latitude_integer = beidou_latitude_degree();//度
    PocCmdDrvobj.Position.latitude_float = (beidou_latitude_minute()*10000+beidou_latitude_second())*10/6;//小数位合并换算
#endif
    Digital_TO_CHAR(&gps_info_buf[0],PocCmdDrvobj.Position.latitude_integer,2);
    gps_info_buf[2] = 0x2E;
    Digital_TO_CHAR(&gps_info_buf[3],PocCmdDrvobj.Position.latitude_float,6);//转换格式二合一
    gps_info_buf[9] = 0x2C;
    Digital_TO_CHAR(&gps_info_buf[10],PocCmdDrvobj.Position.longitude_integer,3);
    gps_info_buf[13] = 0x2E;
    Digital_TO_CHAR(&gps_info_buf[14],PocCmdDrvobj.Position.longitude_float,6);//经度Longitude换算+转换格式二合一
    
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
    DrvGD83_UART_TxCommand(ucAlarm1,strlen((char const *)ucAlarm1));
    memset(PocCmdDrvobj.GroupIdBuf,0,sizeof(PocCmdDrvobj.GroupIdBuf));
    COML_HexToAsc(PocCmdDrvobj.NameInfo.AllGroupName[GroupCallingNum].ID,PocCmdDrvobj.GroupIdBuf);
    GroupIdBuf_len=strlen((char const *)PocCmdDrvobj.GroupIdBuf);
    temp_count=8-GroupIdBuf_len;
    for(i=0;i<temp_count;i++)
    {
      PocCmdDrvobj.GroupIdBuf[i+GroupIdBuf_len]=0x30;
    }
    COML_StringReverse(8,PocCmdDrvobj.GroupIdBuf);
    DrvGD83_UART_TxCommand(PocCmdDrvobj.GroupIdBuf, 8);
    DrvGD83_UART_TxCommand(ucAlarm2,strlen((char const *)ucAlarm2));
    break;
  default:
    break;
  }
  DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(ON);
}

//从EEPROM中读取数据传给写频软件
u8 ApiPocCmd_user_info_get(u8 ** pBuf)
{
  u8 Len=0;
  Len = Combine2Hex(PocCmdDrvobj.ReadBuffer, strlen((char const *)PocCmdDrvobj.ReadBuffer), PocCmdDrvobj.ReadBuffer2);
  *pBuf = PocCmdDrvobj.ReadBuffer2;
  return (strlen((char const *)PocCmdDrvobj.ReadBuffer2));
}

//写频写入数据存入EEPROM
bool ApiPocCmd_user_info_set(u8 *pBuf, u8 len)//cTxBuf为存放ip账号密码的信息
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
		Dec2Hex(pBuf, len, PocCmdDrvobj.NetState.LoginInfo.Buf);//将收到的数转化为字符串//LoginInfo.Buf为存放
		PocCmdDrvobj.NetState.LoginInfo.Msg.Len = len << 0x01;//为什么要len*2？
		PocCmdDrvobj.NetState.LoginInfo.Msg.bSet = ON;
		//adr = CFG_GetCurAdr(ADR_IDLocalUserInfo);
		//FILE_Write(adr.Adr,adr.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                //FILE_Write(0,PocCmdDrvobj.NetState.LoginInfo.Msg.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                FILE_Write(0,200,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
		for(i = 0; i < len; i++)
		{
			PocCmdDrvobj.NetState.LoginInfo.Buf[i] = pBuf[i];
		}
		PocCmdDrvobj.NetState.LoginInfo.Msg.Len = len;
		
		//SYS_BufReset();
#if 0//WCDMA 卓智达
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

#if 1 //WCDMA 卓智达
void ApiPocCmd_10msRenew(void)
{
  u8 ucId, Len,i,temp_id;
  u8 * pBuf;
  u32 ucNameId;
  u32 ucUserId;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x02://获取POC参数
      break;
    case 0x09://进入一个群组
      ucUserId =  COML_AscToHex(pBuf+2, 0x06);
      if(ucUserId==0x000000)
      {
        PocCmdDrvobj.States.current_working_status=m_group_mode;
      }
      break;
    case 0x0A://单呼某用户
      ucUserId =  COML_AscToHex(pBuf+2, 0x06);
      if(ucUserId==0x000000)
      {
        PocCmdDrvobj.States.current_working_status=m_personal_mode;
      }
      break;
    case 0x0B://按下PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        PocCmdDrvobj.States.KeyPttState=1;//KeyPttState 0：未按PTT 1:按下ptt瞬间  2：按住PTT状态 3：松开PTT瞬间
        PocCmdDrvobj.States.ToneState_Intermediate=TRUE;//-----------------------延迟0.5s关闭喇叭
      }
      break;
    case 0x0C://判断松开PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        PocCmdDrvobj.States.KeyPttState=3;//KeyPttState 0：未按PTT 1:按下ptt瞬间  2：按住PTT状态 3：松开PTT瞬间
        PocCmdDrvobj.States.ToneState_Intermediate=TRUE;
      }
      break;
    case 0x0D://获取群组信息
      //ucId = COML_AscToHex(pBuf+10, 0x02);
      break;
    case 0x0e://获取组成员信息
      PocCmdDrvobj.all_user_num=COML_AscToHex(pBuf+10,0x02);
      if(no_online_user()==TRUE)
      {
        VOICE_Play(NoOnlineUser);//无在线成员
        //TASK_PersonalKeyModeSet(FALSE);
      }
      break;
    case 0x11://上报经纬度
      break;
    case 0x12://设置URL参数
      break;
    case 0x13://获取组成员个数
      break;
    case 0x16://设置呼叫时间限制
      break;
    case 0x28://获取北京时间
      break;
    case 0x80://群组列表
      ucNameId=COML_AscToHex(pBuf+12,0x08);
      if(ucNameId==0xffff)
      {}
      else
      {
        switch(AtCmdDrvobj.language_set)
        {
        case m_CHINESE:
          PocCmdDrvobj.GroupXuhao=COML_AscToHex(pBuf+8,0x04);
          PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].ID=ucNameId;//保存群组ID，从[0]开始存
          if(Len >= 24)//如果群组id后面还有群组名
          {
            PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen= (Len-24);//
            if(PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen > APIPOC_GroupName_Len)
            {
              PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen = APIPOC_GroupName_Len;
            }
          }
          else//无群组名
          {
            PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen = 0x00;
          }
          for(i = 0x00; i<PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].Name[i] = pBuf[i+24];//存入获取的群组名
          }
          break;
        case m_ENGLISH:
          PocCmdDrvobj.GroupXuhao=COML_AscToHex(pBuf+8,0x04);
          PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].ID=ucNameId;//保存群组ID，从[0]开始存
          if(Len >= 24)//如果群组id后面还有群组名
          {
            PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen= (Len-24)/2;//英文字符只存一半
            if(PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen > APIPOC_GroupName_Len)
            {
              PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen = APIPOC_GroupName_Len;
            }
          }
          else//无群组名
          {
            PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen = 0x00;
          }
          for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].Name[2*i] = pBuf[4*i+24];//存入获取的群组名
            PocCmdDrvobj.NameInfo.AllGroupName[PocCmdDrvobj.GroupXuhao-1].Name[2*i+1] = pBuf[4*i+1+24];
          }
          break;
        }
      }
      break;
    case 0x81://组成员列表
      ucId=COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x01)//如果成员不在线则不获取群组名
      {
        PocCmdDrvobj.offline_user_count++;
      }
      else
      {
        switch(AtCmdDrvobj.language_set)
        {
        case m_CHINESE:
          ucUserId=COML_AscToHex(pBuf+12,0x08);
          PocCmdDrvobj.UserXuhao=COML_AscToHex(pBuf+8,0x04);
          PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].ID=ucUserId;//保存群组ID，从[0]开始存
          if(Len >= 20)//如果群组id后面还有群组名
          {
            PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen= (Len-20);//
            if(PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen = APIPOC_UserName_Len;
            }
          }
          else//无群组名
          {
            PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen = 0x00;
          }
          for(i = 0x00; i<PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].Name[i] = pBuf[i+20];//存入获取的群组名
          }
          break;
        case m_ENGLISH:
          ucUserId=COML_AscToHex(pBuf+12,0x08);
          PocCmdDrvobj.UserXuhao=COML_AscToHex(pBuf+8,0x04);
          PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].ID=ucUserId;//保存群组ID，从[0]开始存
          if(Len >= 20)//如果群组id后面还有群组名
          {
            PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen= (Len-20)/2;//英文字符只存一半
            if(PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen = APIPOC_UserName_Len;
            }
          }
          else//无群组名
          {
            PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen = 0x00;
          }
          for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].Name[2*i] = pBuf[4*i+20];//存入获取的群组名
            PocCmdDrvobj.NameInfo.AllGroupUserName[PocCmdDrvobj.UserXuhao].Name[2*i+1] = pBuf[4*i+1+20];
          }
          break;
        }
      }
      break;
    case 0x82://登录状态及本机用户名
      ucId = COML_AscToHex(pBuf+2, 0x02);
      switch(ucId)
      {
      case 0x00://离线
        PocCmdDrvobj.States.PocStatus=OffLine;
        break;
      case 0x01://登陆中
        PocCmdDrvobj.States.PocStatus=Landing;
        break;
      case 0x02://登录成功
        PocCmdDrvobj.States.PocStatus=LandSuccess;
        switch(AtCmdDrvobj.language_set)
        {
        case m_CHINESE:
          if(Len >= 12)//保存本机用户名
          {
            PocCmdDrvobj.NameInfo.LocalUserName.NameLen= (Len-12);//
            if(PocCmdDrvobj.NameInfo.LocalUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.LocalUserName.NameLen = APIPOC_UserName_Len;
            }
          }
          else//无群组名
          {
            PocCmdDrvobj.NameInfo.LocalUserName.NameLen = 0x00;
          }
          for(i = 0x00; i<PocCmdDrvobj.NameInfo.LocalUserName.NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.LocalUserName.Name[i] = pBuf[i+12];//存入获取的群组名
          }
          break;
        case m_ENGLISH:
          if(Len >= 12)//保存本机用户名
          {
            PocCmdDrvobj.NameInfo.LocalUserName.NameLen= (Len-12)/2;//英文字符只存一半
            if(PocCmdDrvobj.NameInfo.LocalUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.LocalUserName.NameLen = APIPOC_UserName_Len;
            }
          }
          else//无群组名
          {
            PocCmdDrvobj.NameInfo.LocalUserName.NameLen = 0x00;
          }
          for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.LocalUserName.NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.LocalUserName.Name[2*i] = pBuf[4*i+12];//存入获取的群组名
            PocCmdDrvobj.NameInfo.LocalUserName.Name[2*i+1] = pBuf[4*i+1+12];
          }
          break;
        }
        break;
      case 0x03://注销中
        PocCmdDrvobj.States.PocStatus=Logout;
        break;
      default:
        break;
      }
      break;
    case 0x83://返回讲话用户信息
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        //自己无法讲话
      }
      else
      {
        //自己可以中断讲话人讲话
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
          if(Len >= 12)//如果群组id后面还有群组名
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen= (Len-12);//
            if(PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen = APIPOC_UserName_Len;
            }
          }
          else//无群组名
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen = 0x00;
          }
          for(i = 0x00; i<PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.Name[i] = pBuf[i+12];//存入
          }
          break;
        case m_ENGLISH:
          if(Len >= 12)//如果群组id后面还有群组名
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen= (Len-12)/2;//英文字符只存一半
            if(PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen = APIPOC_UserName_Len;
            }
          }
          else//无群组名
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen = 0x00;
          }
          for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.SpeakingUserName.NameLen; i++)
          {
            PocCmdDrvobj.NameInfo.SpeakingUserName.Name[2*i] = pBuf[4*i+12];//存入
            PocCmdDrvobj.NameInfo.SpeakingUserName.Name[2*i+1] = pBuf[4*i+1+12];
          }
          break;
        }
      }
      break;
    case 0x84://返回提示信息
      break;
    case 0x85://通知更新组或成员信息
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
    case 0x86://通知用户进入群组信息
      PocCmdDrvobj.States.first_enter_into_group_flag=TRUE;
      temp_id = COML_AscToHex(pBuf+2, 0x02);
      if(temp_id==0x01)
      {
        PocCmdDrvobj.States.current_working_status=m_personal_mode;
      }
      else
      {
        PocCmdDrvobj.States.current_working_status=m_group_mode;
        //TASK_PersonalKeyModeSet(FALSE);//解决单呼被结束，机器不退出单呼模式的问题
        
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
            PocCmdDrvobj.NameInfo.NowWorkingGroupName.ID=COML_AscToHex(pBuf+4,0x08);//保存群组ID，从[0]开始存
            if(Len >= 12)//如果群组id后面还有群组名
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen= (Len-12);
              if(PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen > APIPOC_GroupName_Len)
              {
                PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen = APIPOC_GroupName_Len;
              }
            }
            else//无群组名
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen = 0x00;
            }
            for(i = 0x00; i<PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen; i++)
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[i] = pBuf[i+12];//存入获取的群组名
            }
            break;
          case m_ENGLISH:
            PocCmdDrvobj.NameInfo.NowWorkingGroupName.ID=COML_AscToHex(pBuf+4,0x08);//保存群组ID，从[0]开始存
            if(Len >= 12)//如果群组id后面还有群组名
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen= (Len-12)/2;//英文字符只存一半
              if(PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen > APIPOC_GroupName_Len)
              {
                PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen = APIPOC_GroupName_Len;
              }
            }
            else//无群组名
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen = 0x00;
            }
            for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.NowWorkingGroupName.NameLen; i++)
            {
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[2*i] = pBuf[4*i+12];//存入获取的群组名
              PocCmdDrvobj.NameInfo.NowWorkingGroupName.Name[2*i+1] = pBuf[4*i+1+12];
            }
            break;
          }
        }
      }
      //MenuDisplay(Menu_RefreshAllIco);
      break;
    case 0x87://通知用户名字信息
      break;
    case 0x88://通知监听群组信息
      break;
    case 0x8A://通知接收到信息
      switch(AtCmdDrvobj.language_set)
      {
      case m_CHINESE:
        if(Len >= 20)//如果群组id后面还有群组名
        {
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen= (Len-20);//
          if(PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen = APIPOC_UserName_Len;
            }
        }
        else//无群组名
        {
            PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen = 0x00;
        }
        for(i = 0x00; i<PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen; i++)
        {
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[i] = pBuf[i+20];//存入
        }
        break;
      case m_ENGLISH:
        if(Len >= 20)//如果群组id后面还有群组名
        {
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen= (Len-20)/2;//英文字符只存一半
          if(PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen > APIPOC_UserName_Len)
            {
              PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen = APIPOC_UserName_Len;
            }
        }
        else//无群组名
        {
            PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen = 0x00;
        }
        for(i = 0x00; 2*i<PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.NameLen; i++)
        {
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[2*i] = pBuf[4*i+20];//存入
          PocCmdDrvobj.NameInfo.ReceiveMessagesUserName.Name[2*i+1] = pBuf[4*i+1+20];
        }
        break;
      }
      PocCmdDrvobj.States.receive_sos_statas = TRUE;
      api_lcd_pwr_on_hint(0,2,GBK,"                ");
      api_lcd_pwr_on_hint(0,2,UNICODE,GetReceiveMessagesUserNameForDisplay());
      break;
    case 0x8B://通知音频播放状态
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x02)
      {
        PocCmdDrvobj.States.ReceivedVoicePlayStates=TRUE;//喇叭控制
        PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate=FALSE;//解决连续按ptt，第二次喇叭不出声
        PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED=TRUE;//指示灯使用
        PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay=ReceivedStartTTS;//接收图标/显示呼叫用户名/使用
      }
      else if(ucId==0x01)
      {
        PocCmdDrvobj.States.ReceivedVoicePlayStates=TRUE;//喇叭控制
        PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate=FALSE;//解决连续按ptt，第二次喇叭不出声
        PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED=TRUE;//指示灯使用
        PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay=ReceivedStartVoice;//接收图标/显示呼叫用户名/使用
      }
      else if(ucId==0x00)
      {
        
        PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate=TRUE;//喇叭控制使用
        PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED=FALSE;//指示灯使用
        if(PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay==ReceivedStartVoice||PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay==ReceivedBeingVoice)
          PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay=ReceivedEndVoice;//喇叭控制/接收图标/显示呼叫用户名/使用
        else if(PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay==ReceivedStartTTS||PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay==ReceivedBeingTTS)
          PocCmdDrvobj.States.ReceivedVoicePlayStatesForDisplay=ReceivedEndTTS;//喇叭控制/接收图标/显示呼叫用户名/使用
      }
      else
      {}
      break;
    case 0x8C://通知接收其他终端发来的消息
      break;
    default:
      break;
    }
  }
}

#else //CDMA 中兴

void ApiPocCmd_10msRenew(void)
{
  u8 ucId,i, Len;
  u8 * pBuf, ucRet;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x0A://判断讲话状态
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        if(TASK_Ptt_StartPersonCalling_Flag==TRUE)//如果按下PTT键单呼某用户
        {
          EnterPersonalCalling=TRUE;
        }
      }
      break;
    case 0x0B://判断按下PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        KeyPttState=1;//KeyPttState 0：未按PTT 1:按下ptt瞬间  2：按住PTT状态 3：松开PTT瞬间
      }
      break;
    case 0x0C://判断松开PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        KeyPttState=3;//KeyPttState 0：未按PTT 1:按下ptt瞬间  2：按住PTT状态 3：松开PTT瞬间
      }
      break;  
    case 0x0e://在线用户个数
      ucId = COML_AscToHex(pBuf+8, 0x04);
      //0e0000000007
      PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum = ucId;
      if(Len==12)
      {
        if(PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum==0)
        {
          PocNoOnlineMember_Flag=TRUE;
        }
        else
        { 
          PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum = ucId;
        }
      }
      break;
    case 0x0d://群组个数
      ucId = COML_AscToHex(pBuf+10, 0x02);
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum = ucId;
      break;
    case 0x80://获取工作组列表
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(Len >= 24)//如果群组id后面还有群组名
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 24;
      }
      else//无群组名
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+24];//存入获取的群组名
        PocCmdDrvobj.WorkState.UseState.Group[ucId].Name[i] = 
          PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.Group[ucId].NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
      if(ucId==PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum)
      {
      }
      POC_GetAllGroupNameStart_Flag=TRUE; 
      break;
    case 0x81://获取组内成员列表
      ucId = COML_AscToHex(pBuf+10, 0x02);//
      if(Len >= 20)//如果用户ID后面还有用户名
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = Len - 20;
      }
      else//无用户名
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i] = pBuf[i+20];//存入获取的群组名
        PocCmdDrvobj.WorkState.UseState.UserName[ucId].Name[i] = 
          PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.UserName[ucId].NameLen = PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen;
#if 1
          GettheOnlineMembersDone=TRUE;
#endif
      break;
    case 0x82://判断是否登录成功
      ucId = COML_AscToHex(pBuf+3, 0x01);
      if(ucId == 0x02)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x01;
        OffLineCount=0;
      }
      else if(ucId == 0x00)
      {
        OffLineCount++;
        if(OffLineCount>=5)
        {
          //ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(void*)0, 0);
          OffLineCount=0;
        }
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x00;
      }
      else
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x00;
      }
      break;
/********判断是否是被呼状态******************************/
    case 0x83:
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        //830000000001 07592875268df7533100f7530000
        //830000000001 310039003800300030003300300037003400370035000000
        //830000000001 310039003800300030003300300037003400370035000000
        if(Len >= 12)
        {
          PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = Len - 12;
#if 1//尝试解决群组内被呼显示只有8位的问题
          if(InvalidCallCount==1)
          {
            InvalidCallCount=0;
          }
          else
          {
            if(PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen > APIPOC_CalledUserName_Len)
          {
            PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = APIPOC_CalledUserName_Len;
            //解决切换群组出现话权下发指令，导致禁发 
            //PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = 0;
          }
          POC_ReceivedVoice_Flag=TRUE;
          POC_ReceivedVoiceStart_Flag=2;//0:正常 1：收到语音 2：刚开始语音
          POC_ReceivedVoiceEnd_Flag=1;//0:正常 1：收到语音 2：刚结束语音
          POC_ReceivedVoiceEndForXTSF_Flag=1;
          }

#else
          if(PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen > APIPOC_CalledUserName_Len)
          {
            //PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = APIPOC_CalledUserName_Len;
            //解决切换群组出现话权下发指令，导致禁发 
            PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = 0;
          }
          else
          {
            POC_ReceivedVoice_Flag=TRUE;
            POC_ReceivedVoiceStart_Flag=2;//0:正常 1：收到语音 2：刚开始语音
            POC_ReceivedVoiceEnd_Flag=1;//0:正常 1：收到语音 2：刚结束语音
            POC_ReceivedVoiceEndForXTSF_Flag=1;
          }
#endif
        }
        for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen; i++)
        {
          PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[i] = pBuf[i+12];//存入当前说话人的名字
        }
      }
      if(ucId == 0xff)
      {
        //Set_GreenLed(LED_ON);
        if(SwitchGroupBUG==TRUE)
        {
#if 0//解决进入个呼模式，按住PTT键不送，被呼方第一次亮绿灯能接收到语音，松开PTT绿灯常亮的问题
            POC_ReceivedVoice_Flag=FALSE;
            POC_ReceivedVoiceEnd_Flag=2;//0:正常 1：收到语音 2：刚结束语音
            POC_ReceivedVoiceEndForXTSF_Flag=2;
            POC_ReceivedVoiceStart_Flag=0;//0:正常 1：收到语音 2：刚开始语音
            api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//无发射无接收信号图标
            api_disp_all_screen_refresh();// 全屏统一刷新
            SwitchGroupBUG=FALSE;
#endif
        }
        else
        {
          POC_ReceivedVoice_Flag=FALSE;
          POC_ReceivedVoiceEnd_Flag=2;//0:正常 1：收到语音 2：刚结束语音
          POC_ReceivedVoiceEndForXTSF_Flag=2;
          //POC_ReceivedVoiceStart_Flag=0;//0:正常 1：收到语音 2：刚开始语音//尝试解决闪屏问题
        }
        //POC_ReceivedNoVoice_Flag=FALSE;
      }
      break;
/**************************************/
    case 0x8B:
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x00)//语音通话结束
      {
        ApiPocCmd_PlayReceivedVoice_Flag=FALSE;
      }
      if(ucId==0x01)//语音通话开始
      {
        ApiPocCmd_PlayReceivedVoice_Flag=TRUE;
      }
      if(ucId==0x03)//tone音
      {
        ApiPocCmd_Tone_Flag=TRUE;
#if 1//当收到Tone音，将ZTTS至0
        ApiAtCmd_ZTTS_Flag=FALSE;
#endif
      }

      break;
    case 0x86:
      //InvalidCallCount=1;
      POC_Receive86_Flag=TRUE;
/****************判断接入单呼**************************************************************/
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x0a)//接入单呼
      { 
        POC_EnterPersonalCalling_Flag=2;//用于判断进入单呼,0:正常 2：正在进入单呼 1：单呼状态
        POC_QuitPersonalCalling_Flag=1;//用于判断退出单呼
      }
      else
      {
#if 0//test测试单呼后换组不正常的BUG
        if(ucId==0x00)//收到则退出单呼（退出单呼、进组状态）
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
        }
#else
        if(POC_QuitPersonalCalling_Flag==1)//收到则退出单呼（退出单呼、进组状态）
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
        }
#endif
      }
/****************群组状态判断及群组信息获取**************************************************************/
#if 1//将群组名与单呼名分开
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId==0xff)
      {
        if(POC_EnterPersonalCalling_Flag==1)
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
          POC_AtEnterPersonalCalling_Flag=0;
          POC_AtQuitPersonalCalling_Flag=0;
        }
        if(POC_AtEnterPersonalCalling_Flag==1)
        {
          POC_AtEnterPersonalCalling_Flag=0;
          POC_AtQuitPersonalCalling_Flag=2;
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=0;
        }
        POC_EnterGroupCalling_Flag=0;//0默认 1在群组内 2正在进入
        POC_QuitGroupCalling_Flag=2;//0默认 1在群组内 2正在退出
      }
      else//正在进入群组或单呼
      {
          if(EnterPersonalCalling==TRUE)
          {
            POC_AtEnterPersonalCalling_Flag=2;
            POC_AtQuitPersonalCalling_Flag=1;
            EnterPersonalCalling=FALSE;
            TASK_Ptt_StartPersonCalling_Flag=FALSE;
          }
#if 1//解决被呼状态下换组后按PTT提示禁发，绿灯亮
          if(POC_AtEnterPersonalCalling_Flag==0)//解决单呼模式下显示个呼名前还显示一下群组名的BUG
          {
            POC_ReceivedVoice_Flag=FALSE;
            POC_ReceivedVoiceEnd_Flag=2;//0:正常 1：收到语音 2：刚结束语音
            POC_ReceivedVoiceEndForXTSF_Flag=2;
            POC_ReceivedVoiceStart_Flag=0;//0:正常 1：收到语音 2：刚开始语音
          }

#endif
        if(PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode == 0x01)//如果是进入单呼模式则86存入单呼名
        {
          POC_EnterGroupCalling_Flag=2;
          POC_QuitGroupCalling_Flag=1;
            if(Len >= 12)//如果群组id后面还有群组名
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = Len - 12;
              if(PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen > APIPOC_CalledUserName_Len)
              {
                PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = APIPOC_CalledUserName_Len;
              }
            }
            else//无群组名
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = 0x00;
            }
            for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen; i++)
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.Name[i] = pBuf[i+12];//存入获取的群组名
            }
        }
        else//进组存组名
        {
          POC_EnterGroupCalling_Flag=2;
          POC_QuitGroupCalling_Flag=1;
          PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId = ucId;
          ucId = 0x00;
          for(i = 0x00; i < 0x08; i++)
          {
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] = pBuf[i+4];
            PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Id[i] = PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i];
            if(PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] != 'f') //=f为离开群组
              ucId++;
          }
          if(ucId==0x00)//如果为指令代表离开群组
          {}
          else//r如果为在群组内
          {
            if(Len >= 12)//如果群组id后面还有群组名
            {
              PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 12;
              if(PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen > APIPOC_UserName_Len)
              {
                PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = APIPOC_UserName_Len;
              }
            }
            else//无群组名
            {
              PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
            }
            for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
            {
              PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+12];//存入获取的群组名
              PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name[i] = PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
            }
            PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
          }
        }
      }

#else
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId==0xff)
      {
        POC_EnterGroupCalling_Flag=0;//0默认 1在群组内 2正在进入
        POC_QuitGroupCalling_Flag=2;//0默认 1在群组内 2正在退出
      }
      else//正在进入群组
      {
        POC_EnterGroupCalling_Flag=2;
        POC_QuitGroupCalling_Flag=1;
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId = ucId;

      ucId = 0x00;
      for(i = 0x00; i < 0x08; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] = pBuf[i+4];
        PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Id[i] = 
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i];
      if(PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] != 'f') //=f为离开群组
        ucId++;
      }
      if(ucId==0x00)//如果为指令代表离开群组
      {
      }
      else//r如果为在群组内
      {
        if(Len >= 12)//如果群组id后面还有群组名
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 12;
        if(PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen > APIPOC_UserName_Len)
        {
          PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = APIPOC_UserName_Len;
        }
      }
      else//无群组名
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+12];//存入获取的群组名
        PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name[i] = 
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
      }
      }
#endif
      break;
    case 0x91://通知进入某种模式（进入退出一键告警、单呼模式）
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.AlarmMode = 0x00;//退出一键告警
      }
      if(ucId == 0x01)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.AlarmMode = 0x01;//进入一键告警
      }
      if(ucId == 0x02)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode = 0x01;//进入单呼
      }
      if(ucId == 0x03)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode = 0x00;//退出单呼 
        //AUDIO_IOAFPOW(ON);//打开功放，解决'组呼模式'播报问题

      }
      break;
    default:
      break;
    }
  }
}
#endif


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

bool ApiPocCmd_ReceivedVoicePlayStatesIntermediate(void)//中间变量
{
  return PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate;
}
void ApiPocCmd_ReceivedVoicePlayStatesIntermediateSet(bool a)//中间变量
{
  PocCmdDrvobj.States.ReceivedVoicePlayStates_Intermediate=a;
}

bool ApiPocCmd_ReceivedVoicePlayStatesForLED(void)
{
  return PocCmdDrvobj.States.ReceivedVoicePlayStatesForLED;
}

bool ApiPocCmd_ToneStateIntermediate(void)//中间变量
{
  return PocCmdDrvobj.States.ToneState_Intermediate;
}
void ApiPocCmd_ToneStateIntermediateSet(bool a)//中间变量
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

/*****群组用户名相关调用函数****************/

u8 *GetNowWorkingGroupNameForDisplay(void)//当前群组名：显示屏
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

u8 *GetAllGroupNameForDisplay(u8 a)//所有群组：显示屏
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



u8 *GetSpeakingUserNameForDisplay(void)//说话的用户：显示屏
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

u8 *GetAllUserNameForDisplay(u8 a)//所有用户：显示屏
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

u8 *GetLocalUserNameForDisplay(void)//本机用户：显示屏
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

u8 *GetReceiveMessagesUserNameForDisplay(void)//发送短信的用户：显示屏
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
           &&PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen+3]!=0)//如果用户名后面还有SOS的话将0x00变成0x20即空格
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
           &&PocCmdDrvobj.ReceiveMessagesUserNameBuf[ReceiveMessagesUserNameBufLen+3]!=0)//如果用户名后面还有SOS的话将0x00变成0x20即空格
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





u8 *GetNowWorkingGroupNameForVoice(void)//当前群组：播报
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

u8 *GetAllGroupNameForVoice(u8 a)//所有群组：播报
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

u8 *GetAllUserNameForVoice(u8 a)//所有用户：播报
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

u8 *GetLocalUserNameForVoice(void)//本机用户：播报
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

//根据群组ID获取当前组的组索引
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
//获取群组的最大组索引，群组个数
u16 GetAllGroupNum(void)
{
  return PocCmdDrvobj.GroupXuhao;
}

//获取用户的最大组索引，用户个数
u16 GetAllUserNum(void)
{
  return PocCmdDrvobj.UserXuhao+1;
}

//判断是否群组成员在线
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

//判断当前工作模式（单呼或组呼）
/************************/
working_status_type get_current_working_status(void)
{
  return PocCmdDrvobj.States.current_working_status;
}

//选择当前显示的是组呼名还是单呼临时群组名
void get_screen_display_group_name(void)
{
 if(PocCmdDrvobj.States.current_working_status==m_group_mode)//组呼模式
  {
    api_lcd_pwr_on_hint(0,2,GBK,"                ");
    api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());
  }
  else //单呼模式
  {
    DISPLAY_Show(d_individualcall);
  }
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