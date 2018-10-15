#include "AllHead.h"




const u8 *ucPersonalMode                = "2a4e7c54216a0f5f";//个呼模式-
const u8 *ucABELL                       = "276b0768f95bb28b3a67";//欧标对讲
const u8 *ucGroupSelected               = "f25d09902d4e";//已选中-
const u8 *ucNoSimCard                   = "f78bd2636153";//请插卡
const u8 *ucNetworkSearching            = "1c64227d517fdc7e";//搜索网络-
const u8 *ucPowerLowPleaseCharge        = "3575cf914e4f0cfff78b45513575";//电量低，请充电-
const u8 *ucLowBattery                  = "3575cf914e4f";//电量低-
const u8 *ucFivePercentPower            = "3575cf917e7606524b4e944e";//百分之五-
const u8 *ucTwentyPercentPower          = "3575cf917e7606524b4e8c4e4153";//百分之二十-
const u8 *ucFortyPercentPower           = "3575cf917e7606524b4edb564153";//百分之四十-
const u8 *ucSixtyPercentPower           = "3575cf917e7606524b4e6d514153";//百分之六十-
const u8 *ucEightyPercentPower          = "3575cf917e7606524b4e6b514153";//百分之八十-
const u8 *ucOneHundredPercentPower      = "3575cf917e7606524b4e004e7e76";//百分之一百-
const u8 *ucGroupMode                   = "c47e7c54216a0f5f";//组呼模式-
const u8 *ucHandsetMode                 = "2c54527b216a0f5f";//听筒模式-
const u8 *ucHandfreeMode                = "4d51d063216a0f5f";//免提模式-
const u8 *ucNoOnlineUser                = "e0652857bf7e10625854";//无在线成员
const u8 *ucLoggingIn                   = "636b28577b764696";//正在登陆- 

const u8 *ucSimCardError                = "6153025f385e";//卡异常
const u8 *ucCs_unknow_network           = "430053002a67e577517fdc7e";//CS未知网络
const u8 *ucGprs_unknow_network         = "67007000720073002a67e577517fdc7e";//GPRS未知网络
const u8 *ucGprs_refuse_enroll          = "6700700072007300e86c8c51ab88d262dd7e";//GPRS注册被拒绝 
const u8 *ucEps_unknow_network          = "4500500053002a67e577517fdc7e";//EPS未知网络 
const u8 *ucEps_refuse_enroll           = "450050005300e86c8c51ab88d262dd7e";//EPS注册被拒绝 
const u8 *ucAll_unknow_network          = "6300730040620967517fdc7e025f385e";//所有网络异常
const u8 *ucNo_service                  = "e0650d67a152";//无服务
const u8 *ucSet_network_auto            = "517fdc7ebe8b6e7f3a4eea81a852216a0f5f";//网络设置为自动模式
const u8 *ucSet_network_wcdma_only      = "517fdc7ebe8b6e7f3a4ec54e5096094ec45b";//网络设置为仅限3G
const u8 *ucSet_network_gsm_only        = "517fdc7ebe8b6e7f3a4ec54e50968c4ec45b";//网络设置为仅限2G

const u8 *ucPersonalMode2                = "50006500720073006f006e0061006c0020004d006f0064006500";//personal mode
const u8 *ucABELL2                       = "4100620065006C006C00";//ABELL
const u8 *ucGroupSelected2               = "530065006c0065006300740065006400";//Group Selected
const u8 *ucNoSimCard2                   = "6e006f002000730069006D0020006300610072006400";//NO SIM Card
const u8 *ucNetworkSearching2            = "6e006500740077006f0072006b002000730065006100720063006800";//network searching
const u8 *ucPowerLowPleaseCharge2        = "50006f0077006500720020006c006f0077002c0050006c0065006100730065002000430068006100720067006500";//Power Low Please Charge
const u8 *ucLowBattery2                  = "4c006f00770020004200610074007400650072007900";//Low Battery
const u8 *ucFivePercentPower2            = "50006f00770065007200200046006900760065002000500065007200630065006e007400";//Five Percent Power
const u8 *ucTwentyPercentPower2          = "50006f0077006500720020005400770065006e00740079002000500065007200630065006e007400";
const u8 *ucFortyPercentPower2           = "50006f00770065007200200046006f007200740079002000500065007200630065006e007400";
const u8 *ucSixtyPercentPower2           = "50006f007700650072002000530069007800740079002000500065007200630065006e007400";
const u8 *ucEightyPercentPower2          = "50006f0077006500720020004500690067006800740079002000500065007200630065006e007400";
const u8 *ucOneHundredPercentPower2      = "50006f0077006500720020004f006e0065002000480075006e0064007200650064002000500065007200630065006e007400";
const u8 *ucGroupMode2                   = "470072006f007500700020004d006f0064006500";
const u8 *ucHandsetMode2                 = "480061006e006400200073006500740020004d006f0064006500";
const u8 *ucHandfreeMode2                = "480061006e0064002000660072006500650020004d006f0064006500";
const u8 *ucNoOnlineUser2                = "4e006f0020006f006e006c0069006e00650020007500730065007200";
const u8 *ucLoggingIn2                   = "4100630063006f0075006e007400200043006f006e00660069006700750072006100740069006f006e00";//Account Config

