#include "AllHead.h"

#ifdef  CHINESE
const u8 *ucPersonalMode                = "2a4e7c54216a0f5f";//����ģʽ-
const u8 *ucABELL                       = "276b0768f95bb28b3a67";//ŷ��Խ�
const u8 *ucGroupSelected               = "f25d09902d4e";//��ѡ��-
const u8 *ucNoSimCard                   = "2a67d2636153";//δ�忨
const u8 *ucNetworkSearching            = "1c64227d517fdc7e";//��������-
const u8 *ucPowerLowPleaseCharge        = "3575cf914e4f0cfff78b45513575";//�����ͣ�����-
const u8 *ucLowBattery                  = "3575cf914e4f";//������-
const u8 *ucFivePercentPower            = "3575cf917e7606524b4e944e";//�ٷ�֮��-
const u8 *ucTwentyPercentPower          = "3575cf917e7606524b4e8c4e4153";//�ٷ�֮��ʮ-
const u8 *ucFortyPercentPower           = "3575cf917e7606524b4edb564153";//�ٷ�֮��ʮ-
const u8 *ucSixtyPercentPower           = "3575cf917e7606524b4e6d514153";//�ٷ�֮��ʮ-
const u8 *ucEightyPercentPower          = "3575cf917e7606524b4e6b514153";//�ٷ�֮��ʮ-
const u8 *ucOneHundredPercentPower      = "3575cf917e7606524b4e004e7e76";//�ٷ�֮һ��-
const u8 *ucGroupMode                   = "c47e7c54216a0f5f";//���ģʽ-
const u8 *ucHandsetMode                 = "2c54527b216a0f5f";//��Ͳģʽ-
const u8 *ucHandfreeMode                = "4d51d063216a0f5f";//����ģʽ-
const u8 *ucNoOnlineUser                = "e0652857bf7e10625854";//�����߳�Ա
const u8 *ucLoggingIn                   = "636b28577b764696";//���ڵ�½- 
#else
const u8 *ucPersonalMode                = "50006500720073006f006e0061006c0020004d006f0064006500";//personal mode
const u8 *ucABELL                       = "4100620065006C006C00";//ABELL
const u8 *ucGroupSelected               = "530065006c0065006300740065006400";//Group Selected
const u8 *ucNoSimCard                   = "6e006f002000730069006D0020006300610072006400";//NO SIM Card
const u8 *ucNetworkSearching            = "6e006500740077006f0072006b002000730065006100720063006800";//network searching
const u8 *ucPowerLowPleaseCharge        = "50006f0077006500720020006c006f0077002c0050006c0065006100730065002000430068006100720067006500";//Power Low Please Charge
const u8 *ucLowBattery                  = "4c006f00770020004200610074007400650072007900";//Low Battery
const u8 *ucFivePercentPower            = "50006f00770065007200200046006900760065002000500065007200630065006e007400";//Five Percent Power
const u8 *ucTwentyPercentPower          = "50006f0077006500720020005400770065006e00740079002000500065007200630065006e007400";
const u8 *ucFortyPercentPower           = "50006f00770065007200200046006f007200740079002000500065007200630065006e007400";
const u8 *ucSixtyPercentPower           = "50006f007700650072002000530069007800740079002000500065007200630065006e007400";
const u8 *ucEightyPercentPower          = "50006f0077006500720020004500690067006800740079002000500065007200630065006e007400";
const u8 *ucOneHundredPercentPower      = "50006f0077006500720020004f006e0065002000480075006e0064007200650064002000500065007200630065006e007400";
const u8 *ucGroupMode                   = "470072006f007500700020004d006f0064006500";
const u8 *ucHandsetMode                 = "480061006e006400200073006500740020004d006f0064006500";
const u8 *ucHandfreeMode                = "480061006e0064002000660072006500650020004d006f0064006500";
const u8 *ucNoOnlineUser                = "4e006f0020006f006e006c0069006e00650020007500730065007200";
const u8 *ucLoggingIn                   = "4100630063006f0075006e007400200043006f006e00660069006700750072006100740069006f006e00";//Account Config
#endif

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
  switch(id)
  {
  case PersonalMode:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucPersonalMode,strlen((char const*)ucPersonalMode));//personal calling Mode
    break;
  case ABELL:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucABELL,strlen((char const*)ucABELL));//����Abell
    break;
  case AllGroupName:
    //VOICE_SetOutput(ATVOICE_FreePlay,GetAllGroupNameForVoice(GroupCallingNum),strlen((char const*)GetAllGroupNameForVoice(GroupCallingNum)));
    break;
  case GroupSelected:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8 *)ucGroupSelected,strlen((char const*)ucGroupSelected));//Ӣ�Ĳ�����Group Selected
    break;
  case NoSimCard:
    //VOICE_SetOutput(ATVOICE_FreePlay,"c0680d4e30526153",16);//���Ĳ������첻����
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNoSimCard,strlen((char const*)ucNoSimCard));//���Ĳ���:No SIM Card
    break;
  case NetworkSearching:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNetworkSearching,strlen((char const*)ucNetworkSearching));//������������
    break;
  case PowerLowPleaseCharge:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucPowerLowPleaseCharge,strlen((char const*)ucPowerLowPleaseCharge));//����������
    break;
  case FivePercentPower:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucFivePercentPower,strlen((char const*)ucFivePercentPower));//�ٷ�֮��
    break;
  case TwentyPercentPower:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucTwentyPercentPower,strlen((char const*)ucTwentyPercentPower));//�ٷ�֮20
    break;
  case FortyPercentPower:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucFortyPercentPower,strlen((char const*)ucFortyPercentPower));//�ٷ�֮40
    break;
  case SixtyPercentPower:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucSixtyPercentPower,strlen((char const*)ucSixtyPercentPower));//�ٷ�֮60
    break;
  case EightyPercentPower:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucEightyPercentPower,strlen((char const*)ucEightyPercentPower));//�ٷ�80
    break;
  case OneHundredPercentPower:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucOneHundredPercentPower,strlen((char const*)ucOneHundredPercentPower));//�ٷְ�
    break;
  case GroupMode:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucGroupMode,strlen((char const*)ucGroupMode));//�л�Ⱥ��
    break;
  case HandsetMode:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucHandsetMode,strlen((char const*)ucHandsetMode));//��Ͳģʽ
    break;
  case HandfreeMode:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucHandfreeMode,strlen((char const*)ucHandfreeMode));//����ģʽ
    break;
  case AllUserName:
    //VOICE_SetOutput(ATVOICE_FreePlay,GetAllUserNameForVoice(PersonalCallingNum),strlen((char const*)GetAllUserNameForVoice(PersonalCallingNum)));
    break;
  case NoOnlineUser:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucNoOnlineUser,strlen((char const*)ucNoOnlineUser));//No Online User
    break;
  case LowBattery:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucLowBattery,strlen((char const*)ucLowBattery));//����������
    break;
  case LoggingIn:
    VOICE_SetOutput(ATVOICE_FreePlay,(u8*)ucLoggingIn,strlen((char const*)ucLoggingIn));//Account Config
    break;
  default:
    break;
  }
}


void VOICE_SetOutput(AtVoiceType Id, u8 *buf, u8 Len)
{
 // set_ApiAtCmd_bZTTSStates(1);
  //set_ApiAtCmd_bZTTSStates_Intermediate(0);//�����������м�������㣬�ȴ��յ�ztts0���¼���
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
