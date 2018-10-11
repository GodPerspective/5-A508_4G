#include "AllHead.h"

#if 1
u8 ApiMenu_SwitchGroup_Flag=0;
u8 ApiMenu_SwitchCallUser_Flag=0;
u8 ApiMenu_SwitchOnlineUser_Flag=0;
u8 ApiMenu_GpsInfo_Flag=0;
u8 ApiMenu_NativeInfo_Flag=0;
u8 ApiMenu_BacklightTimeSet_Flag=0;
u8 ApiMenu_KeylockTimeSet_Flag=0;
u8 ApiMenu_BeiDouOrWritingFrequency_Flag=0;
#endif


void MenuDisplay(MenuDisplayType id)
{
  u8 Buf1[1];
  u8 Buf2[1];
  switch(id)
  {
  case Menu0:
    break;
  case Menu1://Ⱥ��ѡ��
    DISPLAY_Show(d_menu);
    DISPLAY_Show(d_menu_groupselect);
    break;
  case Menu2://GPS��Ϣ
    DISPLAY_Show(d_menu);
    DISPLAY_Show(d_menu_gpsinformation);
     break;
  case Menu3://�����ʱ��
    DISPLAY_Show(d_menu);
    DISPLAY_Show(d_menu_backlighttime);
     if(ApiMenu_BacklightTimeSet_Flag==2)
     {
       ApiMenu_BacklightTimeSet_Flag=0;
       if(BacklightTimeSetCount==7)
       {
         Buf1[0]=0;
         FILE_Write2(0x236,1,Buf1);//����ʱ�䡾�롿
       }
       else
       {
         Buf1[0]=BacklightTimeSetCount;
         FILE_Write2(0x236,1,Buf1);//����ʱ�䡾�롿
       }
     }
     break;
  case Menu4://������ʱ��
    DISPLAY_Show(d_menu);
    DISPLAY_Show(d_menu_keypadlocktime);
     if(ApiMenu_KeylockTimeSet_Flag==2)
     {
       ApiMenu_KeylockTimeSet_Flag=0;
       if(KeylockTimeSetCount==0x10)
       {
         //KeylockTimeCount=200;//���=200����Զ������
         Buf2[0]=KeylockTimeSetCount-0x10;
         FILE_Write2(0x247,1,Buf2);//������ʱ�䡾�롿
       }
       else
       {
         //KeylockTimeCount=(KeylockTimeSetCount-0x10)*30;
         Buf2[0]=KeylockTimeSetCount-0x10;
         FILE_Write2(0x247,1,Buf2);//������ʱ�䡾�롿
       }
     }
     break;
  case Menu5://����汾
    DISPLAY_Show(d_menu);
    DISPLAY_Show(d_menu_softwareversion);
    break;
  case Menu_Locking_NoOperation:
    MenuDisplay(Menu_RefreshAllIco);
    get_screen_display_group_name();//ѡ����ʾ��ǰȺ���ǳƣ�Ⱥ��򵥺���ʱȺ�飩
    break;
  case Menu_unLocking:
    MenuDisplay(Menu_RefreshAllIco);
    get_screen_display_group_name();//ѡ����ʾ��ǰȺ���ǳƣ�Ⱥ��򵥺���ʱȺ�飩
    break;
  case Menu_RefreshAllIco:
    NetworkModeIcons();
    HDRCSQSignalIcons();
    if(LockingState_Flag==FALSE)
      api_disp_icoid_output( eICO_IDBANDWIDTHN, TRUE, TRUE);//��������ͼ��
    else
      api_disp_icoid_output( eICO_IDBANDWIDTHW, TRUE, TRUE);//����ͼ��
    api_disp_icoid_output( BatteryLevel, TRUE, TRUE);//��ص���ͼ��
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//Ĭ���޷����޽����ź�ͼ��
//if(ShowTime_Flag==FALSE)
    {
      //if(VoiceType_FreehandOrHandset_Flag==0)
        api_disp_icoid_output( eICO_IDTemper, TRUE, TRUE);//����ģʽ
      //else
       // api_disp_icoid_output( eICO_IDMONITER, TRUE, TRUE);//��Ͳģʽͼ��
      
      if(get_current_working_status()==m_group_mode)
        api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//��ʾ���ͼ��
      else
        api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//��ʾ����ͼ��
#if 0
      if(KeyDownUpChoose_GroupOrUser_Flag==0)
        api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//��ͼ��-��ѡ��Ӧ
      else
        api_disp_icoid_output( eICO_IDLOCKED, TRUE, TRUE);//ѡ
#endif
    }
      break;
  case Menu_UnlockStep1_Ok:
    DISPLAY_Show(d_presswell);//��ʾ�밴#�ż�
    break;
  default:
    break;
  }
}

