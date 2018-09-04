#include "AllHead.h"

#if 1//MZ389
const u8 *cTxATE1               ="ATE1";
const u8 *cTxCSQ                ="AT+CSQ";
const u8 *cRequestOpenPoc       ="at+poc=000000010101";
const u8 *cTxPlayZtts          ="AT+ZTTS=";

const u8 *cRxZICCID             ="+ZICCID:";

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

typedef struct{
  union{
    struct{
      u16  bCommunicationTest    :1;
      u16  bSimCardIn            :1;
      u16                        :14;
    }Bits;
    u16 Byte;
  }Msg;
}AtCmdDrv;

AtCmdDrv AtCmdDrvobj;

void ApiAtCmd_PowerOnInitial(void)
{
  AtCmdDrvobj.Msg.Byte = 0;
}


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


void ApiAtCmd_100msRenew(void)
{
  if(GetTaskId()==Task_NormalOperation)//登录成功进入群组状态
    {
    }
}

void ApiCaretCmd_10msRenew(void)
{
  u8 * pBuf, Len;
  while((Len = DrvMC8332_CaretNotify_Queue_front(&pBuf)) != 0)
  {
  }
}

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

u16 ApiAtCmd_bStartingUp(void)
{
  return AtCmdDrvobj.Msg.Bits.bStartingUp;
}
u16 ApiAtCmd_bCardIn(void)
{
  return AtCmdDrvobj.Msg.Bits.bCardIn;
}
u16 ApiAtCmd_bNoCard(void)
{
  return AtCmdDrvobj.Msg.Bits.bNoCard;
}
u16 ApiAtCmd_bPPPStatusOpen(void)
{
  return AtCmdDrvobj.Msg.Bits.bPPPStatusOpen;
}
u8 ApiAtCmd_CSQValue(void)
{
  return AtCmdDrvobj.CSQParam.Value;
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