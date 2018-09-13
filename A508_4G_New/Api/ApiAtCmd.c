#include "AllHead.h"

#if 1//MZ389
const u8 *cTxATE1               ="ATE1";
const u8 *cTxCSQ                ="AT+CSQ";
const u8 *cTxDIALMODE           ="at+dialmode=0";
const u8 *cTxCGDCONT            ="at+cgdcont?";
const u8 *cTxPOWERUP            ="at+cfun=1";
const u8 *cTxCGACT              ="at+cgact=1,1";
const u8 *cTxZGACT              ="at+zgact=1,1";
const u8 *cRequestOpenPoc       ="at+poc=000000010101";
const u8 *cTxPlayZtts           ="AT+ZTTS=";
const u8 *cTxRESET              ="at+cfun=1,1";
const u8 *cTxPOWEROFF           ="at+cfun=0,0";

const u8 *cRxZICCID             ="ZICCID: ";
const u8 *cRxCSQ                ="CSQ: ";
const u8 *cRxCREG               ="CREG: ";
const u8 *cRxCGREG              ="CGREG: ";
const u8 *cRxCEREG              ="CEREG: ";
const u8 *cRxMODE               ="^MODE: ";
const u8 *cRxZGIPDNS            ="ZGIPDNS: ";
const u8 *cRxZCONSTAT           ="ZCONSTAT: 1,1";
const u8 *cRxZLTENOCELL         ="ZLTENOCELL";
const u8 *cRxZMSRI              ="ZMSRI";
const u8 *cRxCGDCONT            ="CGDCONT:";
#else
const u8 *ucTxCIMI              ="AT+CIMI";
const u8 *ucTxZPPPOPEN          ="AT+ZPPPOPEN";
const u8 *ucTxCSQ               ="AT+CSQ?";
const u8 *ucTxPlayZtts          = "AT+ZTTS=";
const u8 *ucTxPocOpenConfig     = "at+poc=000000010001";
const u8 *ucTxReset             ="at+cfun=1,1";

const u8 *ucRxZIND      ="ZIND:8";
const u8 *ucRxCIMI      ="CIMI:";
const u8 *ucRxCMEERROR  ="CME ERROR: 10";
const u8 *ucRxZPPPSTATUS="ZPPPSTATUS: OPENED";
const u8 *ucRxCSQ = "CSQ:";
const u8 *ucRxPASTATE1 = "PASTATE:1";
const u8 *ucRxZTTS0 = "ZTTS:0";

u8 KeyDownUpChoose_GroupOrUser_Flag=0;
#endif



AtCmdDrv AtCmdDrvobj;

void ApiAtCmd_PowerOnInitial(void)
{
  AtCmdDrvobj.Msg.Byte = 0;
}

#if 1//ZM389
bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  switch(id)
  {
  case ATCOMM_ATE1:
    DrvGD83_UART_TxCommand((u8*)cTxATE1, strlen((char const*)cTxATE1));
    break;
  case ATCOMM_DIALMODE:
    DrvGD83_UART_TxCommand((u8*)cTxDIALMODE, strlen((char const*)cTxDIALMODE));
    break;
  case ATCOMM_CGDCONT:
    DrvGD83_UART_TxCommand((u8*)cTxCGDCONT, strlen((char const*)cTxCGDCONT));
    break;
  case ATCOMM_POWERUP:
    DrvGD83_UART_TxCommand((u8*)cTxPOWERUP, strlen((char const*)cTxPOWERUP));
    break;
  case ATCOMM_CGACT:
    DrvGD83_UART_TxCommand((u8*)cTxCGACT, strlen((char const*)cTxCGACT));
    break;
  case ATCOMM_ZGACT:
    DrvGD83_UART_TxCommand((u8*)cTxZGACT, strlen((char const*)cTxZGACT));
    break;
  case ATCOMM_CSQ:
    DrvGD83_UART_TxCommand((u8*)cTxCSQ, strlen((char const*)cTxCSQ));
    break;
  case ATCOMM_ZTTS:
    DrvGD83_UART_TxCommand((u8*)cTxPlayZtts, strlen((char const*)cTxPlayZtts));
    break;
  case ATCOMM_RESET:
    DrvGD83_UART_TxCommand((u8*)cTxRESET, strlen((char const*)cTxRESET));
    break;
  case ATCOMM_POWEROFF:
    DrvGD83_UART_TxCommand((u8*)cTxPOWEROFF, strlen((char const*)cTxPOWEROFF));
    break;
  case ATCOMM_Test:
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  r = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}
#else

bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  switch(id)
  {
  case ATCOMM_CIMI:
    DrvGD83_UART_TxCommand((u8*)ucTxCIMI, strlen((char const*)ucTxCIMI));
    break;
  case ATCOMM_ZPPPOPEN:
    DrvGD83_UART_TxCommand((u8*)ucTxZPPPOPEN, strlen((char const*)ucTxZPPPOPEN));
    break;
  case ATCOMM_CSQ:
    DrvGD83_UART_TxCommand((u8*)ucTxCSQ, strlen((char const*)ucTxCSQ));
    break;
  case ATCOMM_RESET:
    DrvGD83_UART_TxCommand((u8*)ucTxReset, strlen((char const*)ucTxReset));
    break;
  case ATCOMM_POWEROFF:
    DrvGD83_UART_TxCommand((u8*)cTxPOWEROFF, strlen((char const*)cTxPOWEROFF));
    break;
  case ATCOMM_Test:
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  r = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}

#endif


void ApiAtCmd_100msRenew(void)
{

}

void ApiCaretCmd_10msRenew(void)
{
  u8 * pBuf,ucRet, Len,i;
  u8 temp_buf1[10]={0};
  u8 temp_buf2[10]={0};
  while((Len = DrvMC8332_CaretNotify_Queue_front(&pBuf)) != 0)
  {
/********系统模式变化指示^MODE********************************/
    ucRet=memcmp(pBuf,cRxMODE,7);
    if(ucRet==0x00)
    {
      for(i = 0x00; i < Len; i++)
      {
        if(pBuf[i]==',')
        {
            memcpy(temp_buf1,pBuf+7,i-7);
            AtCmdDrvobj.mode_param.sys_mode=CHAR_TO_Digital(temp_buf1,i-7);
            
            memcpy(temp_buf2,pBuf+i+1,Len-i-1);
            AtCmdDrvobj.mode_param.sys_submode=CHAR_TO_Digital(temp_buf2,Len-i-1);
        }
      }
    }
/*************************************************/
  }
}

#if 1

void ApiAtCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len, i;
  u8 ccid_temp_count=0;
  u8 comma_count=0;
  u8 temp_buf1[10]={0};
  u8 temp_buf2[10]={0};
  while((Len = DrvMC8332_AtNotify_Queue_front(&pBuf)) != 0)
  {
/*****开机收到会收到ZMSRI**************/
    ucRet = memcmp(pBuf, cRxZMSRI, 5);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.Msg.Bits.bCommunicationTest=1;
    }
/****信号获取及判断CSQ-测试ok*********************/
    ucRet = memcmp(pBuf, cRxCSQ, 5);
    if(ucRet == 0x00)
    {
      for(i = 0x00; i < Len; i++)
      {
        if(pBuf[i]==',')
        {
          if(comma_count==0)
          {
            memcpy(temp_buf1,pBuf+5,i-5);
            AtCmdDrvobj.csq_param.rssi=CHAR_TO_Digital(temp_buf1,i-5);//获取当前信号强度
          }
          if(comma_count==1)
          {
            memcpy(temp_buf2,pBuf+i+1,Len-i-1);
            AtCmdDrvobj.csq_param.act=CHAR_TO_Digital(temp_buf2,Len-i-1);
          }
          comma_count++;
        }
      }
      comma_count=0;
      if(MenuMode_Flag==0)
      {
        HDRCSQSignalIcons();
      }
    }
