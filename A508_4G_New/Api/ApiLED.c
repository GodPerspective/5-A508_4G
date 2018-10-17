#include "AllHead.h"
u8 LED_Conut=0;

void ApiLED_PowerOnInitial(void)
{
  LED_Conut=0;
}
void LED_IntOutputRenew(void)
{
  if(TaskDrvobj.Id==TASK_LOGIN)
  {
    LED_Conut++;
    if(LED_Conut==5)
    {
      Set_GreenLed(ON);
    }
    if(LED_Conut==8)
    {
      Set_GreenLed(OFF);
      LED_Conut=0;
    }
  }
  else
  {
    if(poc_receive_sos_statas()==TRUE)
    {
    }
    else
    {
      if(ApiPocCmd_ReceivedVoicePlayStatesForLED()==TRUE)
      {
        Set_GreenLed(ON);
      }
      else
      {
        LED_Conut++;
        if(LED_Conut>=49)
        {
#if 1
          if(0)
#else
          if(beidou_valid()==TRUE)
#endif
          {
            Set_RedLed(ON);
          }
          else
          {
            Set_GreenLed(ON);
          } 
        }
        if(LED_Conut>=51)
        {
          Set_GreenLed(OFF);
          Set_RedLed(OFF);
          LED_Conut=0;
        }
        if(LED_Conut<=48)
        {
          Set_GreenLed(OFF);
          if(WriteFreq_Flag==TRUE)//解决读频红灯常亮的问题，应该是闪红灯
          {
            Set_RedLed(OFF);
          }
          else
          {
          }
        }
      }
    }

  }
}