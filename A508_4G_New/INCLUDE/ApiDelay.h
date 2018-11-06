#ifndef	DELAY_H
#define DELAY_H

extern u16 TimeCount;//超时时间
extern u16 TimeCount3;
extern bool LockingState_Flag;
extern u8 BacklightTimeCount;//背光灯时间(需要设置进入eeprom)
extern u16 KeylockTimeCount;//键盘锁时间(需要设置进入eeprom)
extern bool LockingState_Flag;
extern u16 TimeCount;//超时时间
extern u16 TimeCount3;
extern u8 BacklightTimeCount;//背光灯时间(需要设置进入eeprom)
extern u16 KeylockTimeCount;//键盘锁时间(需要设置进入eeprom)

bool delay_gps_value_for_display_flag2(void);
extern u8 read_backlight_time_value(void);
extern u8 read_key_lock_time_value(void);
extern void DEL_PowerOnInitial(void);
extern void DEL_Interrupt(void);
extern void DEL_Renew(void);
extern void DEL_Soft1ms(u16 iLen); 
extern void Delay_100ms(u8 T);
extern bool DEL_SetTimer(u8 cId,u16 iLen);
extern bool DEL_GetTimer(u8 cId);

#endif
