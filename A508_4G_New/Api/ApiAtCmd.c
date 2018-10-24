#include "AllHead.h"

#if 1//MZ389
const u8 *cTxATE1               ="ATE1";
const u8 *cTxCSQ                ="AT+CSQ";
const u8 *cTxDIALMODE           ="at+dialmode=0";

#if 1//test
const u8 *cTxCGDCONT_SET0        ="AT+CGDCONT=1,\"IP\",\"\"";//
const u8 *cTxCGDCONT_SET1        ="AT+CGDCONT=1,\"IP\",\"3gnet\"";
const u8 *cTxCGDCONT_SET2        ="AT+CGDCONT=1,\"IP\",\"etisalat.ae\"";//
const u8 *cTxCGDCONT_SET3       ="AT+CGDCONT=1,\"IP\",\"internet\"";//
const u8 *cTxCGDCONT_SET4        ="AT+CGDCONT=1,\"IP\",\"internet.proximus.be\"";//比利时Proximus
const u8 *cTxCGDCONT_SET5        ="AT+CGDCONT=1,\"IP\",\"\"";//
const u8 *cTxCGDCONT_SET6        ="AT+CGDCONT=1,\"IP\",\"\"";//
const u8 *cTxCGDCONT_SET7        ="AT+CGDCONT=1,\"IP\",\"\"";//
const u8 *cTxCGDCONT_SET8        ="AT+CGDCONT=1,\"IP\",\"\"";//
const u8 *cTxCGDCONT_SET9       ="AT+CGDCONT=1,\"IP\",\"\"";//
#endif
const u8 *cTxPOCID                      ="AT^POCID=2";
const u8 *cTxZICCID                     ="AT+ZICCID?";
const u8 *cTxCGDCONT_READ               ="at+cgdcont?";
const u8 *cTxPOWERUP                    ="at+cfun=1";
const u8 *cTxCGACT                      ="at+cgact=1,1";
const u8 *cTxZGACT1                     ="at+zgact=1,1";
const u8 *cTxZGACT0                     ="at+zgact=0,1";
const u8 *cTxPlayZtts                   ="AT+ZTTS=";
const u8 *cTxRESET                      ="at+cfun=1,1";
const u8 *cTxPOWEROFF                   ="at+cfun=0,0";
const u8 *cTxSetNetworkAuto             ="AT^sysconfig=2,0,1,2";
const u8 *cTxSetNetworkGsmOnly          ="AT^sysconfig=13,0,1,2";
const u8 *cTxSetNetworkWcdmaOnly        ="AT^sysconfig=14,0,1,2";

const u8 *cRxZICCID             ="ZICCID: ";
const u8 *cRxCSQ                ="CSQ: ";
const u8 *cRxCREG               ="CREG: ";
const u8 *cRxCGREG              ="CGREG: ";
const u8 *cRxCEREG              ="CEREG: ";
const u8 *cRxMODE               ="^MODE: ";
const u8 *cRxZGIPDNS            ="ZGIPDNS: 1";
const u8 *cRxZCONSTAT           ="ZCONSTAT: 1,1";
const u8 *cRxZLTENOCELL         ="ZLTENOCELL";
const u8 *cRxZMSRI              ="ZMSRI";
const u8 *cRxCGDCONT            ="CGDCONT:";
const u8 *cRxPASTATE1          ="PASTATE:1";
const u8 *cRxZTTS0             ="ZTTS:0";
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
  AtCmdDrvobj.ZLTENOCELL=0;
  AtCmdDrvobj.ZGIPDNS=0;
  AtCmdDrvobj.ZCONSTAT=0;
  AtCmdDrvobj.csq_param.act=0;
  AtCmdDrvobj.csq_param.rssi=0;
  AtCmdDrvobj.mode_param.sys_mode=0;
  AtCmdDrvobj.mode_param.sys_submode=0;
  AtCmdDrvobj.network_reg.creg=0;
  AtCmdDrvobj.network_reg.cgreg=0;
  AtCmdDrvobj.network_reg.cereg=0;
  memset(AtCmdDrvobj.ccid,0,sizeof(AtCmdDrvobj.ccid));
  AtCmdDrvobj.apn_set=0;
  AtCmdDrvobj.language_value=0;
  AtCmdDrvobj.language_set=m_CHINESE;
  AtCmdDrvobj.Key3_PlayValue=0;
  AtCmdDrvobj.key2_long_value=0;
  AtCmdDrvobj.key3_long_value=0;
  AtCmdDrvobj.key4_long_value=0;
  AtCmdDrvobj.Key3Option=Key3_OptionZero;
  AtCmdDrvobj.punch_the_clock_gps_key_press_flag=FALSE;
  
  FILE_Read(0x230,1,&(AtCmdDrvobj.apn_set));//FILE_Read
  FILE_Read(0x23A,1,&(AtCmdDrvobj.language_value));//FILE_Read
  FILE_Read(598,1,&(AtCmdDrvobj.Key3_PlayValue));
  //FILE_Read(590,20,AtCmdDrvobj.testbuf);
  FILE_Read(599,1,&(AtCmdDrvobj.key2_long_value));//侧键1长键
  FILE_Read(597,1,&(AtCmdDrvobj.key3_long_value));//侧键2长键
  FILE_Read(601,1,&(AtCmdDrvobj.key4_long_value));//侧键2长键
  switch(AtCmdDrvobj.language_value)
  {
  case 0:
    AtCmdDrvobj.language_set=m_CHINESE;
    break;
  case 1:
    AtCmdDrvobj.language_set=m_ENGLISH;
    break;
  default:
    AtCmdDrvobj.language_set=m_CHINESE;
    break;
  }
  