/****获取卡号CIMI*************/
    ucRet = memcmp(pBuf, cRxZICCID, 8);
    if(ucRet==0x00)
    {
      for(i=0;i<Len-8;i++)
      {
        AtCmdDrvobj.ccid[i]=pBuf[i+8];
        if(AtCmdDrvobj.ccid[i]=='0')
        {
          ccid_temp_count++;
        }
        else
        {
          ccid_temp_count=0;
        }
      }
      if(ccid_temp_count>19)
      {
        AtCmdDrvobj.Msg.Bits.bNoSimCard=1;
        AtCmdDrvobj.Msg.Bits.bSimCardIn=0;
      }
      else
      {
        AtCmdDrvobj.Msg.Bits.bSimCardIn=1;
        AtCmdDrvobj.Msg.Bits.bNoSimCard=0;
      }
    }
/*****************************/
    ucRet = memcmp(pBuf, cRxCGDCONT, 8);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.Msg.Bits.bCGDCONT=1;
    }
/*****获取网络注册状态*********/
    ucRet = memcmp(pBuf, cRxCREG, 6);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.network_reg.creg=CHAR_TO_Digital(pBuf+6,1);
    }
    ucRet = memcmp(pBuf, cRxCGREG, 7);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.network_reg.cgreg=CHAR_TO_Digital(pBuf+7,1);
    }
    ucRet = memcmp(pBuf, cRxCEREG, 7);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.network_reg.cereg=CHAR_TO_Digital(pBuf+7,1);
    }
/*****主动上报IP地址/DNS服务器地址ZGIPDNS***************/
    ucRet = memcmp(pBuf, cRxZGIPDNS, 9);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.Msg.Bits.bZGIPDNS=1;
    }
/*****发送zgact回复RNDIS链接情况***************/
    ucRet = memcmp(pBuf, cRxZCONSTAT, 13);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.Msg.Bits.bZCONSTAT=1;
    }
/********LTE丢网指示***********************/
    ucRet = memcmp(pBuf, cRxZLTENOCELL, 10);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.Msg.Bits.bZLTENOCELL=1;
    }
/****未插卡CMEERROR**********************/

/****数据业务拨号ZPPPSTATUS*************/

/****信号获取及判断CSQ*********************/
    
/*******语音播放喇叭控制标志位*******************/

/************************/
  }
}

#else
void ApiAtCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len, i;
  while((Len = DrvMC8332_AtNotify_Queue_front(&pBuf)) != 0)
  {
/*****开机ZIND:8*********/
    ucRet = memcmp(pBuf, ucRxZIND, 6);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bStartingUp=1;
    }
/****已插卡，获取卡号CIMI*************/
    ucRet = memcmp(pBuf, ucRxCIMI, 5);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bCardIn=1;
      AtCmdDrvobj.Msg.Bits.bNoCard=0;//获取到卡号则清除无卡标志位
    }
/****未插卡CMEERROR**********************/
    ucRet = memcmp(pBuf, ucRxCMEERROR, 13);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bNoCard=1;
    }
/****数据业务拨号ZPPPSTATUS*************/
    ucRet = memcmp(pBuf, ucRxZPPPSTATUS, 18);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bPPPStatusOpen=1;
    }
/****信号获取及判断CSQ*********************/
    ucRet = memcmp(pBuf, ucRxCSQ, 4);
    if(ucRet == 0x00)
    {
      if(Len > 5)//去頭
      {
        Len -= 5;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.CSQParam.Buf[i] = pBuf[i + 5];//
       }
      AtCmdDrvobj.CSQParam.Len = i;
      AtCmdDrvobj.CSQParam.Value=CHAR_TO_Digital(AtCmdDrvobj.CSQParam.Buf,2);
    } 
