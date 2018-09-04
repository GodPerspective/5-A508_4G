#define SYSABLE
#include "AllHead.h"

#define SYS_IDLE	0x00
#define SYS_RUN		0x01



typedef struct {
  struct{
    union{
      struct{
        u8 WorkMode	: 2;
        u8 UnlineArm	: 1;
        u8      	: 5;
      }Bits;
      u8 Byte;
    }Cfg;
    u8 UnlineArmCountdown;
    u8 Res[3];
  }Par;
  union {
    struct {
      u16 bLoad		: 1;
      u16 bRenew	: 1;
      u16 bTxRxRun	: 1;
      u16 bDebug	: 3;
      u16 bRxNullCh	: 1;
      u16 bTxNullCh	: 1;
      u16 bTalk		: 1;
      u16 bSq		: 1;
      u16 bSqMome	: 1;
      u16 bMoni		: 1;
      u16 bMoniMome	: 1;
      u16 bTot		: 1;
      u16 bUnlineArm	: 1;
      u16 		: 1;
    }Bit;
    u16 Byte;
  }STA;
  union {
    struct {
      u16 TxError		: 1;
      u16 			: 15;
    }Bit;
    u16 Byte;
  }Error;
  u16 cChannel;
  u8 KeyCode;
  POW_MANTYPE   PowMan;
  IO_ONOFF	  PowOnOff;
  TASK_CODE 	  NewId;
  TASK_CODE  	  OldId;
  MSG_STATUS	  Msg;
  SW_CODE 	  Sw;
  u16 UnlineArmTimer;
}TASK_DRV;

static TASK_DRV	TaskDrvObj;

u8 *ucSwitch            = "10000002";


void main_app(void)
{
  main_init();
  while(1)
  {
    LowVoltageDetection();
    DEL_Renew();
    switch(TaskDrvObj.NewId)
    {
    case Task_Start:
      Task_RunStart();
      if(ApiPocCmd_GroupStates()==EnterGroup)
      {
        TaskDrvObj.NewId=Task_NormalOperation;
        //ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
      }
      break;
    case Task_NormalOperation:
      Task_RunNormalOperation();
      break;
    case TASK_WRITEFREQ:
      TASK_WriteFreq();
      break;
    case TASK_LOBATT:		//task LO battery process
      TASK_RunLoBattery();
      break;
    default:
      break;
    }
}
}


TASK_CODE GetTaskId(void)
{
  return TaskDrvObj.NewId;
}
void SetTaskId(TASK_CODE a)
{
  TaskDrvObj.NewId=a;
}