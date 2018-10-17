#ifndef __APIPOCCMD_H
#define __APIPOCCMD_H

#include "AllHead.h"

#define DrvMC8332_UseId_Len	        200//define UART Tx buffer length value
#define APIPOC_GroupName_Len            64+5//unicode只存前2位，00不存，32/2=16,屏幕最多显示16个字符
#define APIPOC_UserName_Len             64+5
#define APIPOC_Group_Num                5
#define APIPOC_User_Num                 5

typedef enum{
  OffLine       =0x00,
  Landing       =0x01,
  LandSuccess   =0x02,
  Logout        =0x03
}PocStatesType;

typedef enum{
  LeaveGroup    =0x00,
  EnterGroup    =0x01,
  InGroup       =0x02
}GroupStatsType;

typedef enum{
  KEYNONE    =0x00,
  KEYUP      =0x01,
  KEYDOWN    =0x02
}KEYUPDOWNType;

typedef enum{
  ReceivedNone          = 0x00,
  ReceivedStartVoice    = 0x01,
  ReceivedBeingVoice    = 0x02,
  ReceivedEndVoice      = 0x03,
  ReceivedStartTTS      = 0x04,
  ReceivedBeingTTS      = 0x05,
  ReceivedEndTTS        = 0x06
}ReceivedVoicePlayStatesType;


typedef enum{
  m_group_mode     =0x00,
  m_personal_mode  = 0x01
}working_status_type;

typedef struct{
  struct{
    union{
      struct{
        u16 bUserInfo	: 3;
        u16 bUserWrite	: 1;
        u16 bPocReset	: 1;
        u16 bPocOpen	: 1;
        u16 bModeChange	: 1;
        u16 bMode	: 3;
        u16 bNetStat	: 2;
        u16 bUnline	: 1;
        u16             : 1;
        u16             : 2;
      }Bits;
      u16 Byte;
    }Msg;
    struct{
      union{
        struct{
          u8 bSet	: 1;
          u8 Len	: 7;
        }Bits;
        u8 Byte;
      }Msg;
      u8 Buf[DrvMC8332_UseId_Len];
    }LoginInfo;
  }NetState;
  
  struct{
    working_status_type current_working_status;
    PocStatesType PocStatus;
    GroupStatsType GroupStats;
    u8 KeyPttState;
    bool ReceivedVoicePlayStates;
    bool ReceivedVoicePlayStates_Intermediate;//喇叭
    bool ReceivedVoicePlayStatesForLED;
    ReceivedVoicePlayStatesType ReceivedVoicePlayStatesForDisplay;
    bool ToneState;
    bool ToneState_Intermediate;
    bool receive_sos_statas;
    bool first_enter_into_group_flag;
    bool gps_value_for_display_flag;
  }States;
  struct{
/*****组名**************/
    struct{
      u32 ID;
      u8  Name[APIPOC_GroupName_Len];
      u8  NameLen;
    }AllGroupName[APIPOC_Group_Num];//所有群组成员名
    struct{
      u32 ID;
      u8  Name[APIPOC_GroupName_Len];
      u8  NameLen;
    }NowWorkingGroupName;//当前工作群组成员名
/******人名**************/
    struct{
      u32 ID;
      u8  Name[APIPOC_UserName_Len];
      u8 NameLen;
    }AllGroupUserName[APIPOC_User_Num];//群组成员名
    struct{
      u8 Name[APIPOC_UserName_Len];
      u8 NameLen;
    }LocalUserName;//本机用户名
    struct{
      u8 Name[APIPOC_UserName_Len];
      u8 NameLen;
    }SpeakingUserName;//当前说话人的名字
    struct{
      u8 Name[APIPOC_UserName_Len];
      u8 NameLen;
    }ReceiveMessagesUserName;
/**************************/
  }NameInfo;
  u8 ReadBuffer[200];//存EEPROM读取的数据使用
  u8 ReadBuffer2[100];//存EEPROM读取的数据使用
  u8 NowWorkingGroupNameBuf[APIPOC_GroupName_Len];
  u8 AllGroupNameBuf[APIPOC_GroupName_Len];
  u8 AllUserNameBuf[APIPOC_UserName_Len];
  u8 SpeakingUserNameBuf[APIPOC_UserName_Len];
  u8 LocalUserNameBuf[APIPOC_UserName_Len];
  u8 ReceiveMessagesUserNameBuf[APIPOC_UserName_Len];
  
  u8 NowWorkingGroupNameForVoiceBuf[APIPOC_GroupName_Len*2+10];
  u8 AllGroupNameForVoiceBuf[APIPOC_GroupName_Len*2+10];
  u8 AllUserNameForVoiceBuf[APIPOC_UserName_Len*2+10];
  u8 LocalUserNameForVoiceBuf[APIPOC_UserName_Len*2+10];
  u16 offline_user_count;
  u16 all_user_num;//所有成员（包括离线）
  u16 group_num_max;//群组数
  u16 GroupXuhao;
  u16 UserXuhao;
  u8 group_list_count;
  u8 user_list_count;
  u8 getting_group_all_done_flag;
  u8 getting_user_all_done_flag;
  KEYUPDOWNType getting_info_flag;
  bool first_exchange_group_flag;
  u8 GroupIdBuf[10];
  u8 UserIdBuf[8];
  u8 gps_info_report[45];
  struct{
    u32 longitude_integer ;//度
    u32 longitude_float;//小数点后的数
    u32 latitude_integer;//度
    u32 latitude_float;//小数点后的数
  }Position;
}PocCmdDrv;