/*******语音播放喇叭控制标志位*******************/
    ucRet = memcmp(pBuf, ucRxPASTATE1, 9);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bZTTSStates=1;
      AtCmdDrvobj.Msg.Bits.bZTTSStates_Intermediate = 0;//播报新语音时将中间变量清零，等待收到ztts0重新打开标志位
    }
    ucRet = memcmp(pBuf, ucRxZTTS0, 6);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bZTTSStates_Intermediate = 1;
    }
/************************/
  }
}


#endif


#if 1//

bool ApiAtCmd_PlayVoice(AtVoiceType id, u8 *buf, u8 len)
{
  u8 temp_buf[4];
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8*)cTxPlayZtts, strlen((char const *)cTxPlayZtts));
  temp_buf[0] = 0x31;	// 1
  temp_buf[1] = 0x2C;	// ,
  temp_buf[2] = 0x22;	// "
  DrvGD83_UART_TxCommand(temp_buf, 3);
  switch(id)
  {
  case ATVOICE_FreePlay :
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  temp_buf[0] = 0x22;	// "
  DrvGD83_UART_TxCommand(temp_buf, 1);
  r  = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}

#else
bool ApiAtCmd_PlayVoice(AtVoiceType id, u8 *buf, u8 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8*)ucTxPlayZtts, strlen((char const *)ucTxPlayZtts));
  AtCmdDrvobj.Buf[0] = 0x31;	// 1
  AtCmdDrvobj.Buf[1] = 0x2C;	// ,
  AtCmdDrvobj.Buf[2] = 0x22;	// "
  DrvGD83_UART_TxCommand(AtCmdDrvobj.Buf, 3);
  switch(id)
  {
  case ATVOICE_FreePlay :
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  AtCmdDrvobj.Buf[0] = 0x22;	// "
  DrvGD83_UART_TxCommand(AtCmdDrvobj.Buf, 1);
  r  = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}
#endif