void SubmenuMenuDisplay(SubmenuMenuDisplayType id)
{
  u8 Buf1[22];//={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  u8 Buf2[22];//={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  switch(id)
  {
  case GroupSwitch: 
    MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint(0,2,GBK,"                ");//����
    api_lcd_pwr_on_hint(0,2,UNICODE,GetNowWorkingGroupNameForDisplay());//��ʾ��ǰȺ���ǳ�
    break;
  case GpsInfoMenu:
     api_lcd_pwr_on_hint(0,2,GBK,"                ");//����
    DISPLAY_Show(d_longitude_and_latitude0);
    /*if(delay_gps_value_for_display_flag2()==FALSE)
    {
      DISPLAY_Show(d_longitude_and_latitude0);
    }
    else
    {
      //���㲢��ʾ����
      Buf1[0]=0xce;
      Buf1[1]=0xb3;
      Buf1[2]=0xb6;
      Buf1[3]=0xc8;
      Buf1[4]=0x3a;
      COML_DecToAsc(poc_latitude_integer(), Buf1+5);
      COML_StringReverse(2,Buf1+5);
      Buf1[7]=0x2e;
      COML_DecToAsc(poc_latitude_float(), Buf1+8);
      COML_StringReverse(6,Buf1+8);
      Buf1[14]='\0';
      Buf1[15]='\0';
      Buf1[16]='\0';
      Buf1[17]='\0';
      Buf1[18]='\0';
      api_lcd_pwr_on_hint(0,0,Buf1);
      //���㲢��ʾγ��
      Buf2[0]=0xbe;
      Buf2[1]=0xad;
      Buf2[2]=0xb6;
      Buf2[3]=0xc8;
      Buf2[4]=0x3a;
      COML_DecToAsc(poc_longitude_integer(), Buf2+5);
      COML_StringReverse(3,Buf2+5);
      Buf2[8]=0x2e;
      COML_DecToAsc(poc_longitude_float(), Buf2+9);
      COML_StringReverse(6,Buf2+9);
      Buf2[15]='\0';
      Buf1[16]='\0';
      Buf1[17]='\0';
      Buf1[18]='\0';
      Buf1[19]='\0';
      api_lcd_pwr_on_hint(0,2,Buf2);
    }
    */
    break;
  case NativeInfoMenu:
    MCU_VERSIONForMenu();
    break;
  case BacklightTimeSet:
    BacklightTimeSetCount=read_backlight_time_value();
    Level3MenuDisplay(BacklightTimeSetCount);
    break;
  case KeylockTimeSet:
    KeylockTimeSetCount=read_key_lock_time_value()+0x10;
    Level3MenuDisplay(KeylockTimeSetCount);
    break;
  case BeiDouOrWritingFrequencySwitch:
    api_lcd_pwr_on_hint(0,0,GBK,"                ");//����
     api_lcd_pwr_on_hint(0,2,GBK,"  �Ǵ˰汾����  ");
    break;
  }
}

void Level3MenuDisplay(Level3MenuDisplayType id)
{
  switch(id)
  {
  case BacklightTimeSet_0s:
    DISPLAY_Show(d_backlight);
    api_lcd_pwr_on_hint(13,0,GBK,"7/7");
    DISPLAY_Show(d_close);
    break;
  case BacklightTimeSet_10s:
    DISPLAY_Show(d_backlight);
    api_lcd_pwr_on_hint(13,0,GBK,"1/7");
    api_lcd_pwr_on_hint(0,2,GBK,"5s              ");
    break;
  case BacklightTimeSet_20s:
    DISPLAY_Show(d_backlight);
    api_lcd_pwr_on_hint(13,0,GBK,"2/7");
    api_lcd_pwr_on_hint(0,2,GBK,"10s             ");
    break;
  case BacklightTimeSet_30s:
    DISPLAY_Show(d_backlight);
    api_lcd_pwr_on_hint(13,0,GBK,"3/7");
    api_lcd_pwr_on_hint(0,2,GBK,"15s             ");
    break;
  case BacklightTimeSet_40s:
    DISPLAY_Show(d_backlight);
    api_lcd_pwr_on_hint(13,0,GBK,"4/7");
    api_lcd_pwr_on_hint(0,2,GBK,"20s             ");
    break;
  case BacklightTimeSet_50s:
    DISPLAY_Show(d_backlight);
    api_lcd_pwr_on_hint(13,0,GBK,"5/7");
    api_lcd_pwr_on_hint(0,2,GBK,"25s             ");
    break;
  case BacklightTimeSet_60s:
    DISPLAY_Show(d_backlight);
    api_lcd_pwr_on_hint(13,0,GBK,"6/7");
    api_lcd_pwr_on_hint(0,2,GBK,"30s             ");
    break;
  case KeylockTimeSet_0s:
    DISPLAY_Show(d_keypadlock);
    api_lcd_pwr_on_hint(13,0,GBK,"7/7");
    DISPLAY_Show(d_close);
    break;
  case KeylockTimeSet_30s:
    DISPLAY_Show(d_keypadlock);
    api_lcd_pwr_on_hint(13,0,GBK,"1/7");
    api_lcd_pwr_on_hint(0,2,GBK,"5s              ");
    break;
  case KeylockTimeSet_60s:
    DISPLAY_Show(d_keypadlock);
    api_lcd_pwr_on_hint(13,0,GBK,"2/7");
    api_lcd_pwr_on_hint(0,2,GBK,"10s             ");
    break;
  case KeylockTimeSet_90s:
    DISPLAY_Show(d_keypadlock);
    api_lcd_pwr_on_hint(13,0,GBK,"3/7");
    api_lcd_pwr_on_hint(0,2,GBK,"15s             ");
    break;
  case KeylockTimeSet_120s:
    DISPLAY_Show(d_keypadlock);
    api_lcd_pwr_on_hint(13,0,GBK,"4/7");
    api_lcd_pwr_on_hint(0,2,GBK,"20s             ");
    break;
  case KeylockTimeSet_150s:
    DISPLAY_Show(d_keypadlock);
    api_lcd_pwr_on_hint(13,0,GBK,"5/7");
    api_lcd_pwr_on_hint(0,2,GBK,"25s             ");
    break;
  case KeylockTimeSet_180s:
    DISPLAY_Show(d_keypadlock);
    api_lcd_pwr_on_hint(13,0,GBK,"6/7");
    api_lcd_pwr_on_hint(0,2,GBK,"30s             ");
    break;
  default:
    break;
  }

}
