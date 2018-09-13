#ifndef __DRVKEYBOARD_H
#define __DRVKEYBOARD_H

//extern _tagKeyProg stKeyProg;

extern u8 drv_keypad_scan(void);
extern void keyboard_scan(void);
extern KeyStatesType get_keyboard_key_states(void);
extern KeyStatesType get_keyboard_down_states(void);
extern KeyStatesType get_keyboard_up_states(void);
extern KeyStatesType get_keyboard_menu_states(void);
extern KeyStatesType get_keyboard_cancel_states(void);
extern KeyStatesType get_keyboard_ok_states(void);
extern KeyStatesType get_keyboard_1_states(void);
extern KeyStatesType get_keyboard_2_states(void);
extern KeyStatesType get_keyboard_3_states(void);
extern KeyStatesType get_keyboard_4_states(void);
extern KeyStatesType get_keyboard_5_states(void);
extern KeyStatesType get_keyboard_6_states(void);
extern KeyStatesType get_keyboard_7_states(void);
extern KeyStatesType get_keyboard_8_states(void);
extern KeyStatesType get_keyboard_9_states(void);
extern KeyStatesType get_keyboard_0_states(void);
extern KeyStatesType get_keyboard_star_states(void);
extern KeyStatesType get_keyboard_well_states(void);
extern void set_keyboard_key_states(KeyStatesType a);
extern void set_keyboard_down_states(KeyStatesType a);
extern void set_keyboard_up_states(KeyStatesType a);
extern void set_keyboard_menu_states(KeyStatesType a);
extern void set_keyboard_cancel_states(KeyStatesType a);
extern void set_keyboard_ok_states(KeyStatesType a);
extern void set_keyboard_1_states(KeyStatesType a);
extern void set_keyboard_2_states(KeyStatesType a);
extern void set_keyboard_3_states(KeyStatesType a);
extern void set_keyboard_4_states(KeyStatesType a);
extern void set_keyboard_5_states(KeyStatesType a);
extern void set_keyboard_6_states(KeyStatesType a);
extern void set_keyboard_7_states(KeyStatesType a);
extern void set_keyboard_8_states(KeyStatesType a);
extern void set_keyboard_9_states(KeyStatesType a);
extern void set_keyboard_0_states(KeyStatesType a);
extern void set_keyboard_star_states(KeyStatesType a);
extern void set_keyboard_well_states(KeyStatesType a);
extern bool KEYBOAED_press_down_flag_for_backlight(void);
extern bool KEYBOAED_press_down_flag_for_keylock(void);
extern void KEYBOAED_press_down_flag_for_keylock_set(bool a);
#endif