#if 1//侧键1播报语音类型
  switch(AtCmdDrvobj.Key3_PlayValue)
  {
  case 0x00:
    AtCmdDrvobj.Key3Option=Key3_OptionZero;
    break;
  case 0x01:
    AtCmdDrvobj.Key3Option=Key3_OptionOne;
    break;
  case 0x02:
    AtCmdDrvobj.Key3Option=Key3_OptionTwo;
    break;
  case 0x03:
    AtCmdDrvobj.Key3Option=Key3_OptionThree;
    break;
  case 0x04:
    AtCmdDrvobj.Key3Option=Key3_OptionFour;
    break;
  default:
    break;
  }
#endif
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
  case ATCOMM_CGDCONT_SET:
    switch(AtCmdDrvobj.apn_set)
    {
    case 0:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET0, strlen((char const*)cTxCGDCONT_SET0));
      break;
    case 1:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET1, strlen((char const*)cTxCGDCONT_SET1));
      break;
    case 2:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET2, strlen((char const*)cTxCGDCONT_SET2));
      break;
    case 3:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET3, strlen((char const*)cTxCGDCONT_SET3));
      break;
    case 4:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET4, strlen((char const*)cTxCGDCONT_SET4));
      break;
    case 5:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET5, strlen((char const*)cTxCGDCONT_SET5));
      break;
    case 6:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET6, strlen((char const*)cTxCGDCONT_SET6));
      break;
    case 7:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET7, strlen((char const*)cTxCGDCONT_SET7));
      break;
    case 8:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET8, strlen((char const*)cTxCGDCONT_SET8));
      break;
    case 9:
      DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_SET9, strlen((char const*)cTxCGDCONT_SET9));
      break;
    default:
      break;
    }
    break;
  case ATCOMM_CGDCONT_READ:
    DrvGD83_UART_TxCommand((u8*)cTxCGDCONT_READ, strlen((char const*)cTxCGDCONT_READ));
    break;
  case ATCOMM_POWERUP:
    DrvGD83_UART_TxCommand((u8*)cTxPOWERUP, strlen((char const*)cTxPOWERUP));
    break;
  case ATCOMM_CGACT:
    DrvGD83_UART_TxCommand((u8*)cTxCGACT, strlen((char const*)cTxCGACT));
    AtCmdDrvobj.ZGIPDNS=1;
    break;
  case ATCOMM_ZGACT1:
    DrvGD83_UART_TxCommand((u8*)cTxZGACT1, strlen((char const*)cTxZGACT1));
    AtCmdDrvobj.ZCONSTAT=1;
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
  case ATCOMM_ZGACT0:
    DrvGD83_UART_TxCommand((u8*)cTxZGACT0, strlen((char const*)cTxZGACT0));
    break;
  case ATCOMM_SetNetworkAuto:
    DrvGD83_UART_TxCommand((u8*)cTxSetNetworkAuto, strlen((char const*)cTxSetNetworkAuto));
    break;
  case ATCOMM_SetNetworkGsmOnly:
    DrvGD83_UART_TxCommand((u8*)cTxSetNetworkGsmOnly, strlen((char const*)cTxSetNetworkGsmOnly));
    break;
  case ATCOMM_SetNetworkWcdmaOnly:
    DrvGD83_UART_TxCommand((u8*)cTxSetNetworkWcdmaOnly, strlen((char const*)cTxSetNetworkWcdmaOnly));
    break;
  case ATCOMM_ZICCID:
    DrvGD83_UART_TxCommand((u8*)cTxZICCID, strlen((char const*)cTxZICCID));
    break;
  case ATCOMM_POCID:
    DrvGD83_UART_TxCommand((u8*)cTxPOCID, strlen((char const*)cTxPOCID));
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
      if(pBuf[7]=='0')//^MODE: 0
      {
        AtCmdDrvobj.mode_param.sys_mode=0x18;//自定义0x18为无服务
        AtCmdDrvobj.mode_param.sys_submode=0x00;
      }
      else
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
      if(MenuMode_Flag==0)
      {
        NetworkModeIcons();
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
      if(AtCmdDrvobj.Msg.Bits.bCommunicationTest==1)
      {
        TaskDrvobj.Id = TASK_LOGIN;
      }
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
    ucRet = memcmp(pBuf, cRxZGIPDNS, 10);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.ZGIPDNS=2;
    }
