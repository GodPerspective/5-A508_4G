#ifndef __DRVKEYSCAN_H
#define __DRVKEYSCAN_H

typedef enum{
  m_key_idle                    = 0x00,
  m_key_short_press             = 0x01,//¶Ì°´
  m_key_long_press              = 0x02,//³¤°´
  
  m_key_press_moment        = 0x03,
  m_key_loosen_moment       = 0x04,
  m_key_press_state         = 0x05,
    
}KeyStatesType;

extern void key_scan();
extern  KeyStatesType get_key_ptt_states(void);
extern KeyStatesType get_key_2_states(void);
extern KeyStatesType get_key_3_states(void);
extern KeyStatesType get_key_4_states(void);
extern void set_key_ptt_states(KeyStatesType a);
extern void set_key_2_states(KeyStatesType a);
extern void set_key_3_states(KeyStatesType a);
extern void set_key_4_states(KeyStatesType a);

#endif