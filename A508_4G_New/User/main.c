#include "AllHead.h"

void main_all_init(void);
void main_task(void);

void main(void)
{
  
#if 1//使能串口下载程序
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_ProgramOptionByte(0x487E,0x55);
  FLASH_ProgramOptionByte(0x487F,0xAA);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
#endif

  main_all_init();
  while(1)
  {
    LowVoltageDetection();
    DEL_Renew();
    switch(TaskDrvobj.Id)
    {
     case TASK_LOGIN:
      Task_login_progress();
      if(poccmd_states_poc_status()==LandSuccess)
      {
        DISPLAY_Show(d_not_in_groups);
        TaskDrvobj.Id=TASK_NORMAL;
        TaskDrvobj.login_step=0;
      }
      break;
    case TASK_NORMAL:
      Task_normal_progress();
      break;
    case TASK_LOW_BATTERY:
      Task_low_battery_progress();
      break;
    case TASK_WRITE_FREQ:
      Task_write_freq_progress();
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
  
  //任务初始化
  Task_Init();
  
  //模块通讯串口uart1初始化
  DrvUart1_init();
  DrvMC8332_Software_Initial();

  //POC初始化
  ApiPocCmd_PowerOnInitial();
  
  //AT初始化
  ApiAtCmd_PowerOnInitial();
  
  //定时初始化
  DEL_PowerOnInitial();
  
  //喇叭控制脚
  Noise_Mute_Init();
  VOICE_PowerOnInitial();
  //显示屏
  drv_gt20_pwr_on_init();
  drv_htg_pwr_on_init();
  ApiDisplay_PowerOnInitial();
  //写频初始化
  Uart3_Init(); //串口写频使用
  
  //按键初始化
  Key_Init();
  ApiKeyCmd_PowerOnInitial();
  ApiKeyboard_PowerOnInitial();
  //指示灯初始化
  LED_Init();
  ApiLED_PowerOnInitial();
  
  //软件版本读取函数初始化
  ApiMcuVersion_PowerOnInitial();
  //BEEP
  BEEP_PowerOnInitial();
  TONE_PowerOnInitial();
  
  //电池电压采集
  ADC_Init();
  ApiBattery_PowerOnInitial();
  
  enableInterrupts();
  
  AUDIO_IOAFPOW(ON);
  BEEP_Time(100);
  MCU_LCD_BACKLIGTH(ON);
  api_disp_icoid_output( eICO_IDBATT5, TRUE, TRUE);//显示电池满电图标
  
  DISPLAY_Show(d_ABELL);
  //IIC-AW87319功放
  iic_init();
  ApiAtCmd_WritCommand(ATCOMM_RESET,(void*)0, 0);
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