const u8 *ucSimCardError2                = "530049004d002000630061007200640020006500720072006f007200";//SIM card error
const u8 *ucCs_unknow_network2           = "4300530020006e006500740077006f0072006b00200075006e006b006e006f007700";//CS network unknow
const u8 *ucGprs_unknow_network2         = "470050005200530020006e006500740077006f0072006b00200075006e006b006e006f007700";//GPRS network unknow
const u8 *ucGprs_refuse_enroll2          = "470050005200530020006e006500740077006f0072006b00200075006e006b006e006f007700";//GPRS Signup Fail
const u8 *ucEps_unknow_network2          = "45005000530020006e006500740077006f0072006b00200075006e006b006e006f007700";//EPS network unknow
const u8 *ucEps_refuse_enroll2           = "45005000530020005300690067006e007500700020004600610069006c00";//EPS Signup Fail
const u8 *ucAll_unknow_network2          = "41006c006c0020006e006500740077006f0072006b00200075006e006b006e006f007700";//All network unknow
const u8 *ucNo_service2                  = "4e006f0020007300650072007600690063006500";//No service
const u8 *ucSet_network_auto2            = "6e006500740077006f0072006b0020006d006f006400650020006100750074006f006d006100740069006300";//network mode automatic
const u8 *ucSet_network_wcdma_only2      = "5700430044004d00410020006f006e006c007900";//WCDMA only
const u8 *ucSet_network_gsm_only2        = "470053004d0020006f006e006c007900";//GSM only


typedef struct{
	union{
		struct{
			u16 bFreePlay		: 1;
			u16 bCardError		: 1;
			u16 bSingalError	: 1;
			u16 bCheckNet		: 1;
			u16 bLanding		: 1;
			u16 bLandSuccess	: 1;
			u16 bLowAlarm		: 1;
			u16 bLowPowOff		: 1;
			u16 bBattery		: 1;
			u16			: 7;
		}Bits;
		u16 Byte;
	}Msg;
	struct{
                u8 *buf;
		u8 Len;
	}FreePlayData;
	u8 Timer;
}VOICE_DRV;

static VOICE_DRV VoiceDrvObj;

void VOICE_PowerOnInitial(void)
{
	VoiceDrvObj.Msg.Byte = 0x00;
	VoiceDrvObj.Timer = 0x00;
	VoiceDrvObj.FreePlayData.Len = 0x00;
}

