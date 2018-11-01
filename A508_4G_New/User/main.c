#include "AllHead.h"
//test测试
void main_all_init(void);
void main_task(void);

void main(void)
{
  
#if 1//ʹ�ܴ������س���
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_ProgramOptionByte(0x487E,0x55);
  FLASH_ProgramOptionByte(0x487F,0xAA);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
#endif

  main_all_init();
  while(1)
  {
    switch(TaskDrvobj.Id)
    {
     case TASK_LOGIN:
      LowVoltageDetection();
      DEL_Renew();
      Task_login_progress();
      if(poccmd_states_poc_status()==LandSuccess)
      {
        DISPLAY_Show(d_not_in_groups);
        TaskDrvobj.Id=TASK_NORMAL;
        TaskDrvobj.login_step=0;
      }
      break;
    case TASK_NORMAL:
      LowVoltageDetection();
      DEL_Renew();
      Task_normal_progress();
      break;
    case TASK_LOW_BATTERY:
      LowVoltageDetection();
      DEL_Renew();
      Task_low_battery_progress();
      break;
    case TASK_POWEROFF:
      Task_PowerOff();
      break;
    default:
      break;
    }
  }
}

void main_all_init(void)
{
  disableInterrupts();
  SystemClock_Init(HSE_Clock);
  ITC_SetSoftwarePriority(ITC_IRQ_UART1_RX,ITC_PRIORITYLEVEL_3);
  ITC_SetSoftwarePriority(ITC_IRQ_UART1_TX,ITC_PRIORITYLEVEL_3);
  ITC_SetSoftwarePriority(ITC_IRQ_UART3_RX,ITC_PRIORITYLEVEL_2);
  ITC_SetSoftwarePriority(ITC_IRQ_TIM1_OVF,ITC_PRIORITYLEVEL_2);
  ITC_SetSoftwarePriority(ITC_IRQ_TIM3_OVF,ITC_PRIORITYLEVEL_1);
  
  //�����ʼ��
  Task_Init();
  
  //ģ��ͨѶ����uart1��ʼ��
  DrvUart1_init();
  DrvMC8332_Software_Initial();

  //POC��ʼ��
  ApiPocCmd_PowerOnInitial();
  
  //AT��ʼ��
  ApiAtCmd_PowerOnInitial();
  
  //��ʱ��ʼ��
  DEL_PowerOnInitial();
  
  //GPS��ʼ��
  ApiBeidou_PowerOnInitial();
  
  //���ȿ��ƽ�
  Noise_Mute_Init();
  VOICE_PowerOnInitial();
  //��ʾ��
  drv_gt20_pwr_on_init();
  drv_htg_pwr_on_init();
  ApiDisplay_PowerOnInitial();
  
  //�˵���ʼ��
  ApiMenu_init();
  
  //дƵ��ʼ��
  Uart3_Init(); //����дƵʹ��
  
  //������ʼ��
  Key_Init();
  ApiKeyCmd_PowerOnInitial();
  ApiKeyboard_PowerOnInitial();
  //ָʾ�Ƴ�ʼ��
  LED_Init();
  ApiLED_PowerOnInitial();
  
  //����汾��ȡ������ʼ��
  ApiMcuVersion_PowerOnInitial();
  //BEEP
  BEEP_PowerOnInitial();
  TONE_PowerOnInitial();
  
  //��ص�ѹ�ɼ�
  ADC_Init();
  ApiBattery_PowerOnInitial();
  
  enableInterrupts();
  AUDIO_IOAFPOW(ON);
  GPIO_Init(GPIOB,GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);//NFC
  GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);//����
#if 0//������λ
  GPIO_WriteLow(GPIOB,GPIO_PIN_3);//NFC
  GPIO_WriteHigh(GPIOB,GPIO_PIN_4);//����
#else//дƵ������Ĭ��дƵ��һ���Ӻ�תΪ������λ��
  GPIO_WriteHigh(GPIOB,GPIO_PIN_3);//NFC
  GPIO_WriteHigh(GPIOB,GPIO_PIN_4);//����
#endif
  DISPLAY_Show(d_ABELL);
  BEEP_Time(100);
  MCU_LCD_BACKLIGTH(ON);
  api_disp_icoid_output( eICO_IDBATT5, TRUE, TRUE);//��ʾ�������ͼ��
  api_disp_icoid_output( eICO_IDTemper, TRUE, TRUE);//����ģʽ
  //IIC-AW87319����
  //iic_init();
  //ApiAtCmd_WritCommand(ATCOMM_RESET,(void*)0, 0);
}

#ifdef USE_FULL_ASSERT        

/**
  * @brief  Reports the name of the source file and the source line number
  * where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  while (1)
  {
  }
}
#endif
