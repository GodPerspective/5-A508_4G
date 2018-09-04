#include "DrvMcuPin.h"

/******************************************************************************
;--------1-LED hardware macro define
******************************************************************************/
void LED_Init(void)
{
  GPIO_Init(GPIO_LED_Green,GPIO_PIN_LED_Green,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIO_LED_Red,GPIO_PIN_LED_Red,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIO_RSSI,GPIO_PIN_RSSI,GPIO_MODE_IN_PU_NO_IT);//RSSI
}

void Set_GreenLed(u8 state)
{
  switch(state)
  {
  case LED_ON:
      GPIO_WriteHigh(GPIO_LED_Green, GPIO_PIN_LED_Green);
    break;
  case LED_OFF:
      GPIO_WriteLow(GPIO_LED_Green, GPIO_PIN_LED_Green);
    break;
  }
}

void Set_RedLed(u8 state)
{
  switch(state)
  {
  case LED_ON:
      GPIO_WriteHigh(GPIO_LED_Red, GPIO_PIN_LED_Red);
    break;
  case LED_OFF:
      GPIO_WriteLow(GPIO_LED_Red, GPIO_PIN_LED_Red);
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
void MIC_GPIO_Init(void)
{GPIO_Init(GPIO_MIC_Mute,GPIO_PIN_MIC_Mute,GPIO_MODE_OUT_PP_LOW_FAST);}

void AF_Mute_Init(void)
{GPIO_Init(GPIO_AF_Mute,GPIO_PIN_AF_Mute,GPIO_MODE_OUT_PP_LOW_FAST);}

void Noise_Mute_Init(void)
{GPIO_Init(GPIO_Noise_Mute,GPIO_PIN_Noise_Mute,GPIO_MODE_OUT_PP_LOW_FAST);}

void C_TEST_OUT_SET(void)
{
  GPIO_Init(GPIO_MIC_Mute,GPIO_PIN_MIC_Mute,GPIO_MODE_OUT_PP_LOW_FAST);
}

void MIC_IOMUT(IO_ONOFF OnOff)
{
  switch(OnOff)
  {
  case ON:
      GPIO_WriteHigh(GPIO_MIC_Mute,GPIO_PIN_MIC_Mute);
    break;
  case OFF:
      GPIO_WriteLow(GPIO_MIC_Mute,GPIO_PIN_MIC_Mute);
    break;
  }
}



void AUDIO_IOAFMUT(IO_ONOFF OnOff)
{
  switch(OnOff)
  {
  case ON:
      GPIO_WriteHigh(GPIO_AF_Mute, GPIO_PIN_AF_Mute);
    break;
  case OFF:
      GPIO_WriteLow(GPIO_AF_Mute, GPIO_PIN_AF_Mute);
    break;
  default:break; 
  }
}

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

/******************************************************************************
;--------5-KEY hardware macro define
******************************************************************************/
void Key_Init(void)
{
  GPIO_Init(GPIO_Key_2,GPIO_PIN_Key_2,GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIO_Key_3,GPIO_PIN_Key_3,GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIO_Key_4,GPIO_PIN_Key_4,GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIO_Key_PTT,GPIO_PIN_Key_PTT,GPIO_MODE_IN_PU_NO_IT);
}

