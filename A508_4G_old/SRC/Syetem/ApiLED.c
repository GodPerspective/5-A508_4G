#include "AllHead.h"
u8 LED_Conut=0;
void LED_IntOutputRenew(void)
{

  if( GetTaskId()==Task_Start)
  {
    LED_Conut++;
    if(LED_Conut==5)
    {
      Set_GreenLed(LED_ON);
    }
    if(LED_Conut==8)
    {
      Set_GreenLed(LED_OFF);
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
        Set_GreenLed(LED_ON);
      }
      else
      {
        LED_Conut++;
        if(LED_Conut>=49)
        {
          if(beidou_valid()==TRUE)
          {
            Set_RedLed(LED_ON);
          }
          else
          {
            Set_GreenLed(LED_ON);
          } 
        }
        if(LED_Conut>=51)
        {
          Set_GreenLed(LED_OFF);
          Set_RedLed(LED_OFF);
          LED_Conut=0;
        }
        if(LED_Conut<=48)
        {
          Set_GreenLed(LED_OFF);
          if(WriteFreq_Flag==TRUE)//�����Ƶ��Ƴ��������⣬Ӧ���������
          {
            Set_RedLed(LED_OFF);
          }
          else
          {
          }
        }
      }
    }

  }
}