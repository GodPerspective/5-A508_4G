#ifndef __APIKEYCMD_H
#define __APIKEYCMD_H

extern void ApiKeyCmd_PowerOnInitial(void);
extern void key_process(void);
extern bool KEYCMD_PersonalKeyMode(void);
extern void KEYCMD_PersonalKeyModeSet(bool a);
extern working_status_type KEYCMD_key_2_short_states(void);
extern void KEYCMD_key_2_short_states_set(working_status_type a);
extern void key_2_3_4_long_press_function_setting(u8 a);
#endif