void VOICE_Play(VOICEPLAY_TYPE id)
{
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    switch(id)
    {
    case SimCardError:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucSimCardError,strlen((char const*)ucSimCardError));//个呼模式
      break;
    case PersonalMode:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucPersonalMode,strlen((char const*)ucPersonalMode));//个呼模式
      break;
    case ABELL:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucABELL,strlen((char const*)ucABELL));//播报欧标广域对讲机
      break;
    case AllGroupName:
      VOICE_SetOutput(ATVOICE_FreePlay,GetAllGroupNameForVoice(groupCallingcount),strlen((char const*)GetAllGroupNameForVoice(groupCallingcount)));
      break;
    case GroupSelected:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucGroupSelected,strlen((char const*)ucGroupSelected));//英文播报：Group Selected
      break;
    case NoSimCard:
      //VOICE_SetOutput(ATVOICE_FreePlay,"c0680d4e30526153",16);//中文播报：检不到卡
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNoSimCard,strlen((char const*)ucNoSimCard));//中文播报:No SIM Card
      break;
    case NetworkSearching:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNetworkSearching,strlen((char const*)ucNetworkSearching));//播报搜索网络
      break;
    case PowerLowPleaseCharge:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucPowerLowPleaseCharge,strlen((char const*)ucPowerLowPleaseCharge));//电量低请充电
      break;
    case FivePercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucFivePercentPower,strlen((char const*)ucFivePercentPower));//百分之五
      break;
    case TwentyPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucTwentyPercentPower,strlen((char const*)ucTwentyPercentPower));//百分之20
      break;
    case FortyPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucFortyPercentPower,strlen((char const*)ucFortyPercentPower));//百分之40
      break;
    case SixtyPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucSixtyPercentPower,strlen((char const*)ucSixtyPercentPower));//百分之60
      break;
    case EightyPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucEightyPercentPower,strlen((char const*)ucEightyPercentPower));//百分80
      break;
    case OneHundredPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucOneHundredPercentPower,strlen((char const*)ucOneHundredPercentPower));//百分百
      break;
    case GroupMode:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucGroupMode,strlen((char const*)ucGroupMode));//切换群组
      break;
    case HandsetMode:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucHandsetMode,strlen((char const*)ucHandsetMode));//听筒模式
      break;
    case HandfreeMode:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucHandfreeMode,strlen((char const*)ucHandfreeMode));//免提模式
      break;
    case AllUserName:
      VOICE_SetOutput(ATVOICE_FreePlay,GetAllUserNameForVoice(PersonalCallingNum),strlen((char const*)GetAllUserNameForVoice(PersonalCallingNum)));
      break;
    case NoOnlineUser:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNoOnlineUser,strlen((char const*)ucNoOnlineUser));//No Online User
      break;
    case LowBattery:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucLowBattery,strlen((char const*)ucLowBattery));//电量低请充电
      break;
    case LoggingIn:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucLoggingIn,strlen((char const*)ucLoggingIn));//Account Config
      break;
    case cs_unknow_network://CS未知网络
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucCs_unknow_network,strlen((char const*)ucCs_unknow_network));
      break;
    case gprs_unknow_network://GPRS未知网络
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucGprs_unknow_network,strlen((char const*)ucGprs_unknow_network));
      break;
    case gprs_refuse_enroll://GPRS注册被拒绝 
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucGprs_refuse_enroll,strlen((char const*)ucGprs_refuse_enroll));
      break;
    case eps_unknow_network://EPS未知网络 
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucEps_unknow_network,strlen((char const*)ucEps_unknow_network));
      break;
    case eps_refuse_enroll://EPS注册被拒绝 
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucEps_refuse_enroll,strlen((char const*)ucEps_refuse_enroll));
      break;
    case all_unknow_network://所有网络异常
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucAll_unknow_network,strlen((char const*)ucAll_unknow_network));
      break;
    case No_service:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNo_service,strlen((char const*)ucNo_service));
      break;
    case set_network_auto:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucSet_network_auto,strlen((char const*)ucSet_network_auto));
      break;
    case set_network_wcdma_only:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucSet_network_wcdma_only,strlen((char const*)ucSet_network_wcdma_only));
      break;
    case set_network_gsm_only:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucSet_network_gsm_only,strlen((char const*)ucSet_network_gsm_only));
      break;
    default:
      break;
    }
    break;
  case m_ENGLISH:
    switch(id)
    {
    case SimCardError:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucSimCardError2,strlen((char const*)ucSimCardError2));//个呼模式
      break;
    case PersonalMode:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucPersonalMode2,strlen((char const*)ucPersonalMode2));//个呼模式
      break;
    case ABELL:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucABELL2,strlen((char const*)ucABELL2));//播报欧标广域对讲机
      break;
    case AllGroupName:
      VOICE_SetOutput(ATVOICE_FreePlay,GetAllGroupNameForVoice(groupCallingcount),strlen((char const*)GetAllGroupNameForVoice(groupCallingcount)));
      break;
    case GroupSelected:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucGroupSelected2,strlen((char const*)ucGroupSelected2));//英文播报：Group Selected
      break;
    case NoSimCard:
      //VOICE_SetOutput(ATVOICE_FreePlay,"c0680d4e30526153",16);//中文播报：检不到卡
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNoSimCard2,strlen((char const*)ucNoSimCard2));//中文播报:No SIM Card
      break;
    case NetworkSearching:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNetworkSearching2,strlen((char const*)ucNetworkSearching2));//播报搜索网络
      break;
    case PowerLowPleaseCharge:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucPowerLowPleaseCharge2,strlen((char const*)ucPowerLowPleaseCharge2));//电量低请充电
      break;
    case FivePercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucFivePercentPower2,strlen((char const*)ucFivePercentPower2));//百分之五
      break;
    case TwentyPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucTwentyPercentPower2,strlen((char const*)ucTwentyPercentPower2));//百分之20
      break;
    case FortyPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucFortyPercentPower2,strlen((char const*)ucFortyPercentPower2));//百分之40
      break;
    case SixtyPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucSixtyPercentPower2,strlen((char const*)ucSixtyPercentPower2));//百分之60
      break;
    case EightyPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucEightyPercentPower2,strlen((char const*)ucEightyPercentPower2));//百分80
      break;
    case OneHundredPercentPower:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucOneHundredPercentPower2,strlen((char const*)ucOneHundredPercentPower2));//百分百
      break;
    case GroupMode:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucGroupMode2,strlen((char const*)ucGroupMode2));//切换群组
      break;
    case HandsetMode:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucHandsetMode2,strlen((char const*)ucHandsetMode2));//听筒模式
      break;
    case HandfreeMode:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucHandfreeMode2,strlen((char const*)ucHandfreeMode2));//免提模式
      break;
    case AllUserName:
      VOICE_SetOutput(ATVOICE_FreePlay,GetAllUserNameForVoice(PersonalCallingNum),strlen((char const*)GetAllUserNameForVoice(PersonalCallingNum)));
      break;
    case NoOnlineUser:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNoOnlineUser2,strlen((char const*)ucNoOnlineUser2));//No Online User
      break;
    case LowBattery:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucLowBattery2,strlen((char const*)ucLowBattery2));//电量低请充电
      break;
    case LoggingIn:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucLoggingIn2,strlen((char const*)ucLoggingIn2));//Account Config
      break;
    case cs_unknow_network://CS未知网络
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucCs_unknow_network2,strlen((char const*)ucCs_unknow_network2));
      break;
    case gprs_unknow_network://GPRS未知网络
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucGprs_unknow_network2,strlen((char const*)ucGprs_unknow_network2));
      break;
    case gprs_refuse_enroll://GPRS注册被拒绝 
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucGprs_refuse_enroll2,strlen((char const*)ucGprs_refuse_enroll2));
      break;
    case eps_unknow_network://EPS未知网络 
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucEps_unknow_network2,strlen((char const*)ucEps_unknow_network2));
      break;
    case eps_refuse_enroll://EPS注册被拒绝 
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucEps_refuse_enroll2,strlen((char const*)ucEps_refuse_enroll2));
      break;
    case all_unknow_network://所有网络异常
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucAll_unknow_network2,strlen((char const*)ucAll_unknow_network2));
      break;
    case No_service:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNo_service2,strlen((char const*)ucNo_service2));
      break;
    case set_network_auto:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucSet_network_auto2,strlen((char const*)ucSet_network_auto2));
      break;
    case set_network_wcdma_only:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucSet_network_wcdma_only2,strlen((char const*)ucSet_network_wcdma_only2));
      break;
    case set_network_gsm_only:
      VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucSet_network_gsm_only2,strlen((char const*)ucSet_network_gsm_only2));
      break;
    default:
      break;
    }
    break;
  }

}