typedef enum{
  PocComm_OpenPOC               = 0x00,
  PocComm_SetParam		= 0x01,
  PocComm_SetURL		= 0x02,
  PocComm_Login			= 0x03,
  PocComm_Logout		= 0x04,
  PocComm_Cancel		= 0x05,
  PocComm_ModifyPwd		= 0x06,
  PocComm_EnterGroup		= 0x09,
  PocComm_Invite                = 0x0A,
  PocComm_StartPTT		= 0x0B,
  PocComm_EndPTT		= 0x0C,
  PocComm_GroupListInfo	        = 0x0D,
  PocComm_UserListInfo          = 0x0E,
  PocComm_SetGps                = 0x0F,
  PocComm_Key			= 0x10,
  PocComm_Alarm                 = 0x11
}PocCommType;


extern PocCmdDrv PocCmdDrvobj;
extern u32 poc_longitude_integer(void);
extern u32 poc_longitude_float(void);
extern u32 poc_latitude_integer(void);
extern u32 poc_latitude_float(void);

extern GroupStatsType ApiPocCmd_GroupStates(void);
extern void ApiPocCmd_GroupStatesSet(GroupStatsType a);

extern u8 ApiPocCmd_KeyPttState(void);
extern void ApiPocCmd_SetKeyPttState(u8 i);

extern bool ApiPocCmd_ReceivedVoicePlayStates(void);
extern void ApiPocCmd_ReceivedVoicePlayStatesSet(bool a);

extern ReceivedVoicePlayStatesType ApiPocCmd_ReceivedVoicePlayStatesForDisplay(void);
extern void ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedVoicePlayStatesType a);


extern bool ApiPocCmd_ReceivedVoicePlayStatesIntermediate(void);//中间变量
extern void ApiPocCmd_ReceivedVoicePlayStatesIntermediateSet(bool a);//中间变量

extern bool ApiPocCmd_ToneStateIntermediate(void);
extern void ApiPocCmd_ToneStateIntermediateSet(bool a);

extern bool ApiPocCmd_ReceivedVoicePlayStatesForLED(void);

extern bool ApiPocCmd_ToneState(void);
extern void ApiPocCmd_ToneStateSet(bool a);

extern working_status_type get_current_working_status(void);

extern PocStatesType poccmd_states_poc_status(void);
extern bool poc_receive_sos_statas(void);
extern void set_poc_receive_sos_statas(bool a);
extern bool poc_first_enter_into_group_flag(void);
extern bool poc_gps_value_for_display_flag(void);
/*****群组用户名相关调用函数****************/
extern u8 *GetNowWorkingGroupNameForDisplay(void);
extern u8 GetNowWorkingGroupNameLenForDisplay(void);
extern u8 *GetAllGroupNameForDisplay(u8 a);
extern u8 *GetSpeakingUserNameForDisplay(void);//说话的用户：显示屏
extern u8 *GetAllUserNameForDisplay(u8 a);//所有用户：显示屏
extern u8 *GetLocalUserNameForDisplay(void);//本机用户：显示屏
extern u8 *GetReceiveMessagesUserNameForDisplay(void);//发送短信的用户：显示屏
extern u16 GetNowWorkingGroupXuhao(void);
extern u16 GetAllGroupNum(void);
extern u16 GetAllGroupXuhao(void);
extern u16 GetAllUserNum(void);
extern u16 GetAllOnlineUserNum(void);
extern u8 *GetAllGroupNameForVoice(u8 a);
extern u8 *GetAllUserNameForVoice(u8 a);//所有用户：播报
extern u8 *GetLocalUserNameForVoice(void);//本机用户：播报
extern u8 *GetNowWorkingGroupNameForVoice(void);//当前群组：播报
extern void get_screen_display_group_name(void);
/*************/
extern void ApiPocCmd_PowerOnInitial(void);
extern void ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len);
extern bool ApiPocCmd_user_info_set(u8 *pBuf, u8 len);//cTxBuf为存放ip账号密码的信息
extern u8 ApiPocCmd_user_info_get(u8 ** pBuf);
extern void ApiPocCmd_10msRenew(void);
//extern void ApiPocCmd_83_1msRenew(void);


#endif