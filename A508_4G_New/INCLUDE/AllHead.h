#ifndef __ALLHEAD_H
#define __ALLHEAD_H

#include "stm8s.h"
#include <string.h>
#include "CommonLib.h"
#include "main.h"
#include "Task.h"
#include "sysclock.h"

#include "DrvMcuPin.h"
#include "DrvZM389.h"
#include "DrvUart1.h"

#include "ApiVoice.h"
#include "ApiAtCmd.h"
#include "ApiPocCmd.h"
#include "ApiDelay.h"
#include "ApiGpsCmd.h"
//功放
#include "DrvI2cAw87319.h"

//显示屏
#include "ApiDisplay.h"
#include "DrvHTG12832.h"
#include "DrvGT20L16S1Y.h"

//字库IC
#include "GT20L16P1Y.h"
#include "GT20L16P1Y_D.h"//字库补丁

//写频
#include "DrvUart3.h"
#include "DrvEeprom.h"
#include "DataStructAddr.h"
#include "ApiMcuVersion.h"

//按键处理
#include "DrvKeyscan.h"
#include "ApiKeycmd.h"

//键盘处理
#include "DrvKeyboard.h"
#include "ApiKeyboard.h"

//菜单
#include "ApiMenu.h"

//电压检测
#include "ApiBattery.h"

//beep
#include "DrvBeep.h"
#include "DrvTone.h"

//LED
#include "ApiLED.h"

#if 0
#include "McuConfig.h"
#include "stm8s_it.h"
#include "CommonLib.h"
#include "Define.h"// 
#include "Delay.h"
#include "Voice.h"
//通讯模块
#include "DrvGD83.h"
#include <string.h>
#include "ApiPocCmd.h"
#include "ApiAtCmd.h"
//键盘
#include "DrvKeyboard.h"
#include "ApiKeyboard.h"
//显示屏
#include "ApiDisplay.h"
#include "DrvHTG12832.h"
#include "DrvGT20L16S1Y.h"
//字库IC
#include "GT20L16P1Y.h"
#include "GT20L16P1Y_D.h"//字库补丁
//BEEP
#include "Tone.h"
#include "Beep.h"
//任务
#include "Task.h"
#include "System.h"
//指示灯
#include "ApiLED.h"
//写频
#include "uart3.h"
#include "Flash_eeprom.h"
#include "Config.h"//部标强行添加
#include "DataStructAddr.h"
//部标
#include "ApiGpsCmd.h"
//电池电量检测
#include "ADC.h"
//菜单显
#include "ApiMenu.h"
//北斗
#include "ApiBeidou.h"
//软件版本设置
#include "KeyCmd.h"
#endif
//使能英文语音
//#define CHINESE

#endif