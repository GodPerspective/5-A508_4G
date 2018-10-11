#ifndef __ALLHEAD_H
#define __ALLHEAD_H

#include "stm8s.h"
#include <string.h>
#include "CommonLib.h"
#include "Task.h"
#include "sysclock.h"

#include "DrvMcuPin.h"
#include "DrvZM389.h"
#include "DrvUart1.h"

#include "ApiVoice.h"
#include "ApiAtCmd.h"
#include "ApiPocCmd.h"
#include "ApiDelay.h"

//����
#include "DrvI2cAw87319.h"

//��ʾ��
#include "ApiDisplay.h"
#include "DrvHTG12832.h"
#include "DrvGT20L16S1Y.h"
//�ֿ�IC
#include "GT20L16P1Y.h"
#include "GT20L16P1Y_D.h"//�ֿⲹ��

//дƵ
#include "DrvUart3.h"
#include "DrvEeprom.h"
#include "DataStructAddr.h"
#include "ApiMcuVersion.h"

//��������
#include "DrvKeyscan.h"
#include "ApiKeycmd.h"

//���̴���
#include "DrvKeyboard.h"
#include "ApiKeyboard.h"

//�˵�
#include "ApiMenu.h"

//��ѹ���
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
//ͨѶģ��
#include "DrvGD83.h"
#include <string.h>
#include "ApiPocCmd.h"
#include "ApiAtCmd.h"
//����
#include "DrvKeyboard.h"
#include "ApiKeyboard.h"
//��ʾ��
#include "ApiDisplay.h"
#include "DrvHTG12832.h"
#include "DrvGT20L16S1Y.h"
//�ֿ�IC
#include "GT20L16P1Y.h"
#include "GT20L16P1Y_D.h"//�ֿⲹ��
//BEEP
#include "Tone.h"
#include "Beep.h"
//����
#include "Task.h"
#include "System.h"
//ָʾ��
#include "ApiLED.h"
//дƵ
#include "uart3.h"
#include "Flash_eeprom.h"
#include "Config.h"//����ǿ������
#include "DataStructAddr.h"
//����
#include "ApiGpsCmd.h"
//��ص������
#include "ADC.h"
//�˵���
#include "ApiMenu.h"
//����
#include "ApiBeidou.h"
//�����汾����
#include "KeyCmd.h"
#endif
//ʹ��Ӣ������
//#define CHINESE

#endif