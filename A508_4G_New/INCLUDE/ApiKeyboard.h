#ifndef __APIKEYBOARD_H
#define __APIKEYBOARD_H


extern u8 BacklightTimeSetCount;
extern u8 KeylockTimeSetCount;
extern bool LockingState_EnterOK_Flag;
extern u8 TheMenuLayer_Flag;//�����˵��㼶��Ĭ��״̬��1 һ���˵���1 �����˵���2
extern u8 BacklightTimeSetCount;
extern u8 KeylockTimeSetCount;

extern s8 KeyPersonalCallingCount;//�������¼�����
extern s8 PersonalCallingNum;
extern u8 MenuModeCount;
extern s8 KeyUpDownCount;//������¼�����
extern s8 GroupCallingNum;
extern u8 groupCallingcount;//��GroupCallingNum���㣬��Χ0-4��
extern u8 user_calling_count;//��PersonalCallingNum���㣬��Χ0-4��
extern u8 MenuMode_Flag;
extern void keyboard_process(void);
extern void changing_group_voice_and_display(u8 a);
extern void changing_user_voice_and_display(u8 a);


#endif