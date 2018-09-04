#ifndef __APIATCMD_H
#define __APIATCMD_H

#include "AllHead.h"

typedef enum{
  ATCOMM_CIMI                   = 0x00,
  ATCOMM_ZPPPOPEN               = 0x01,
  ATCOMM_CSQ                    = 0x02,
  ATCOMM_RESET                  = 0x03,
  ATCOMM_Test                   = 0x04
}AtCommType;


#if 1//WCDMA ×¿ÖÇ´ï
extern void ApiAtCmd_PowerOnInitial(void);
extern u16 ApiAtCmd_bStartingUp(void);
extern u16 ApiAtCmd_bCardIn(void);
extern u16 ApiAtCmd_bNoCard(void);
extern u16 ApiAtCmd_bPPPStatusOpen(void);
extern u8 ApiAtCmd_CSQValue(void);
extern u16   ApiAtCmd_bZTTSStates(void);
extern void set_ApiAtCmd_bZTTSStates(u16 a);
extern u16   ApiAtCmd_bZTTSStates_Intermediate(void);
extern void set_ApiAtCmd_bZTTSStates_Intermediate(u16 a);
#endif


extern u8 HDRCSQValue;//HDRCSQµÄÖµ
extern u8 BootProcess_SIMST_Flag;
extern u8 BootProcess_PPPCFG_Flag;
extern u8 ApiAtCmd_TrumpetVoicePlay_Flag;
extern u8 KeyDownUpChoose_GroupOrUser_Flag;
extern u8 CSQ_Flag;
extern bool PositionInfoSendToATPORT_RedLed_Flag;
extern bool PositionInfoSendToATPORT_InfoDisplay_Flag;


extern bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len);
extern bool ApiAtCmd_PlayVoice(AtVoiceType id, u8 *buf, u8 len);
extern void HDRCSQSignalIcons(void);
extern void ApiAtCmd_100msRenew(void);
extern void ApiCaretCmd_10msRenew(void);
extern void ApiAtCmd_10msRenew(void);
extern u8 ApiAtCmd_GetIccId(u8 **pBuf);
extern void ApiGetIccidBuf(void);
extern u8 ApiAtCmd_Ppp_state(void);

extern u32  CHAR_TO_Digital(u8 * pBuf, u8 Len);
extern void  Digital_TO_CHAR(u8 * pBuf, u32 data, u8 Len);
extern void  CHAR_TO_DIV_CHAR(u8 * pPrimary, u8 * pDestination, u8 Len);

#endif