#if 1//ZM389
void HDRCSQSignalIcons(void)
{
    switch(AtCmdDrvobj.csq_param.act)
    {
    case 3://GSM/GPRS模式
      api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//显示2G
      if(AtCmdDrvobj.csq_param.rssi==99||AtCmdDrvobj.csq_param.rssi==0)//当无信号时
      {
        api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0格信号
      }
      else if(AtCmdDrvobj.csq_param.rssi>=19&&AtCmdDrvobj.csq_param.rssi<22)
      {
        api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//1格信号
      }
      else if(AtCmdDrvobj.csq_param.rssi>=22&&AtCmdDrvobj.csq_param.rssi<25)
      {
        api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//2格信号
      }
      else if(AtCmdDrvobj.csq_param.rssi>=25&&AtCmdDrvobj.csq_param.rssi<28)
      {
        api_disp_icoid_output( eICO_IDSCAN, TRUE, TRUE);//3格信号
      }
      else if(AtCmdDrvobj.csq_param.rssi>=28&&AtCmdDrvobj.csq_param.rssi<31)
      {
        api_disp_icoid_output( eICO_IDSCANPA, TRUE, TRUE);//4格信号
      }
      else if(AtCmdDrvobj.csq_param.rssi>=31)//5格
      {
        api_disp_icoid_output( eICO_IDSPEAKER, TRUE, TRUE);//5格信号
      }
      else
      {
        api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0格信号
      }
      break;
    case 5://WCDMA模式
    case 15://TD-SCDMA
      api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3G图标
        AtCmdDrvobj.csq_param.rssi=AtCmdDrvobj.csq_param.rssi-100;
        if(AtCmdDrvobj.csq_param.rssi==99||AtCmdDrvobj.csq_param.rssi==0)//当无信号时
        {
          api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>0&&AtCmdDrvobj.csq_param.rssi<25)
        {
          api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//1格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>=25&&AtCmdDrvobj.csq_param.rssi<30)
        {
          api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//2格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>=30&&AtCmdDrvobj.csq_param.rssi<35)
        {
          api_disp_icoid_output( eICO_IDSCAN, TRUE, TRUE);//3格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>=35&&AtCmdDrvobj.csq_param.rssi<40)
        {
          api_disp_icoid_output( eICO_IDSCANPA, TRUE, TRUE);//4格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>=40)
        {
          api_disp_icoid_output( eICO_IDSPEAKER, TRUE, TRUE);//5格信号
        }
      break;
    case 17://LTE
        api_disp_icoid_output( eICO_IDOffStart, TRUE, TRUE);//4G图标
        AtCmdDrvobj.csq_param.rssi=AtCmdDrvobj.csq_param.rssi-100;
        if(AtCmdDrvobj.csq_param.rssi==99||AtCmdDrvobj.csq_param.rssi==0)//当无信号时
        {
          api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>0&&AtCmdDrvobj.csq_param.rssi<25)
        {
          api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//1格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>=25&&AtCmdDrvobj.csq_param.rssi<30)
        {
          api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//2格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>=30&&AtCmdDrvobj.csq_param.rssi<35)
        {
          api_disp_icoid_output( eICO_IDSCAN, TRUE, TRUE);//3格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>=35&&AtCmdDrvobj.csq_param.rssi<40)
        {
          api_disp_icoid_output( eICO_IDSCANPA, TRUE, TRUE);//4格信号
        }
        else if(AtCmdDrvobj.csq_param.rssi>=40)
        {
          api_disp_icoid_output( eICO_IDSPEAKER, TRUE, TRUE);//5格信号
        }
      break;
    default:
      break;
    }
    api_disp_all_screen_refresh();// 全屏统一刷新
  
}
#else
void HDRCSQSignalIcons(void)
{
  if(MenuMode_Flag==0)
  {
    if(ApiAtCmd_CSQValue()>=31)//5格
    {
      api_disp_icoid_output( eICO_IDSPEAKER, TRUE, TRUE);//5格信号
    }
    else if(ApiAtCmd_CSQValue()>=28&&ApiAtCmd_CSQValue()<31)
    {
      api_disp_icoid_output( eICO_IDSCANPA, TRUE, TRUE);//4格信号
    }
    else if(ApiAtCmd_CSQValue()>=25&&ApiAtCmd_CSQValue()<28)
    {
      api_disp_icoid_output( eICO_IDSCAN, TRUE, TRUE);//3格信号
    }
    else if(ApiAtCmd_CSQValue()>=22&&ApiAtCmd_CSQValue()<25)
    {
      api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//2格信号
    }
    else if(ApiAtCmd_CSQValue()>=19&&ApiAtCmd_CSQValue()<22)
    {
      api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//1格信号
    }
    else
    {
      api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0格信号
    }
    api_disp_all_screen_refresh();// 全屏统一刷新
  }
}
#endif

u32  CHAR_TO_Digital(u8 * pBuf, u8 Len)
{
	u8 i;
	u32 buf = 0;
	for(i = 0; i < Len; i++)
	{
		buf *= 10;
		buf += (pBuf[i] - 0x30);
	}
	return buf;
}
void  Digital_TO_CHAR(u8 * pBuf, u32 data, u8 Len)
{
	u8 i;
	for(i = Len-1; i != 0; i--)
	{
		pBuf[i] = data%10 + 0x30;
		data /= 10;
	}
	pBuf[0] = data+0x30;
}

void  CHAR_TO_DIV_CHAR(u8 * pPrimary, u8 * pDestination, u8 Len)
{
	u8 i, j , buf;
	for(i =0; i != Len; i++)
	{
		buf = (pPrimary[i] >> 0x04) & 0x0F;
		j = i * 2;
		if(buf < 0x0A)
		{
			pDestination[j] = buf + 0x30;
		}
		else
		{
			pDestination[j] = buf + 0x57;
		}
		buf = pPrimary[i] & 0x0F;
		if(buf < 0x0A)
		{
			pDestination [j + 1] = buf + 0x30;
		}
		else
		{
			pDestination[j + 1] = buf + 0x57;
		}
	}
}