void VOICE_SetOutput(AtVoiceType Id, u8 *buf, u8 Len)
{
 // set_ApiAtCmd_bZTTSStates(1);
  //set_ApiAtCmd_bZTTSStates_Intermediate(0);//将播报语音中间变量清零，等待收到ztts0重新计数
  AUDIO_IOAFPOW(ON);
	VoiceDrvObj.Msg.Byte |= Id;	
	VoiceDrvObj.FreePlayData.buf = buf;
	VoiceDrvObj.FreePlayData.Len = Len;
	if(Id != ATVOICE_FreePlay)
	{
		VoiceDrvObj.Timer = 0x00;
	}
	else
	{
		VoiceDrvObj.Timer = 0x00;
		VOICE_Renew();
	}
        
	return;
}

void VOICE_Renew()
{
  u8 i;
  const	u8 * pBuf;
  u16 ucMask;
  if (VoiceDrvObj.Msg.Byte != 0x00)
  {
    if(VoiceDrvObj.Timer == 0x00)
    {
      ucMask = 0x01;
      for(i = 0x00; i < 0x10; i++)
      {
        if((ucMask & VoiceDrvObj.Msg.Byte) != OFF)
        {
          VoiceDrvObj.Msg.Byte &= (~ucMask);
          if(VoiceDrvObj.Msg.Byte != 0x00)
          {
            VoiceDrvObj.Timer = 0x08;
          }
          switch(ucMask)
          {
            case ATVOICE_FreePlay:
              pBuf = VoiceDrvObj.FreePlayData.buf;
              i = VoiceDrvObj.FreePlayData.Len;
              break;
            default:
              pBuf = (void *)0;
              i = 0x00;
              break;
          }
          ApiAtCmd_PlayVoice(ucMask, (u8 *)pBuf, i);
          break;
        }
        ucMask <<= 0x01;
      }
    }
  }
}

void VOICE_1sProcess()
{
	if(VoiceDrvObj.Timer != 0x00)
	{
		VoiceDrvObj.Timer--;
	}
}
