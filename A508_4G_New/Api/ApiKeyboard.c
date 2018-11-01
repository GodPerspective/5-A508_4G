#include "AllHead.h"

u8 MenuMode_Flag=0;
u8 BacklightTimeSetCount=1;
u8 KeylockTimeSetCount=0x11;
s8 KeyPersonalCallingCount=0;//个呼上下键计数
s8 PersonalCallingNum=0;
u8 MenuModeCount=1;
s8 KeyUpDownCount=0;//组呼上下键计数
s8 GroupCallingNum=0;//取决于最大群组数大小
u8 groupCallingcount=0;//由GroupCallingNum换算，范围0-4；
u8 user_calling_count=0;//由PersonalCallingNum换算，范围0-4；
u8 KeyDownUpChoose_GroupOrUser_Flag=0;
bool LockingState_EnterOK_Flag=FALSE;
u8 TheMenuLayer_Flag=0;//所处菜单层级；默认状态：1 一级菜单：1 二级菜单：2
u8 network_count;

void ApiKeyboard_PowerOnInitial(void)
{
  MenuMode_Flag=0;
  BacklightTimeSetCount=1;
  KeylockTimeSetCount=0x11;
  KeyPersonalCallingCount=0;//个呼上下键计数
  PersonalCallingNum=0;
  MenuModeCount=1;
  KeyUpDownCount=0;//组呼上下键计数
  GroupCallingNum=0;//取决于最大群组数大小
  groupCallingcount=0;//由GroupCallingNum换算，范围0-4；
  user_calling_count=0;//由PersonalCallingNum换算，范围0-4；
  KeyDownUpChoose_GroupOrUser_Flag=0;
  LockingState_EnterOK_Flag=FALSE;
  TheMenuLayer_Flag=0;//所处菜单层级；默认状态：1 一级菜单：1 二级菜单：2
  network_count=0;
}

void keyboard_process(void)
{
/*********键盘down*****************************************************************************/
    if(get_keyboard_down_states()==m_key_loosen_moment)
    {
      if(LockingState_Flag==TRUE)
      {}
      else
      {
        if(MenuMode_Flag==1)
        {
          if(ApiMenu_BacklightTimeSet_Flag==1)//如果是设置背光灯二级菜单
          {
            BacklightTimeSetCount=BacklightTimeSetCount-1;
            if(BacklightTimeSetCount<1) {BacklightTimeSetCount=7;}
            Level3MenuDisplay(BacklightTimeSetCount);
          }
          else if(ApiMenu_KeylockTimeSet_Flag==1)//如果是设置键盘锁二级菜单
          {
            KeylockTimeSetCount=KeylockTimeSetCount-1;
            if(KeylockTimeSetCount<0x10) {KeylockTimeSetCount=0x16;}
            Level3MenuDisplay(KeylockTimeSetCount);
          }
          else if(ApiMenu_SwitchOnlineUser_Flag==1)//如果是显示在线用户名二级菜单---------------------------------------------------------------
          {
            KeyPersonalCallingCount--;
            PersonalCallingNum=KeyPersonalCallingCount;//个呼计数从零开始
            if(PersonalCallingNum<0)
            {
              // PersonalCallingNum=ApiAtCmd_GetUserNum()-1;
              // KeyPersonalCallingCount=ApiAtCmd_GetUserNum()-1;
            }
#if 0 //WCDMA
            VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum),ApiAtCmd_GetUserNameLen(PersonalCallingNum));//播报按上键之后对应的用户名
#endif
            api_lcd_pwr_on_hint(0,2,UNICODE,"                ");//清屏
            api_lcd_pwr_on_hint(0,2,UNICODE,GetAllGroupNameForDisplay(PersonalCallingNum));//显示当前选中的群组名
          }
          else if(ApiMenu_GpsInfo_Flag==1||ApiMenu_NativeInfo_Flag==1||ApiMenu_BeiDouOrWritingFrequency_Flag==1)//如果是GPS信息、本机信息、北斗写频切换二级菜单
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
          if(AtCmdDrvobj.getting_info_flag==FALSE)//当不处于'获取中状态'时进入,当处于获取中状态,上下键失效
          {
            api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//S选择图标
            if(KEYCMD_PersonalKeyMode()==TRUE)//单呼模式
            {
              PersonalCallingNum--;
              if(PersonalCallingNum<0)
              {
                PersonalCallingNum=GetAllOnlineUserNum()-1;
              }
              if((PersonalCallingNum+1)%APIPOC_User_Num==0||PersonalCallingNum+1==GetAllOnlineUserNum())//按up键换组时，处于最大群组数=5临界时，获取一次群组列表，计数+1
              {
                  AtCmdDrvobj.getting_info_flag=TRUE;
                  DISPLAY_Show(d_getting_info);
                  PocCmdDrvobj.getting_info_flag=KEYUP;
                  ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
              }
              else//正常状态
              {
                changing_user_voice_and_display(PersonalCallingNum);//播报选中用户名
              }
              KeyDownUpChoose_GroupOrUser_Flag=2;
            }
            else
            {
              if(PocCmdDrvobj.first_exchange_group_flag==FALSE)//第一次换组
              {
                GroupCallingNum--;
                if(GroupCallingNum<=0)//
                {
                  GroupCallingNum=GetAllGroupNum();
                }
                if(GroupCallingNum%APIPOC_Group_Num==0||GroupCallingNum==GetAllGroupNum())//按up键换组时，处于最大群组数=5临界时，获取一次群组列表，计数+1
                {
                    AtCmdDrvobj.getting_info_flag=TRUE;
                    DISPLAY_Show(d_getting_info);
                    PocCmdDrvobj.getting_info_flag=KEYDOWN;
                    ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
                }
                else//正常状态
                {
                  changing_group_voice_and_display(GroupCallingNum-1);//换组时播报和显示当前组名
                }
              }
              KeyDownUpChoose_GroupOrUser_Flag=1;
            }
          }
        }
      }
      set_keyboard_down_states(m_key_idle);
    }
