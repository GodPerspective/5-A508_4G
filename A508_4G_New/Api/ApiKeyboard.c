#include "AllHead.h"

u8 MenuMode_Flag=0;
u8 BacklightTimeSetCount=1;
u8 KeylockTimeSetCount=0x11;
s8 KeyPersonalCallingCount=0;//�������¼�����
s8 PersonalCallingNum=0;
u8 MenuModeCount=1;
s8 KeyUpDownCount=0;//������¼�����
s8 GroupCallingNum=0;//ȡ�������Ⱥ������С
u8 groupCallingcount=0;//��GroupCallingNum���㣬��Χ0-4��
u8 user_calling_count=0;//��PersonalCallingNum���㣬��Χ0-4��
u8 KeyDownUpChoose_GroupOrUser_Flag=0;
bool LockingState_EnterOK_Flag=FALSE;
u8 TheMenuLayer_Flag=0;//�����˵��㼶��Ĭ��״̬��1 һ���˵���1 �����˵���2
u8 network_count;

void ApiKeyboard_PowerOnInitial(void)
{
  MenuMode_Flag=0;
  BacklightTimeSetCount=1;
  KeylockTimeSetCount=0x11;
  KeyPersonalCallingCount=0;//�������¼�����
  PersonalCallingNum=0;
  MenuModeCount=1;
  KeyUpDownCount=0;//������¼�����
  GroupCallingNum=0;//ȡ�������Ⱥ������С
  groupCallingcount=0;//��GroupCallingNum���㣬��Χ0-4��
  user_calling_count=0;//��PersonalCallingNum���㣬��Χ0-4��
  KeyDownUpChoose_GroupOrUser_Flag=0;
  LockingState_EnterOK_Flag=FALSE;
  TheMenuLayer_Flag=0;//�����˵��㼶��Ĭ��״̬��1 һ���˵���1 �����˵���2
  network_count=0;
}

void keyboard_process(void)
{
/*********����down*****************************************************************************/
    if(get_keyboard_down_states()==m_key_loosen_moment)
    {
      if(LockingState_Flag==TRUE)
      {}
      else
      {
        if(MenuMode_Flag==1)
        {
          if(ApiMenu_BacklightTimeSet_Flag==1)//��������ñ���ƶ����˵�
          {
            BacklightTimeSetCount=BacklightTimeSetCount-1;
            if(BacklightTimeSetCount<1) {BacklightTimeSetCount=7;}
            Level3MenuDisplay(BacklightTimeSetCount);
          }
          else if(ApiMenu_KeylockTimeSet_Flag==1)//��������ü����������˵�
          {
            KeylockTimeSetCount=KeylockTimeSetCount-1;
            if(KeylockTimeSetCount<0x10) {KeylockTimeSetCount=0x16;}
            Level3MenuDisplay(KeylockTimeSetCount);
          }
          else if(ApiMenu_SwitchOnlineUser_Flag==1)//�������ʾ�����û��������˵�---------------------------------------------------------------
          {
            KeyPersonalCallingCount--;
            PersonalCallingNum=KeyPersonalCallingCount;//�����������㿪ʼ
            if(PersonalCallingNum<0)
            {
              // PersonalCallingNum=ApiAtCmd_GetUserNum()-1;
              // KeyPersonalCallingCount=ApiAtCmd_GetUserNum()-1;
            }
#if 0 //WCDMA
            VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum),ApiAtCmd_GetUserNameLen(PersonalCallingNum));//�������ϼ�֮���Ӧ���û���
#endif
            api_lcd_pwr_on_hint(0,2,UNICODE,"                ");//����
            api_lcd_pwr_on_hint(0,2,UNICODE,GetAllGroupNameForDisplay(PersonalCallingNum));//��ʾ��ǰѡ�е�Ⱥ����
          }
          else if(ApiMenu_GpsInfo_Flag==1||ApiMenu_NativeInfo_Flag==1||ApiMenu_BeiDouOrWritingFrequency_Flag==1)//�����GPS��Ϣ��������Ϣ������дƵ�л������˵�
          {}
          else
          {
            MenuModeCount=MenuModeCount-1;
            if(MenuModeCount<1) {MenuModeCount=5;}
            MenuDisplay(MenuModeCount);
          }
        }
        else
        {
          if(AtCmdDrvobj.getting_info_flag==FALSE)//��������'��ȡ��״̬'ʱ����,�����ڻ�ȡ��״̬,���¼�ʧЧ
          {
            api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//Sѡ��ͼ��
            if(KEYCMD_PersonalKeyMode()==TRUE)//����ģʽ
            {
              PersonalCallingNum--;
              if(PersonalCallingNum<0)
              {
                PersonalCallingNum=GetAllOnlineUserNum()-1;
              }
              if((PersonalCallingNum+1)%APIPOC_User_Num==0||PersonalCallingNum+1==GetAllOnlineUserNum())//��up������ʱ���������Ⱥ����=5�ٽ�ʱ����ȡһ��Ⱥ���б�����+1
              {
                  AtCmdDrvobj.getting_info_flag=TRUE;
                  DISPLAY_Show(d_getting_info);
                  PocCmdDrvobj.getting_info_flag=KEYUP;
                  ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
              }
              else//����״̬
              {
                changing_user_voice_and_display(PersonalCallingNum);//����ѡ���û���
              }
              KeyDownUpChoose_GroupOrUser_Flag=2;
            }
            else
            {
              if(PocCmdDrvobj.first_exchange_group_flag==FALSE)//��һ�λ���
              {
                GroupCallingNum--;
                if(GroupCallingNum<=0)//
                {
                  GroupCallingNum=GetAllGroupNum();
                }
                if(GroupCallingNum%APIPOC_Group_Num==0||GroupCallingNum==GetAllGroupNum())//��up������ʱ���������Ⱥ����=5�ٽ�ʱ����ȡһ��Ⱥ���б�����+1
                {
                    AtCmdDrvobj.getting_info_flag=TRUE;
                    DISPLAY_Show(d_getting_info);
                    PocCmdDrvobj.getting_info_flag=KEYDOWN;
                    ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
                }
                else//����״̬
                {
                  changing_group_voice_and_display(GroupCallingNum-1);//����ʱ��������ʾ��ǰ����
                }
              }
              KeyDownUpChoose_GroupOrUser_Flag=1;
            }
          }
        }
      }
      set_keyboard_down_states(m_key_idle);
    }
