#ifndef	DELAY_H
#define DELAY_H

extern u16 TimeCount;//��ʱʱ��
extern u16 TimeCount3;
extern bool LockingState_Flag;
extern u8 ApiAtCmd_ZTTSCount;
extern u8 ShowTime_Flag;
extern bool delay_gps_value_for_display_flag2(void);
extern u8 read_backlight_time_value(void);
extern u8 read_key_lock_time_value(void);
extern u8 BacklightTimeCount;//�����ʱ��(��Ҫ���ý���eeprom)
extern u16 KeylockTimeCount;//������ʱ��(��Ҫ���ý���eeprom)

extern u8 TaskStartDeleteDelay1Count;
extern u8 TaskStartDeleteDelay3Count;
extern u8 TaskStartDeleteDelay4Count;
extern u8 TaskStartDeleteDelay6Count;
extern bool LockingState_Flag;
extern u16 TimeCount;//��ʱʱ��
extern u16 TimeCount3;
extern u8 BacklightTimeCount;//�����ʱ��(��Ҫ���ý���eeprom)
extern u16 KeylockTimeCount;//������ʱ��(��Ҫ���ý���eeprom)
extern void DEL_PowerOnInitial(void);
extern void DEL_Interrupt(void);
extern void DEL_Renew(void);
extern void DEL_Soft1ms(u16 iLen); 
extern void Delay_100ms(u8 T);
extern bool DEL_SetTimer(u8 cId,u16 iLen);
extern bool DEL_GetTimer(u8 cId);

#endif
