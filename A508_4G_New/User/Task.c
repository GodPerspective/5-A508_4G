#include "AllHead.h"



TaskDrv TaskDrvobj;

void Task_Init(void)
{
  TaskDrvobj.Id = TASK_LOGIN;
}

void Task_login_progress(void)
{
  //UART3_ToMcuMain();//¶ÁÈ¡Ð´ÆµÊý¾Ý
}

void Task_normal_progress(void)
{
  
}

void Task_low_battery_progress(void)
{
  
}

void Task_write_freq_progress(void)
{
  
}