/*********����up*****************************************************************************/
    if(get_keyboard_up_states()==m_key_loosen_moment)//����̰����������ǰ��־λm_key_idle
    {
      if(LockingState_Flag==TRUE)
      {}
      else
      {
        if(MenuMode_Flag==1)
        {
          if(ApiMenu_BacklightTimeSet_Flag==1)//��������ñ���ƶ����˵�
          {
            BacklightTimeSetCount=BacklightTimeSetCount+1;
            if(BacklightTimeSetCount>7) {BacklightTimeSetCount=1;}
            Level3MenuDisplay(BacklightTimeSetCount);
          }
          else if(ApiMenu_KeylockTimeSet_Flag==1)//��������ü����������˵�
          {
            KeylockTimeSetCount=KeylockTimeSetCount+1;
            if(KeylockTimeSetCount>0x16) {KeylockTimeSetCount=0x10;}
            Level3MenuDisplay(KeylockTimeSetCount);
          }
          else if(ApiMenu_SwitchOnlineUser_Flag==1)//�������ʾ�����û��������˵�---------------------------------------------------------------
          {
            KeyPersonalCallingCount++;
            PersonalCallingNum=KeyPersonalCallingCount;//�����������㿪ʼ
            KeyPersonalCallingCount=0;
            PersonalCallingNum=0;
            api_lcd_pwr_on_hint(0,2,UNICODE,"                ");//����
          }
          else if(ApiMenu_GpsInfo_Flag==1||ApiMenu_NativeInfo_Flag==1||ApiMenu_BeiDouOrWritingFrequency_Flag==1)//�����GPS��Ϣ��������Ϣ������дƵ�л������˵�
          {}
          else
          {
            MenuModeCount=MenuModeCount+1;
            if(MenuModeCount>5) {MenuModeCount=1;}
            MenuDisplay(MenuModeCount);
          }
        }
        else
        {
          if(AtCmdDrvobj.getting_info_flag==FALSE)//��������'��ȡ��״̬'ʱ����,�����ڻ�ȡ��״̬,���¼�ʧЧ
          {
            api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//Sѡ��ͼ��
            if(KEYCMD_PersonalKeyMode()==TRUE)//����ģʽ
            {
              PersonalCallingNum++;
              if(PersonalCallingNum>GetAllOnlineUserNum()-1)
              {
                PersonalCallingNum=0;
              }
              if(PersonalCallingNum%APIPOC_User_Num==0)//��up������ʱ���������Ⱥ����=5�ٽ�ʱ����ȡһ��Ⱥ���б�����+1
              {
                  AtCmdDrvobj.getting_info_flag=TRUE;
                  DISPLAY_Show(d_getting_info);
                  PocCmdDrvobj.getting_info_flag=KEYUP;
                  ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
              }
              else//����״̬
              {
                changing_user_voice_and_display(PersonalCallingNum);//����ѡ���û���
              }
              KeyDownUpChoose_GroupOrUser_Flag=2;
            }
            else
            {
              if(PocCmdDrvobj.first_exchange_group_flag==FALSE)//��һ�λ���
              {
                GroupCallingNum++;
                if(GroupCallingNum>GetAllGroupNum())//
                {
                  GroupCallingNum=1;
                }
                if(GroupCallingNum%APIPOC_Group_Num==1)//��up������ʱ���������Ⱥ����=5�ٽ�ʱ����ȡһ��Ⱥ���б�����+1
                {
                    AtCmdDrvobj.getting_info_flag=TRUE;
                    DISPLAY_Show(d_getting_info);
                    PocCmdDrvobj.getting_info_flag=KEYUP;
                    ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
                }
                else//����״̬
                {
                  changing_group_voice_and_display(GroupCallingNum-1);//����ʱ��������ʾ��ǰ����
                }
              }
  #if 0 //��ʱ��������Ҫ�ټ�
              else//��һ�λ�����¼��ǰȺ��������ţ��Ժ��鰴���¼����ɴӵ�ǰȺ��λ�û���һ�顢��һ��
              {
                KeyUpDownCount++;
                GroupCallingNum=GetNowWorkingGroupXuhao()+KeyUpDownCount;
                if(GroupCallingNum>GetAllGroupXuhao()-1)
                {
                  GroupCallingNum=0;
                  KeyUpDownCount=-GetNowWorkingGroupXuhao();
                }
                if(GroupCallingNum%APIPOC_Group_Num==0)//��up������ʱ���������Ⱥ����=5�ٽ�ʱ����ȡһ��Ⱥ���б�����+1
                {
                  DISPLAY_Show(d_getting_info);
                  PocCmdDrvobj.getting_info_flag=TRUE;
                  ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
                }
                else//����״̬
                {
                  changing_group_voice_and_display(GroupCallingNum);//����ʱ��������ʾ��ǰ����
                }
              }
  #endif
              KeyDownUpChoose_GroupOrUser_Flag=1;
            }
          }
        }
      }
      set_keyboard_up_states(m_key_idle);
    }
