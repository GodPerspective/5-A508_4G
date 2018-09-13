#include "AllHead.h"

TaskDrv TaskDrvobj;

u8 Key_PersonalCalling_Flag;

void Task_Init(void)
{
  TaskDrvobj.Id = TASK_LOGIN;
  TaskDrvobj.login_set_account=FALSE;
}

void Task_login_progress(void)
{
  UART3_ToMcuMain();//读取写频数据
  switch(TaskDrvobj.login_step)
  {
  case 0:
    if(AtCmdDrvobj.Msg.Bits.bCommunicationTest==1)//开启上报回复
    {
      ApiAtCmd_WritCommand(ATCOMM_ATE1,0,0);
      TaskDrvobj.login_step=1;
    }
    break;
  case 1:
    if(AtCmdDrvobj.Msg.Bits.bSimCardIn==1)//已插卡
    {
      ApiAtCmd_WritCommand(ATCOMM_DIALMODE,0,0);//设置为手动拨号
      ApiAtCmd_WritCommand(ATCOMM_CGDCONT,0,0);//设置APN（暂未设置，只是读取）
      TaskDrvobj.login_step=2;
    }
    break;
  case 2:
    if(AtCmdDrvobj.Msg.Bits.bCGDCONT==1)
    {
      ApiAtCmd_WritCommand(ATCOMM_POWERUP,0,0);//开机准备搜网
      TaskDrvobj.login_step=3;
    }
    break;
  case 3:
    if(AtCmdDrvobj.network_reg.cgreg==1||AtCmdDrvobj.network_reg.cereg==1)
    {
      ApiAtCmd_WritCommand(ATCOMM_CGACT,0,0);//PDP上下文激活
      TaskDrvobj.login_step=4;
    }
    break;
  case 4:
    if(AtCmdDrvobj.Msg.Bits.bZGIPDNS==1)//收到ZGIPDNS后开始发送指令
    {
      ApiAtCmd_WritCommand(ATCOMM_ZGACT,0,0);//PDP上下文激活
      TaskDrvobj.login_step=5;
    }
    break;
  case 5:
    if(AtCmdDrvobj.Msg.Bits.bZCONSTAT==1)//收到ZCONSTAT:1,1后表示网络链路成功，可以上网了
    {
      TaskDrvobj.login_step=6;
    }
    break;
  case 6:
    break;
  case 7:
    ApiPocCmd_WritCommand(PocComm_OpenPOC,0,0);//打开POC应用
    //ApiPocCmd_WritCommand(PocComm_SetParam,0,0);//配置登录账号密码、IP
    //ApiPocCmd_WritCommand(PocComm_SetURL,0,0);//设置URL
    TaskDrvobj.login_step=8;
    break;
  default:
    break;
  }
}



void Task_normal_progress(void)
{
  UART3_ToMcuMain();//读取写频数据
  keyboard_process();
  key_process();

/***********判断正常进组；正常退出组;被单呼模式；退出单呼模式；主动开始单呼；单呼；主动退出单呼*************/
  if(get_current_working_status()==m_group_mode)//组呼模式
  {
    if(ApiPocCmd_GroupStates()==EnterGroup)
    {
      api_lcd_pwr_on_hint(0,2,"                ");
      api_lcd_pwr_on_hint4(GetNowWorkingGroupNameForDisplay());
      ApiPocCmd_GroupStatesSet(InGroup);
    }
    else if(ApiPocCmd_GroupStates()==InGroup)
    {
    }
    else//LeaveGroup
    {
    }
  }
  else //单呼模式
  {
    if(ApiPocCmd_GroupStates()==EnterGroup)
    {
      DISPLAY_Show(d_individualcall);
      ApiPocCmd_GroupStatesSet(InGroup);
    }
    else if(ApiPocCmd_GroupStates()==InGroup)
    {
    }
    else//LeaveGroup
    {
    }
  }
  
/*********按PTT发射接收时的显示屏显示****************************************************************************************************************************/
  switch(ApiPocCmd_KeyPttState())//KeyPttState 0：未按PTT 1:按下ptt瞬间  2：按住PTT状态 3：松开PTT瞬间
  {
  case 0://未按PTT
    break;
  case 1://1:按下ptt瞬间
    ApiPocCmd_SetKeyPttState(2);
    api_disp_icoid_output( eICO_IDTX, TRUE, TRUE);//发射信号图标
    api_lcd_pwr_on_hint(0,2,"                ");
    //api_lcd_pwr_on_hint4(GetSpeakingUserNameForDisplay());//显示本机正在讲话
    api_disp_all_screen_refresh();// 全屏统一刷新
    break;
  case 2://2：按住PTT状态
    api_lcd_pwr_on_hint4(GetSpeakingUserNameForDisplay());//显示本机正在讲话
    api_disp_all_screen_refresh();// 全屏统一刷新
    break;
  case 3://3：松开PTT瞬间
    ApiPocCmd_SetKeyPttState(0);
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//默认无发射无接收信号图标
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetNowWorkingGroupNameForDisplay());//显示当前群组
    api_disp_all_screen_refresh();// 全屏统一刷新
    break;
  default:
    break;
  }
  
  switch(ApiPocCmd_ReceivedVoicePlayStatesForDisplay())
  {
  case ReceivedNone:
    break;
  case ReceivedStartVoice:
    api_lcd_pwr_on_hint(0,2,"                ");
    api_lcd_pwr_on_hint4(GetSpeakingUserNameForDisplay());
    api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//接收信号图标
    api_disp_all_screen_refresh();// 全屏统一刷新
    ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedBeingVoice);

#if 0 //解决换组或换呼状态下，被呼叫后按PTT或OK键会切换群组，而不是回复刚刚说话人的语音
    if(MenuMode_Flag!=0)
    {
      MenuDisplay(Menu_RefreshAllIco);
      MenuMode_Flag = 0;
    }
    KeyDownUpChoose_GroupOrUser_Flag=0;
    api_disp_icoid_output( eICO_IDMESSAGEOff, TRUE, TRUE);//S选择对应空图标
    KeyUpDownCount=0;
#endif
    break;
  case ReceivedBeingVoice:
    break;
  case ReceivedEndVoice:
    get_screen_display_group_name();
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//无发射无接收空图标
    api_disp_all_screen_refresh();// 全屏统一刷新
    ApiPocCmd_ReceivedVoicePlayStatesForDisplaySet(ReceivedNone);
    break;
  default:
    break;
  }
/********控制功放喇叭*************************************/
#if 0
if(ApiPocCmd_ReceivedVoicePlayStates()==TRUE)
{
  AUDIO_IOAFPOW(ON);
}
else
{
  if(poc_receive_sos_statas()==TRUE)
  {
     AUDIO_IOAFPOW(ON);
  }
  else
  {
    if(ApiAtCmd_bZTTSStates()==1)//解决报警的时候异常关闭喇叭，导致声音卡顿的文体
    {
      AUDIO_IOAFPOW(ON);
    }
    else
    {
      if(ApiPocCmd_ToneState()==TRUE)
      {
        AUDIO_IOAFPOW(ON);
      }
      else
      {
        AUDIO_IOAFPOW(OFF);
      }
    }
  }

}
#endif
}

void Task_low_battery_progress(void)
{
  UART3_ToMcuMain();//读取写频数据
}

void Task_write_freq_progress(void)
{
  UART3_ToMcuMain();//读取写频数据
  
}