/*****发送zgact回复RNDIS链接情况***************/
    ucRet = memcmp(pBuf, cRxZCONSTAT, 13);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.ZCONSTAT=2;
      AtCmdDrvobj.network_reg.creg=0;
      AtCmdDrvobj.network_reg.cereg=0;
      AtCmdDrvobj.network_reg.cgreg=0;
    }
/********LTE丢网指示***********************/
    ucRet = memcmp(pBuf, cRxZLTENOCELL, 10);
    if(ucRet==0x00)
    {
      AtCmdDrvobj.ZLTENOCELL=1;
    }
/*******语音播放喇叭控制标志位*******************/
    ucRet = memcmp(pBuf, cRxPASTATE1, 9);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bZTTSStates=1;
      AtCmdDrvobj.Msg.Bits.bZTTSStates_Intermediate = 0;//播报新语音时将中间变量清零，等待收到ztts0重新打开标志位
    }
    ucRet = memcmp(pBuf, cRxZTTS0, 6);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bZTTSStates_Intermediate = 1;
    }
/****未插卡CMEERROR**********************/

/****数据业务拨号ZPPPSTATUS*************/

/****信号获取及判断CSQ*********************/
    

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
void NetworkModeIcons(void)
{
  switch(AtCmdDrvobj.mode_param.sys_submode)
  {
  case 0:
    api_disp_icoid_output( eICO_IDSPEAKEROff, TRUE, TRUE);//显示× 无服务
    break;
  case 1:
  case 2:
    api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//显示2G 
    break;
  case 3:
    api_disp_icoid_output( eICO_IDVOXOff, TRUE, TRUE);//显示E
    break;
  case 4:
    api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//显示3G
    break;
  case 5:
  case 6:
  case 7:
    api_disp_icoid_output( eICO_IDSCANOff, TRUE, TRUE);//显示H
    break;
  case 8:
    api_disp_icoid_output( eICO_IDTALKAROff, TRUE, TRUE);//显示T
    break;
  case 9:
  case 10:
    api_disp_icoid_output( eICO_IDOffStart, TRUE, TRUE);//显示4G
    break;
  case 11:
    api_disp_icoid_output( eICO_IDLOCKEDOff, TRUE, TRUE);//显示H+
    break;
  default:
    break;
  }
}
void HDRCSQSignalIcons(void)
{
    switch(AtCmdDrvobj.csq_param.act)
    {
    case 0:
      api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0格信号
      break;
    case 3://GSM/GPRS模式
      
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
u16 ApiAtCmd_bZTTSStates(void)
{
  return AtCmdDrvobj.Msg.Bits.bZTTSStates;
}
void set_ApiAtCmd_bZTTSStates(u16 a)
{
  AtCmdDrvobj.Msg.Bits.bZTTSStates = a;
}
u16 ApiAtCmd_bZTTSStates_Intermediate(void)
{
  return AtCmdDrvobj.Msg.Bits.bZTTSStates_Intermediate;
}
void set_ApiAtCmd_bZTTSStates_Intermediate(u16 a)
{
  AtCmdDrvobj.Msg.Bits.bZTTSStates_Intermediate = a;
}