/*********����menu*****************************************************************************/
    if(get_keyboard_menu_states()==m_key_loosen_moment)//����̰����������ǰ��־λm_key_idle
    {
      if(LockingState_Flag==TRUE)
      {}
      else
      {
#if 0
        switch(network_count)
        {
        case 0:
          VOICE_Play(set_network_wcdma_only);
          ApiAtCmd_WritCommand(ATCOMM_ZGACT0,0,0);
          Delay_100ms(5);
          ApiAtCmd_WritCommand(ATCOMM_SetNetworkWcdmaOnly,0,0);
          network_count=1;
          break;
        case 1:
          VOICE_Play(set_network_auto);
          ApiAtCmd_WritCommand(ATCOMM_ZGACT0,0,0);
          Delay_100ms(5);
          ApiAtCmd_WritCommand(ATCOMM_SetNetworkAuto,0,0);
          network_count=0;
          break;
        default:
          network_count=0;
          break;
        }
#else
       voice_tone();
#endif
      }
      set_keyboard_menu_states(m_key_idle);
    }
 /*********����cancel*****************************************************************************/
    if(get_keyboard_cancel_states()==m_key_loosen_moment)//����̰����������ǰ��־λm_key_idle
    {
      if(LockingState_Flag==TRUE)
      {}
      else
      {
        if(MenuMode_Flag==1)
        {
          if(TheMenuLayer_Flag==2)//�����˵������ؼ�����1���˵�
          {
            if(ApiMenu_GpsInfo_Flag==1)
            {
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              ApiMenu_GpsInfo_Flag=0;
              TheMenuLayer_Flag=1;//����һ���˵�
            }
            else if(ApiMenu_SwitchOnlineUser_Flag==1)
            {
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              ApiMenu_SwitchOnlineUser_Flag=0;
              TheMenuLayer_Flag=1;//����һ���˵�
            }
            else if(ApiMenu_NativeInfo_Flag==1)
            {
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              ApiMenu_NativeInfo_Flag=0;
              TheMenuLayer_Flag=1;//����һ���˵�
            }
            else if(ApiMenu_BeiDouOrWritingFrequency_Flag==1)
            {
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              ApiMenu_BeiDouOrWritingFrequency_Flag=0;
              TheMenuLayer_Flag=1;//����һ���˵�
            }
            else if(ApiMenu_BacklightTimeSet_Flag==1)
            {
              ApiMenu_BacklightTimeSet_Flag=0;
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              TheMenuLayer_Flag=1;//����һ���˵�
            }
            else if(ApiMenu_KeylockTimeSet_Flag==1)
            {
              ApiMenu_KeylockTimeSet_Flag=0;
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              TheMenuLayer_Flag=1;//����һ���˵�
            }
            else
            {}
          }
          else if(TheMenuLayer_Flag==1)
          {
  #if 1
            MenuDisplay(Menu_RefreshAllIco);
            get_screen_display_group_name();//ѡ����ʾ��ǰȺ���ǳƣ�Ⱥ��򵥺���ʱȺ�飩
            MenuModeCount=1;
            TheMenuLayer_Flag=0;
            MenuMode_Flag=0;
            ApiMenu_SwitchGroup_Flag=0;
            ApiMenu_SwitchCallUser_Flag=0;
            ApiMenu_SwitchOnlineUser_Flag=0;
            ApiMenu_GpsInfo_Flag=0;
            ApiMenu_BacklightTimeSet_Flag=0;
            ApiMenu_KeylockTimeSet_Flag=0;
            ApiMenu_NativeInfo_Flag=0;
            ApiMenu_BeiDouOrWritingFrequency_Flag=0;
  #else
            switch(MenuModeCount)//Ĭ�ϰ�ok������һ���˵�
            {
            case 1://Ⱥ��ѡ��
              if(ApiMenu_SwitchGroup_Flag==1)//����Ϊһ���˵������ؼ�����Ĭ�Ͻ���
              {
                TheMenuLayer_Flag=0;//����һ���˵�
                MenuMode_Flag=0;
                ApiMenu_SwitchGroup_Flag=0;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//����
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              }
              break;
            case 2://��Աѡ��
              if(ApiMenu_SwitchCallUser_Flag==0)//����Ϊһ���˵������ؼ�����Ĭ�Ͻ���
              {
                TheMenuLayer_Flag=0;//����һ���˵�
                MenuMode_Flag=0;
                ApiMenu_SwitchCallUser_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//����
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              }
              break;
            case 3://GPS��Ϣ
              if(ApiMenu_GpsInfo_Flag==0)//����Ϊһ���˵������ؼ�����Ĭ�Ͻ���
              {
                TheMenuLayer_Flag=0;//����һ���˵�
                MenuMode_Flag=0;
                ApiMenu_GpsInfo_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//����
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              }
              break;
            case 4://���������
              if(ApiMenu_BacklightTimeSet_Flag==0)
              {
                TheMenuLayer_Flag=0;//����һ���˵�
                MenuMode_Flag=0;
                ApiMenu_BacklightTimeSet_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//����
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              }
              break;
            case 5://��������
              if(ApiMenu_KeylockTimeSet_Flag==0)
              {
                TheMenuLayer_Flag=0;//����һ���˵�
                MenuMode_Flag=0;
                ApiMenu_KeylockTimeSet_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//����
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              }
              break;
            case 6://������Ϣ
              if(ApiMenu_NativeInfo_Flag==0)
              {
                TheMenuLayer_Flag=0;//����һ���˵�
                MenuMode_Flag=0;
                ApiMenu_NativeInfo_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//����
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              }
              break;
            case 7://����/дƵ�л�
              if(ApiMenu_BeiDouOrWritingFrequency_Flag==0)
              {
                TheMenuLayer_Flag=0;//����һ���˵�
                MenuMode_Flag=0;
                ApiMenu_BeiDouOrWritingFrequency_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//����
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//��ʾ��ǰȺ��ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//��ʾ��ǰȺ���ǳ�
              }
              break;
            }
  #endif
          }
          else
          {}
        }
        else
        {
          if(get_current_working_status()==m_personal_mode)//����״̬�����ؼ�
          {
            ApiPocCmd_WritCommand(PocComm_EnterGroup,0,0);
          }
          return_group_and_clear_flag();//������б�־λ����Ĭ��Ⱥ��״̬
        }
      }
      set_keyboard_cancel_states(m_key_idle);
    }

 /*********����ok*****************************************************************************/
    if(get_keyboard_ok_states()==m_key_loosen_moment)//����̰����������ǰ��־λm_key_idle
    {
      if(LockingState_Flag==TRUE)
      {
        MenuDisplay(Menu_UnlockStep1_Ok);
        LockingState_EnterOK_Flag=TRUE;
        set_keyboard_key_states(m_key_idle);
      }
      else
      { 
        if(KeyDownUpChoose_GroupOrUser_Flag!=0)//������ڻ������ѡ�����״̬
        {
          switch(KeyDownUpChoose_GroupOrUser_Flag)
          {
          case 0://Ĭ��PTT״̬
            break;
          case 1://=1������ĳȺ��
            VOICE_Play(GroupSelected);
            DEL_SetTimer(0,80);
            while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
            ApiPocCmd_WritCommand(PocComm_EnterGroup,0,0);
            KeyDownUpChoose_GroupOrUser_Flag=0;
            api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//Sѡ���Ӧ��ͼ��
            KeyUpDownCount=0;
            break;
          case 2://=2,����ĳ�û�
            VOICE_Play(GroupSelected);
            DEL_SetTimer(0,80);
            while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
            ApiPocCmd_WritCommand(PocComm_Invite,0,0);
            KeyDownUpChoose_GroupOrUser_Flag=0;
            api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//Sѡ���Ӧ��ͼ��
            KeyPersonalCallingCount=0;
            break;
          case 3:
            break;
          default:
            break;
          }
        }
        else//����ͽ���˵�ģʽ
        {
          if(get_current_working_status()==m_personal_mode)
          {
            
          }
          else
          {
            switch(MenuModeCount)//Ĭ�ϰ�ok������һ���˵�
            {
            case 1://Ⱥ��ѡ��
              Key_PersonalCalling_Flag=0;//���������־λ
              switch(ApiMenu_SwitchGroup_Flag)
              {
              case 0://Ĭ��ģʽ��OK������һ���˵�
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                ApiMenu_SwitchGroup_Flag=1;
                TheMenuLayer_Flag=1;//����һ���˵�
                break;
              case 1://һ���˵��ٰ�ok��Ĭ��ģʽ
                SubmenuMenuDisplay(GroupSwitch);
                VOICE_Play(GroupMode);
                ApiMenu_SwitchGroup_Flag=0;
                TheMenuLayer_Flag=0;//����0���˵������뻻��Ϊ�˵��⹦��
                MenuMode_Flag=0;
                break;
              }
              break;
    #if 0//WCDMA
            case 2://��Աѡ��
              switch(ApiMenu_SwitchCallUser_Flag)
              {
              case 1://Ĭ�ϲ˵���OK������һ���˵�
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                ApiMenu_SwitchCallUser_Flag=0;
                TheMenuLayer_Flag=1;//����һ���˵�
                break;
              case 0://һ���˵���ok�����뵥��ģʽ
                MenuDisplay(Menu_RefreshAllIco);
                ApiMenu_SwitchCallUser_Flag=1;
                MenuMode_Flag=0;
                break;
              }
              break;
            case 3://���߳�Ա�б�
              switch(ApiMenu_SwitchOnlineUser_Flag)
              {
              case 2:
                ApiMenu_SwitchOnlineUser_Flag=0;
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                break;
              case 1://Ĭ�ϲ˵���OK������һ���˵�
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                ApiMenu_SwitchOnlineUser_Flag=2;
                TheMenuLayer_Flag=1;//����һ���˵�
                break;
              case 0://һ���˵���ok�����뵥��ģʽ
                MenuDisplay(Menu_RefreshAllIco);
                ApiMenu_SwitchOnlineUser_Flag=1;
                MenuMode_Flag=1;
                break;
              }
              break;
    #endif
            case 2://GPS����
                  switch(ApiMenu_GpsInfo_Flag)
                  {
                   //�ͻ�Ҫ��GPS��γ�ȼ�������Ϣ���水���¼���OK����Ч��ֻ�а��˳����˳�������������
                  /*case 1://�����˵���OK������һ���˵�
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    ApiMenu_GpsInfo_Flag=0;
                    TheMenuLayer_Flag=1;//����һ���˵�
                    break;*/
                  case 0://һ���˵���ok����������˵�
                    SubmenuMenuDisplay(GpsInfoMenu);
                    ApiMenu_GpsInfo_Flag=1;
                    TheMenuLayer_Flag=2;//���ڶ����˵�
                    break;
                  }
              break;
            case 3://���������
                  switch(ApiMenu_BacklightTimeSet_Flag)
                  {
                  case 2:
                    ApiMenu_BacklightTimeSet_Flag=0;
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    break;
                  case 0://��һ���˵���ok����������˵�
                    ApiMenu_BacklightTimeSet_Flag=1;//�����¼��д���
                    SubmenuMenuDisplay(BacklightTimeSet);
                    TheMenuLayer_Flag=2;//���ڶ����˵�
                    break;
                  case 1://�����˵���ok������һ���˵�
                    ApiMenu_BacklightTimeSet_Flag=2;
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    TheMenuLayer_Flag=1;//����һ���˵�
                    break;
                  }
              break;
            case 4://��������
                  switch(ApiMenu_KeylockTimeSet_Flag)
                  {
                  case 2://Ĭ��״̬��OK������һ���˵�
                    ApiMenu_KeylockTimeSet_Flag=0;
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    break;
                  case 0://��һ���˵���ok����������˵�
                    ApiMenu_KeylockTimeSet_Flag=1;//�����¼��д���
                    SubmenuMenuDisplay(KeylockTimeSet);
                    TheMenuLayer_Flag=2;//���ڶ����˵�
                    break;
                  case 1:
                    ApiMenu_KeylockTimeSet_Flag=2;
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    TheMenuLayer_Flag=1;//����һ���˵�
                    break;
                  }
              break;
            case 5://������Ϣ
                  switch(ApiMenu_NativeInfo_Flag)
                  {
                    //�ͻ�Ҫ��GPS��γ�ȼ�������Ϣ���水���¼���OK����Ч��ֻ�а��˳����˳�������������
                  /*case 1://Ĭ��״̬��OK������һ���˵�
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    ApiMenu_NativeInfo_Flag=0;
                    TheMenuLayer_Flag=1;//����һ���˵�
                    break;*/
                  case 0://��gps��Ϣһ���˵���ok����������˵�
                    SubmenuMenuDisplay(NativeInfoMenu);
                    ApiMenu_NativeInfo_Flag=1;
                    TheMenuLayer_Flag=2;//���ڶ����˵�
                    break;
                  }
              break;
            default:
              break;
            }
          }
        }
      }
      set_keyboard_ok_states(m_key_idle);
    }
 /*********����1*****************************************************************************/
    if(get_keyboard_1_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_1_states(m_key_idle);
    }
    else if(get_keyboard_1_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_1_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����2*****************************************************************************/
    if(get_keyboard_2_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_2_states(m_key_idle);
    }
    else if(get_keyboard_2_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_2_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����3****************************************************************************/
    if(get_keyboard_3_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_3_states(m_key_idle);
    }
    else if(get_keyboard_3_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_3_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����4*****************************************************************************/
    if(get_keyboard_4_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_4_states(m_key_idle);
    }
    else if(get_keyboard_4_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_4_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����5*****************************************************************************/
    if(get_keyboard_5_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_5_states(m_key_idle);
    }
    else if(get_keyboard_5_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_5_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����6*****************************************************************************/
    if(get_keyboard_6_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_6_states(m_key_idle);
    }
    else if(get_keyboard_6_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_6_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����7*****************************************************************************/
    if(get_keyboard_7_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_7_states(m_key_idle);
    }
    else if(get_keyboard_7_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_7_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����8****************************************************************************/
    if(get_keyboard_8_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_8_states(m_key_idle);
    }
    else if(get_keyboard_8_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_8_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����9*****************************************************************************/
    if(get_keyboard_9_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_9_states(m_key_idle);
    }
    else if(get_keyboard_9_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_9_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����0*****************************************************************************/
    if(get_keyboard_0_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_0_states(m_key_idle);
    }
    else if(get_keyboard_0_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_0_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����star*****************************************************************************/
    if(get_keyboard_star_states()==m_key_short_press)//����̰����������ǰ��־λm_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_star_states(m_key_idle);
    }
    else if(get_keyboard_star_states()==m_key_long_press)//���������������ǰ��־λm_key_idle
    {
      Set_RedLed(ON);
      Delay_100ms(1);
      set_keyboard_star_states(m_key_idle);
    }
    else
    {
      //Set_GreenLed(OFF);
      //Set_RedLed(OFF);
    }
  /*********����well*****************************************************************************/
    if(get_keyboard_well_states()==m_key_loosen_moment)//����̰����������ǰ��־λm_key_idle
    {
      if(LockingState_EnterOK_Flag==TRUE)//�������״̬�°���OK��Ȼ����#��
      {
        LockingState_EnterOK_Flag=FALSE;
        LockingState_Flag=FALSE;
        TimeCount=0;
        TimeCount3=0;//���������ż��ʧЧ������
        MenuDisplay(Menu_unLocking);
        api_disp_icoid_output(eICO_IDBANDWIDTHN, TRUE, TRUE);//��������ͼ��
        api_disp_all_screen_refresh();// ȫ��ͳһˢ��
      }
      set_keyboard_well_states(m_key_idle);
    }
}


void changing_group_voice_and_display(u8 a)
{
  groupCallingcount=a%APIPOC_Group_Num;
  VOICE_Play(AllGroupName);
  DISPLAY_Show(d_AllGroupName);
}
void changing_user_voice_and_display(u8 a)
{
  user_calling_count=a%APIPOC_User_Num;
  VOICE_Play(AllUserName);
  DISPLAY_Show(d_AllUserName);
}

void return_group_and_clear_flag(void)
{
  AtCmdDrvobj.getting_info_flag=FALSE;//����'��ȡ��'״̬�����ؼ�����
  ApiPocCmd_SetKeyPttState(0);//��ֹPTT����
  KEYCMD_PersonalKeyModeSet(FALSE);
  api_lcd_pwr_on_hint(0,2,GBK,"                ");//����
  api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());//��ʾ��ǰȺ���ǳ�
  KeyDownUpChoose_GroupOrUser_Flag=0;
  api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//Sѡ���Ӧ��ͼ��
  KeyUpDownCount=0;
  PocCmdDrvobj.getting_info_flag=KEYNONE;//�����ȡȺ����û�����־λ
  KEYCMD_key_2_short_states_set(m_group_mode);
  #if 1//����ʱ�����ؼ��˳�
  set_poc_receive_sos_statas(FALSE);
  ApiPocCmd_ToneStateSet(FALSE);
  AUDIO_IOAFPOW(OFF);  
  BEEP_SetOutput(BEEP_IDPowerOff,OFF,0x00,TRUE);
  #endif
  
  if(MenuMode_Flag!=0)
  {
    MenuDisplay(Menu_RefreshAllIco);
    MenuModeCount=1;
    TheMenuLayer_Flag=0;
    MenuMode_Flag=0;
    ApiMenu_SwitchGroup_Flag=0;
    ApiMenu_SwitchCallUser_Flag=0;
    ApiMenu_SwitchOnlineUser_Flag=0;
    ApiMenu_GpsInfo_Flag=0;
    ApiMenu_BacklightTimeSet_Flag=0;
    ApiMenu_KeylockTimeSet_Flag=0;
    ApiMenu_NativeInfo_Flag=0;
    ApiMenu_BeiDouOrWritingFrequency_Flag=0;
  }
}
void voice_tone(void)
{
  Set_TIM1_PWM_Frequency(3300);
  Set_TIM1_PWM1_DutyCycle(3300/2);
  AtCmdDrvobj.voice_tone_play=TRUE;
  AUDIO_IOAFPOW(ON);
  BEEP_SetOutput(BEEP_IDPowerOff,ON,0x00,TRUE);
}