/*********键盘up*****************************************************************************/
    if(get_keyboard_up_states()==m_key_loosen_moment)//处理短按按键清除当前标志位m_key_idle
    {
      if(LockingState_Flag==TRUE)
      {}
      else
      {
        if(MenuMode_Flag==1)
        {
          if(ApiMenu_BacklightTimeSet_Flag==1)//如果是设置背光灯二级菜单
          {
            BacklightTimeSetCount=BacklightTimeSetCount+1;
            if(BacklightTimeSetCount>7) {BacklightTimeSetCount=1;}
            Level3MenuDisplay(BacklightTimeSetCount);
          }
          else if(ApiMenu_KeylockTimeSet_Flag==1)//如果是设置键盘锁二级菜单
          {
            KeylockTimeSetCount=KeylockTimeSetCount+1;
            if(KeylockTimeSetCount>0x16) {KeylockTimeSetCount=0x10;}
            Level3MenuDisplay(KeylockTimeSetCount);
          }
          else if(ApiMenu_SwitchOnlineUser_Flag==1)//如果是显示在线用户名二级菜单---------------------------------------------------------------
          {
            KeyPersonalCallingCount++;
            PersonalCallingNum=KeyPersonalCallingCount;//个呼计数从零开始
            KeyPersonalCallingCount=0;
            PersonalCallingNum=0;
            api_lcd_pwr_on_hint(0,2,UNICODE,"                ");//清屏
          }
          else if(ApiMenu_GpsInfo_Flag==1||ApiMenu_NativeInfo_Flag==1||ApiMenu_BeiDouOrWritingFrequency_Flag==1)//如果是GPS信息、本机信息、北斗写频切换二级菜单
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
          if(AtCmdDrvobj.getting_info_flag==FALSE)//当不处于'获取中状态'时进入,当处于获取中状态,上下键失效
          {
            api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//S选择图标
            if(KEYCMD_PersonalKeyMode()==TRUE)//单呼模式
            {
              PersonalCallingNum++;
              if(PersonalCallingNum>GetAllOnlineUserNum()-1)
              {
                PersonalCallingNum=0;
              }
              if(PersonalCallingNum%APIPOC_User_Num==0)//按up键换组时，处于最大群组数=5临界时，获取一次群组列表，计数+1
              {
                  AtCmdDrvobj.getting_info_flag=TRUE;
                  DISPLAY_Show(d_getting_info);
                  PocCmdDrvobj.getting_info_flag=KEYUP;
                  ApiPocCmd_WritCommand(PocComm_UserListInfo,0,0);
              }
              else//正常状态
              {
                changing_user_voice_and_display(PersonalCallingNum);//播报选中用户名
              }
              KeyDownUpChoose_GroupOrUser_Flag=2;
            }
            else
            {
              if(PocCmdDrvobj.first_exchange_group_flag==FALSE)//第一次换组
              {
                GroupCallingNum++;
                if(GroupCallingNum>GetAllGroupNum())//
                {
                  GroupCallingNum=1;
                }
                if(GroupCallingNum%APIPOC_Group_Num==1)//按up键换组时，处于最大群组数=5临界时，获取一次群组列表，计数+1
                {
                    AtCmdDrvobj.getting_info_flag=TRUE;
                    DISPLAY_Show(d_getting_info);
                    PocCmdDrvobj.getting_info_flag=KEYUP;
                    ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
                }
                else//正常状态
                {
                  changing_group_voice_and_display(GroupCallingNum-1);//换组时播报和显示当前组名
                }
              }
  #if 0 //暂时屏蔽有需要再加
              else//第一次换组后记录当前群组名的序号，以后换组按上下键即可从当前群组位置换上一组、下一组
              {
                KeyUpDownCount++;
                GroupCallingNum=GetNowWorkingGroupXuhao()+KeyUpDownCount;
                if(GroupCallingNum>GetAllGroupXuhao()-1)
                {
                  GroupCallingNum=0;
                  KeyUpDownCount=-GetNowWorkingGroupXuhao();
                }
                if(GroupCallingNum%APIPOC_Group_Num==0)//按up键换组时，处于最大群组数=5临界时，获取一次群组列表，计数+1
                {
                  DISPLAY_Show(d_getting_info);
                  PocCmdDrvobj.getting_info_flag=TRUE;
                  ApiPocCmd_WritCommand(PocComm_GroupListInfo,0,0);
                }
                else//正常状态
                {
                  changing_group_voice_and_display(GroupCallingNum);//换组时播报和显示当前组名
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

    
/*********键盘menu*****************************************************************************/
    if(get_keyboard_menu_states()==m_key_loosen_moment)//处理短按按键清除当前标志位m_key_idle
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
 /*********键盘cancel*****************************************************************************/
    if(get_keyboard_cancel_states()==m_key_loosen_moment)//处理短按按键清除当前标志位m_key_idle
    {
      if(LockingState_Flag==TRUE)
      {}
      else
      {
        if(MenuMode_Flag==1)
        {
          if(TheMenuLayer_Flag==2)//二级菜单按返回键返回1级菜单
          {
            if(ApiMenu_GpsInfo_Flag==1)
            {
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              ApiMenu_GpsInfo_Flag=0;
              TheMenuLayer_Flag=1;//处于一级菜单
            }
            else if(ApiMenu_SwitchOnlineUser_Flag==1)
            {
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              ApiMenu_SwitchOnlineUser_Flag=0;
              TheMenuLayer_Flag=1;//处于一级菜单
            }
            else if(ApiMenu_NativeInfo_Flag==1)
            {
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              ApiMenu_NativeInfo_Flag=0;
              TheMenuLayer_Flag=1;//处于一级菜单
            }
            else if(ApiMenu_BeiDouOrWritingFrequency_Flag==1)
            {
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              ApiMenu_BeiDouOrWritingFrequency_Flag=0;
              TheMenuLayer_Flag=1;//处于一级菜单
            }
            else if(ApiMenu_BacklightTimeSet_Flag==1)
            {
              ApiMenu_BacklightTimeSet_Flag=0;
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              TheMenuLayer_Flag=1;//处于一级菜单
            }
            else if(ApiMenu_KeylockTimeSet_Flag==1)
            {
              ApiMenu_KeylockTimeSet_Flag=0;
              MenuDisplay(MenuModeCount);
              MenuMode_Flag=1;
              TheMenuLayer_Flag=1;//处于一级菜单
            }
            else
            {}
          }
          else if(TheMenuLayer_Flag==1)
          {
  #if 1
            MenuDisplay(Menu_RefreshAllIco);
            get_screen_display_group_name();//选择显示当前群组昵称（群组或单呼临时群组）
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
            switch(MenuModeCount)//默认按ok键进入一级菜单
            {
            case 1://群组选择
              if(ApiMenu_SwitchGroup_Flag==1)//以下为一级菜单按返回键进入默认界面
              {
                TheMenuLayer_Flag=0;//处于一级菜单
                MenuMode_Flag=0;
                ApiMenu_SwitchGroup_Flag=0;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//清屏
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
              }
              break;
            case 2://成员选择
              if(ApiMenu_SwitchCallUser_Flag==0)//以下为一级菜单按返回键进入默认界面
              {
                TheMenuLayer_Flag=0;//处于一级菜单
                MenuMode_Flag=0;
                ApiMenu_SwitchCallUser_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//清屏
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
              }
              break;
            case 3://GPS信息
              if(ApiMenu_GpsInfo_Flag==0)//以下为一级菜单按返回键进入默认界面
              {
                TheMenuLayer_Flag=0;//处于一级菜单
                MenuMode_Flag=0;
                ApiMenu_GpsInfo_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//清屏
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
              }
              break;
            case 4://背光灯设置
              if(ApiMenu_BacklightTimeSet_Flag==0)
              {
                TheMenuLayer_Flag=0;//处于一级菜单
                MenuMode_Flag=0;
                ApiMenu_BacklightTimeSet_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//清屏
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
              }
              break;
            case 5://键盘锁定
              if(ApiMenu_KeylockTimeSet_Flag==0)
              {
                TheMenuLayer_Flag=0;//处于一级菜单
                MenuMode_Flag=0;
                ApiMenu_KeylockTimeSet_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//清屏
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
              }
              break;
            case 6://本机信息
              if(ApiMenu_NativeInfo_Flag==0)
              {
                TheMenuLayer_Flag=0;//处于一级菜单
                MenuMode_Flag=0;
                ApiMenu_NativeInfo_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//清屏
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
              }
              break;
            case 7://北斗/写频切换
              if(ApiMenu_BeiDouOrWritingFrequency_Flag==0)
              {
                TheMenuLayer_Flag=0;//处于一级菜单
                MenuMode_Flag=0;
                ApiMenu_BeiDouOrWritingFrequency_Flag=1;
                MenuDisplay(Menu_RefreshAllIco);
                api_lcd_pwr_on_hint("                ");//清屏
                api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
                api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
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
          if(get_current_working_status()==m_personal_mode)//单呼状态按返回键
          {
            ApiPocCmd_WritCommand(PocComm_EnterGroup,0,0);
          }
          return_group_and_clear_flag();//清空所有标志位返回默认群组状态
        }
      }
      set_keyboard_cancel_states(m_key_idle);
    }

 /*********键盘ok*****************************************************************************/
    if(get_keyboard_ok_states()==m_key_loosen_moment)//处理短按按键清除当前标志位m_key_idle
    {
      if(LockingState_Flag==TRUE)
      {
        MenuDisplay(Menu_UnlockStep1_Ok);
        LockingState_EnterOK_Flag=TRUE;
        set_keyboard_key_states(m_key_idle);
      }
      else
      { 
        if(KeyDownUpChoose_GroupOrUser_Flag!=0)//如果处于换组或者选择个呼状态
        {
          switch(KeyDownUpChoose_GroupOrUser_Flag)
          {
          case 0://默认PTT状态
            break;
          case 1://=1，进入某群组
            VOICE_Play(GroupSelected);
            DEL_SetTimer(0,40);
            while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
            ApiPocCmd_WritCommand(PocComm_EnterGroup,0,0);
            KeyDownUpChoose_GroupOrUser_Flag=0;
            api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//S选择对应空图标
            KeyUpDownCount=0;
            break;
          case 2://=2,呼叫某用户
            VOICE_Play(GroupSelected);
            DEL_SetTimer(0,40);
            while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
            ApiPocCmd_WritCommand(PocComm_Invite,0,0);
            KeyDownUpChoose_GroupOrUser_Flag=0;
            api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//S选择对应空图标
            KeyPersonalCallingCount=0;
            break;
          case 3:
            break;
          default:
            break;
          }
        }
        else//否则就进入菜单模式
        {
          if(get_current_working_status()==m_personal_mode)
          {
            
          }
          else
          {
            switch(MenuModeCount)//默认按ok键进入一级菜单
            {
            case 1://群组选择
              Key_PersonalCalling_Flag=0;//进入组呼标志位
              switch(ApiMenu_SwitchGroup_Flag)
              {
              case 0://默认模式按OK键进入一级菜单
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                ApiMenu_SwitchGroup_Flag=1;
                TheMenuLayer_Flag=1;//处于一级菜单
                break;
              case 1://一级菜单再按ok键默认模式
                SubmenuMenuDisplay(GroupSwitch);
                VOICE_Play(GroupMode);
                ApiMenu_SwitchGroup_Flag=0;
                TheMenuLayer_Flag=0;//处于0级菜单，进入换组为菜单外功能
                MenuMode_Flag=0;
                break;
              }
              break;
    #if 0//WCDMA
            case 2://成员选择
              switch(ApiMenu_SwitchCallUser_Flag)
              {
              case 1://默认菜单按OK键进入一级菜单
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                ApiMenu_SwitchCallUser_Flag=0;
                TheMenuLayer_Flag=1;//处于一级菜单
                break;
              case 0://一级菜单按ok键进入单呼模式
                MenuDisplay(Menu_RefreshAllIco);
                ApiMenu_SwitchCallUser_Flag=1;
                MenuMode_Flag=0;
                break;
              }
              break;
            case 3://在线成员列表
              switch(ApiMenu_SwitchOnlineUser_Flag)
              {
              case 2:
                ApiMenu_SwitchOnlineUser_Flag=0;
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                break;
              case 1://默认菜单按OK键进入一级菜单
                MenuDisplay(MenuModeCount);
                MenuMode_Flag=1;
                ApiMenu_SwitchOnlineUser_Flag=2;
                TheMenuLayer_Flag=1;//处于一级菜单
                break;
              case 0://一级菜单按ok键进入单呼模式
                MenuDisplay(Menu_RefreshAllIco);
                ApiMenu_SwitchOnlineUser_Flag=1;
                MenuMode_Flag=1;
                break;
              }
              break;
    #endif
            case 2://GPS设置
                  switch(ApiMenu_GpsInfo_Flag)
                  {
                   //客户要求GPS经纬度及本机信息界面按上下键和OK键无效，只有按退出键退出，故屏蔽以下
                  /*case 1://二级菜单按OK键进入一级菜单
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    ApiMenu_GpsInfo_Flag=0;
                    TheMenuLayer_Flag=1;//处于一级菜单
                    break;*/
                  case 0://一级菜单按ok键进入二级菜单
                    SubmenuMenuDisplay(GpsInfoMenu);
                    ApiMenu_GpsInfo_Flag=1;
                    TheMenuLayer_Flag=2;//处于二级菜单
                    break;
                  }
              break;
            case 3://背光灯设置
                  switch(ApiMenu_BacklightTimeSet_Flag)
                  {
                  case 2:
                    ApiMenu_BacklightTimeSet_Flag=0;
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    break;
                  case 0://在一级菜单按ok键进入二级菜单
                    ApiMenu_BacklightTimeSet_Flag=1;//在上下键中处理
                    SubmenuMenuDisplay(BacklightTimeSet);
                    TheMenuLayer_Flag=2;//处于二级菜单
                    break;
                  case 1://二级菜单按ok键进入一级菜单
                    ApiMenu_BacklightTimeSet_Flag=2;
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    TheMenuLayer_Flag=1;//处于一级菜单
                    break;
                  }
              break;
            case 4://键盘锁定
                  switch(ApiMenu_KeylockTimeSet_Flag)
                  {
                  case 2://默认状态按OK键进入一级菜单
                    ApiMenu_KeylockTimeSet_Flag=0;
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    break;
                  case 0://在一级菜单按ok键进入二级菜单
                    ApiMenu_KeylockTimeSet_Flag=1;//在上下键中处理
                    SubmenuMenuDisplay(KeylockTimeSet);
                    TheMenuLayer_Flag=2;//处于二级菜单
                    break;
                  case 1:
                    ApiMenu_KeylockTimeSet_Flag=2;
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    TheMenuLayer_Flag=1;//处于一级菜单
                    break;
                  }
              break;
            case 5://本机信息
                  switch(ApiMenu_NativeInfo_Flag)
                  {
                    //客户要求GPS经纬度及本机信息界面按上下键和OK键无效，只有按退出键退出，故屏蔽以下
                  /*case 1://默认状态按OK键进入一级菜单
                    MenuDisplay(MenuModeCount);
                    MenuMode_Flag=1;
                    ApiMenu_NativeInfo_Flag=0;
                    TheMenuLayer_Flag=1;//处于一级菜单
                    break;*/
                  case 0://在gps信息一级菜单按ok键进入二级菜单
                    SubmenuMenuDisplay(NativeInfoMenu);
                    ApiMenu_NativeInfo_Flag=1;
                    TheMenuLayer_Flag=2;//处于二级菜单
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
 /*********键盘1*****************************************************************************/
    if(get_keyboard_1_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_1_states(m_key_idle);
    }
    else if(get_keyboard_1_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘2*****************************************************************************/
    if(get_keyboard_2_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_2_states(m_key_idle);
    }
    else if(get_keyboard_2_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘3****************************************************************************/
    if(get_keyboard_3_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_3_states(m_key_idle);
    }
    else if(get_keyboard_3_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘4*****************************************************************************/
    if(get_keyboard_4_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_4_states(m_key_idle);
    }
    else if(get_keyboard_4_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘5*****************************************************************************/
    if(get_keyboard_5_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_5_states(m_key_idle);
    }
    else if(get_keyboard_5_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘6*****************************************************************************/
    if(get_keyboard_6_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_6_states(m_key_idle);
    }
    else if(get_keyboard_6_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘7*****************************************************************************/
    if(get_keyboard_7_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_7_states(m_key_idle);
    }
    else if(get_keyboard_7_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘8****************************************************************************/
    if(get_keyboard_8_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_8_states(m_key_idle);
    }
    else if(get_keyboard_8_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘9*****************************************************************************/
    if(get_keyboard_9_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_9_states(m_key_idle);
    }
    else if(get_keyboard_9_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘0*****************************************************************************/
    if(get_keyboard_0_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_0_states(m_key_idle);
    }
    else if(get_keyboard_0_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘star*****************************************************************************/
    if(get_keyboard_star_states()==m_key_short_press)//处理短按按键清除当前标志位m_key_idle
    {
      Set_GreenLed(ON);
      Delay_100ms(1);
      set_keyboard_star_states(m_key_idle);
    }
    else if(get_keyboard_star_states()==m_key_long_press)//处理长按按键清除当前标志位m_key_idle
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
  /*********键盘well*****************************************************************************/
    if(get_keyboard_well_states()==m_key_loosen_moment)//处理短按按键清除当前标志位m_key_idle
    {
      if(LockingState_EnterOK_Flag==TRUE)//如果锁定状态下按了OK键然后按了#键
      {
        LockingState_EnterOK_Flag=FALSE;
        LockingState_Flag=FALSE;
        TimeCount=0;
        TimeCount3=0;//解决锁屏键偶尔失效的问题
        MenuDisplay(Menu_unLocking);
        api_disp_icoid_output(eICO_IDBANDWIDTHN, TRUE, TRUE);//无锁屏空图标
        api_disp_all_screen_refresh();// 全屏统一刷新
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
  AtCmdDrvobj.getting_info_flag=FALSE;//处于'获取中'状态按返回键返回
  ApiPocCmd_SetKeyPttState(0);//防止PTT卡死
  KEYCMD_PersonalKeyModeSet(FALSE);
  api_lcd_pwr_on_hint(0,2,GBK,"                ");//清屏
  api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());//显示当前群组昵称
  KeyDownUpChoose_GroupOrUser_Flag=0;
  api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//S选择对应空图标
  KeyUpDownCount=0;
  PocCmdDrvobj.getting_info_flag=KEYNONE;//清除获取群组或用户名标志位
  KEYCMD_key_2_short_states_set(m_group_mode);
  #if 1//报警时按返回键退出
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