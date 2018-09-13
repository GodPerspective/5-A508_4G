#ifndef __APIATCMD_H
#define __APIATCMD_H

#include "AllHead.h"

typedef struct{
  union{
    struct{
      u16  bCommunicationTest    :1;
      u16  bSimCardIn            :1;
      u16  bNoSimCard            :1;
      u16  bCGDCONT              :1;
      u16  bZGIPDNS              :1;
      u16  bZCONSTAT             :1;
      u16  bZLTENOCELL           :1;
      u16                        :9;
    }Bits;
    u16 Byte;
  }Msg;
  struct{
    u8 rssi;//�ź�ǿ��ָʾ
    u8 act;//�źŶ�Ӧ�ĵ�ǰ��ʽ
  }csq_param;
  struct{
    u8 sys_mode;
    u8 sys_submode;
  }mode_param;
  struct{
    u8 creg;
    u8 cgreg;
    u8 cereg;
  }network_reg;//����ע��״̬CREG/CGREG/CEREG
  u8 ccid[20];
}AtCmdDrv;

typedef enum{
  ATCOMM_ATE1                   = 0x00,
  ATCOMM_DIALMODE               = 0x01,
  ATCOMM_CGDCONT                = 0x02,
  ATCOMM_POWERUP                = 0x03,
  ATCOMM_CGACT                  = 0x04,
  ATCOMM_ZGACT                  = 0x05,
  ATCOMM_CSQ                    = 0x06,
  ATCOMM_ZTTS                   = 0x07,
  ATCOMM_RESET                  = 0x08,
  ATCOMM_POWEROFF               = 0x09,
  ATCOMM_Test                   = 0x0A
}AtCommType;


extern AtCmdDrv AtCmdDrvobj;

#if 1//WCDMA ׿�Ǵ�
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


extern u8 HDRCSQValue;//HDRCSQ��ֵ
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