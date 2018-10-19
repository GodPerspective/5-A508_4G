#include "DrvMcuPin.h"

/******************************************************************************
;--------1-LED &POWER hardware macro define
******************************************************************************/
void LED_Init(void)
{
  GPIO_Init(GPIO_LED_Green,GPIO_PIN_LED_Green,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIO_LED_Red,GPIO_PIN_LED_Red,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIO_RSSI,GPIO_PIN_RSSI,GPIO_MODE_IN_PU_NO_IT);//RSSI
  GPIO_Init(GPIO_Poweroff,GPIO_PIN_Poweroff,GPIO_MODE_OUT_PP_HIGH_FAST);
}

void Set_GreenLed(IO_ONOFF state)
{
  switch(state)
  {
  case ON:
      GPIO_WriteHigh(GPIO_LED_Green, GPIO_PIN_LED_Green);
    break;
  case OFF:
      GPIO_WriteLow(GPIO_LED_Green, GPIO_PIN_LED_Green);
    break;
  }
}

void Set_RedLed(IO_ONOFF state)
{
  switch(state)
  {
  case ON:
      GPIO_WriteHigh(GPIO_Poweroff, GPIO_PIN_LED_Red);
    break;
  case OFF:
      GPIO_WriteLow(GPIO_Poweroff, GPIO_PIN_LED_Red);
      break;
  }
}

void set_power_off(IO_ONOFF state)
{
  switch(state)
  {
  case ON:
      GPIO_WriteHigh(GPIO_Poweroff, GPIO_PIN_Poweroff);
    break;
  case OFF:
      GPIO_WriteLow(GPIO_Poweroff, GPIO_PIN_Poweroff);
      break;
  }
}
/******************************************************************************
;--------2-TIM3:DELAY hardware macro define
******************************************************************************/
void Tim3_Timer_Init(void)
{
#ifdef HSI_CLK
  	/*BaseTime=1/(16000000/16)*(999+1)=1ms*/
	TIM3_TimeBaseInit(TIM3_PRESCALER_16 ,999);
        
	TIM3_PrescalerConfig(TIM3_PRESCALER_16,TIM3_PSCRELOADMODE_IMMEDIATE);
#endif
#ifdef HSE_CLK
    /*BaseTime=1/(24000000/8)*(2999+1)=1ms*/
	TIM3_TimeBaseInit(TIM3_PRESCALER_8 ,499);
	TIM3_PrescalerConfig(TIM3_PRESCALER_16,TIM3_PSCRELOADMODE_IMMEDIATE);
#endif
	TIM3_ARRPreloadConfig(ENABLE);
	TIM3_ITConfig(TIM3_IT_UPDATE , ENABLE);
	TIM3_Cmd(ENABLE);
}

/******************************************************************************
;--------4-Audio&MIC hardware macro define
******************************************************************************/
void Noise_Mute_Init(void)
{GPIO_Init(GPIO_Noise_Mute,GPIO_PIN_Noise_Mute,GPIO_MODE_OUT_PP_LOW_FAST);}

void AUDIO_IOAFPOW(IO_ONOFF OnOff)
{
  switch(OnOff)
  {
  case ON:
      GPIO_WriteHigh(GPIO_Noise_Mute, GPIO_PIN_Noise_Mute);
    break;
  case OFF:
      GPIO_WriteLow(GPIO_Noise_Mute, GPIO_PIN_Noise_Mute);
    break;
  default:break; 
  }
}


