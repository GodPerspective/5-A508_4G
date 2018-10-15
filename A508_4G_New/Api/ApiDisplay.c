#include "AllHead.h"
#include <string.h>

/*-----------------------------------------------------------------------------
 Macro Define (macro definitions used in this file internally)
-----------------------------------------------------------------------------*/
const u8 CHAR_HIGH[2][8]  = { 0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
							  0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02 };

const u8 CHAR_WIDTH[2][8] = { 0x08, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10,
							  0x08, 0x10, 0x06, 0x08, 0x08, 0x08, 0x10, 0x10 };


#define DISPRELAOD_TIMER_PERIOD ( 10*OS_TICKS_PER_10MS )  /* 定时周期，每100ms回调一次函数 */
#define LCD_ALIAS_LEN_MAX 10
u16 CharCode;
typedef struct
{
	union
	{
		struct
		{
			u32	bRefresh	: 1;
			u32	bAllRef		: 1;
		}Bit;
		u32	Byte;
	}Msg;
}DIS_DRV;

//static DIS_DRV DisDrvObj;

static void DISP_DataBuf(DISP_CHAR DisInfo, u8 *CharData);
static void DISP_MulTypePro(DISP_CHAR CharInfo, u8 *CharData);
static void DISP_MulTypePro2(DISP_CHAR CharInfo, u8 *CharData);//UNICODE显示
/* ------------------------------------------------------------------------------- */
/* 因编写以上代码时,以下函数尚未定义，故临时定义（便于编译通过），后续请重新编写   */
/* ------------------------------------------------------------------------------- */
const u8 BitTab[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };


u8 DisDataBit[64]  = {0};
u8 DisDataBuf[512] = {0};
const u8 Ico_DataBuf[32][32]=
{
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//0-电池电量0

{0x00,0xF8,0x08,0xE8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//1-电池电量1

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//2-电池电量2

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,   //3-电池电量3
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0x08,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//4-电池电量4

{0x00,0xF8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xE8,0x08,0xF8,0xE0,0x00,
0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//5-电池电量5

/*
{0x00,0x00,0xE0,0x3C,0x22,0xA2,0xA2,0xA2,0xA2,0xA2,0x22,0x3C,0xE0,0x00,0x00,0x00,
0x00,0x00,0x0F,0x08,0x08,0x08,0x08,0x0B,0x08,0x08,0x08,0x08,0x0F,0x00,0x00,0x00},//6

{0x00,0x00,0xFC,0x0C,0x14,0x24,0x44,0x84,0x84,0x44,0x24,0x14,0x0C,0xFC,0x00,0x00,
0x00,0x00,0x07,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x07,0x00,0x00},//7
 
{0x00,0x02,0x0E,0x12,0x22,0xFE,0x22,0x12,0x8E,0x02,0x00,0xF0,0x00,0x00,0xFE,0x00,
0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x0F,0x00,0x00,0x0F,0x00,0x00,0x0F,0x00 },//8
 
{0x00,0x02,0x0E,0x12,0x22,0xFE,0x22,0x12,0x0E,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },//9
 
{0x00,0x00,0xB8,0x30,0x28,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x84,0x88,0xB0,0x00,
0x00,0x00,0x01,0x02,0x04,0x04,0x08,0x08,0x08,0x08,0x08,0x04,0x02,0x01,0x03,0x00 },//10
 
{0x00,0x00,0xB8,0x30,0x28,0x04,0x42,0x42,0xF2,0x42,0x42,0x04,0x84,0x88,0xB0,0x00,
0x00,0x00,0x01,0x02,0x04,0x04,0x08,0x08,0x09,0x08,0x08,0x04,0x02,0x01,0x03,0x00 },//11
 
{0x00,0xF8,0x08,0xF8,0x04,0x02,0xFE,0x00,0x10,0xE0,0x08,0xF0,0x04,0xF8,0x00,0x00,
0x00,0x03,0x02,0x03,0x04,0x08,0x0F,0x00,0x01,0x00,0x02,0x01,0x04,0x03,0x00,0x00 },//12*/

#if 1//国外版选显示为“s”
{0X00,0X00,0X00,0X00,0X38,0X7C,0XEC,0XCC,0XCC,0X9C,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X06,0X06,0X06,0X07,0X03,0X00,0X00,0X00,0X00,0X00,0X00,//选//6
},
#else
{0X00,0X00,0X10,0X11,0XF2,0X00,0X28,0X26,0XE4,0X3F,0XE4,0X24,0X20,0X00,0X00,0X00,
0X00,0X00,0X08,0X04,0X03,0X04,0X0A,0X09,0X08,0X08,0X09,0X0A,0X0B,0X00,0X00,0X00,//选//6
},
#endif

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//0格信号//7

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//1格信号//8

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//2格信号//9

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X0F,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00},//3格信号//10

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0XC0,0X00,0XF0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X0F,0X00,0X0F,0X00,0X0F,0X00,0X00,0X00,0X00},//4格信号//11

{0X04,0X0C,0X14,0XFC,0X14,0X0C,0X04,0X00,0X00,0XC0,0X00,0XF0,0X00,0XFC,0X00,0X00,
0X00,0X00,0X00,0X0F,0X00,0X0C,0X00,0X0F,0X00,0X0F,0X00,0X0F,0X00,0X0F,0X00,0X00},//5格信号//12

 
{0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//无发射无接收状态（空图标）//13
 
{0X00,0X7C,0X82,0X39,0X44,0X82,0X10,0XB8,0X10,0X82,0X44,0X39,0X82,0X7C,0X00,0X00,
0X00,0X00,0X00,0X01,0X00,0X00,0X0C,0X0F,0X0C,0X00,0X00,0X01,0X00,0X00,0X00,0X00},//发射新//14

{0X82,0X44,0X39,0X82,0X7C,0X00,0X10,0XB8,0X10,0X00,0X7C,0X82,0X39,0X44,0X82,0X00,
0X00,0X00,0X01,0X00,0X00,0X00,0X0C,0X0F,0X0C,0X00,0X00,0X00,0X01,0X00,0X00,0X00},//接收新//15

 {0X00,0X00,0X00,0X00,0X48,0X48,0XB8,0X00,0XE0,0X10,0X08,0X48,0X48,0XD0,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X02,0X01,0X00,0X00,0X01,0X02,0X02,0X02,0X03,0X00,0X00},//3G//16

{0X00,0X00,0X00,0X10,0X08,0X88,0X70,0X00,0XE0,0X10,0X08,0X48,0X48,0XD0,0X00,0X00,
0X00,0X00,0X00,0X02,0X03,0X02,0X02,0X00,0X00,0X01,0X02,0X02,0X02,0X03,0X00,0X00},//2G//17

 
{0XC0,0XE0,0XEC,0XFE,0XFE,0XEC,0XC0,0X80,0XC0,0XEC,0XFE,0XFE,0XEC,0XE0,0XC0,0X00,
0X01,0X03,0X03,0X03,0X03,0X03,0X01,0X01,0X01,0X03,0X03,0X03,0X03,0X03,0X01,0X00},//组呼//18
 
{0X00,0X00,0X00,0X00,0XC0,0XE0,0XEC,0XFE,0XFE,0XEC,0XE0,0XC0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X03,0X03,0X03,0X03,0X03,0X03,0X01,0X00,0X00,0X00,0X00},//个呼//19

{0X00,0X00,0XF0,0XF0,0XF0,0XF8,0XFC,0XFE,0X00,0X08,0XF0,0X02,0X04,0XF8,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0X03,0X07,0X00,0X01,0X00,0X04,0X02,0X01,0X00,0X00},//免提模式//20

{0X00,0X00,0X00,0X0E,0X1F,0X3F,0X66,0XC0,0X80,0X00,0X80,0XC0,0XC0,0X80,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X03,0X03,0X03,0X03,0X01,0X00,0X00},//听筒模式//21
 
{0X00,0X00,0XF0,0X08,0X0E,0X09,0X69,0XE9,0X69,0X09,0X0E,0X08,0XF0,0X00,0X00,0X00,
0X00,0X00,0X03,0X04,0X04,0X04,0X04,0X05,0X04,0X04,0X04,0X04,0X03,0X00,0X00,0X00},//锁屏图标//22
 
{0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//空图标//23
 
 
{0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,//空白图标（与选ico对应）//24
},

{0X00,0XC0,0XA0,0X90,0X8C,0XFC,0X80,0X00,0XF0,0X08,0X04,0X44,0X44,0XC8,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X03,0X00,0X00,0X00,0X01,0X02,0X02,0X02,0X03,0X00,0X00},//25-4G

{0X00,0X00,0XFC,0X24,0X24,0X24,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X01,0X01,0X01,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//26-E
 
{0X00,0X00,0XFC,0X20,0X20,0X20,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X01,0X00,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//27-H
 
{0X00,0X00,0XF8,0X40,0X40,0X40,0XF8,0X00,0X20,0X20,0XF8,0X20,0X20,0X00,0X00,0X00,
0X00,0X00,0X03,0X00,0X00,0X00,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//28-H+
 
{0X00,0X00,0X04,0X04,0XFC,0X04,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//29-T
 
{0X00,0X00,0X88,0X50,0X20,0X50,0X88,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},//30-x
 
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xE0,0x00,
 0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00},//31
};


const u8 *dp_abell              = "   欧标对讲机   ";//1
const u8 *dp_personal_mode      = "   个呼模式     ";//2
const u8 *dp_no_sim_card        = "   请插卡       ";//3
const u8 *dp_network_search     = "搜索网络...     ";//4
const u8 *dp_account_config     = "正在登录...     ";//5
const u8 *dp_please_charge      = "电量低,请充电   ";//6
const u8 *dp_menu               = "菜单            ";//7
const u8 *dp_group_select       = "群组选择        ";//8
const u8 *dp_gps_information    = "GPS信息         ";//9
const u8 *dp_backlight_time     = "背光灯时间      ";//10
const u8 *dp_keypad_lock_time   = "键盘锁时间      ";//11
const u8 *dp_software_version   = "软件版本        ";//12
const u8 *dp_press_well         = "请按#键         ";//13
const u8 *dp_latitude0          = "纬度:00.000000  ";//14
const u8 *dp_longitude0         = "经度:000.000000 ";//15
const u8 *dp_backlight          = "背光灯时间      ";//16
const u8 *dp_keypad_lock        = "键盘锁时间      ";//17
const u8 *dp_close              = "关闭            ";//18
const u8 *dp_individual_call    = "临时群组        ";//19
const u8 *dp_version            = "软件版本        ";//20
const u8 *dp_press_ok_then_well = "先按OK键再按#键 ";//21
const u8 *dp_status_offline     = "状态：离线      ";//22

const u8 *dp_SimCardError       = "卡异常          ";//
const u8 *dp_cs_unknow_network  = "CS未知网络      ";//
const u8 *dp_gprs_unknow_network= "GPRS未知网络    ";//
const u8 *dp_gprs_refuse_enroll = "GPRS注册被拒绝  ";//
const u8 *dp_eps_unknow_network = "EPS未知网络     ";//
const u8 *dp_eps_refuse_enroll  = "EPS注册被拒绝   ";//
const u8 *dp_all_unknow_network = "所有网络异常    ";//
const u8 *dp_no_service         = "无服务          ";//
const u8 *dp_getting_info       = "获取中...       ";

const u8 *dp_abell2              = "     ABELL      ";//1
const u8 *dp_personal_mode2      = "Personal Mode   ";//2
const u8 *dp_no_sim_card2        = "No SIM Card     ";//3
const u8 *dp_network_search2     = "Network Search  ";//4
const u8 *dp_account_config2     = "Account Config..";//5
const u8 *dp_please_charge2      = " Please charge  ";//6
const u8 *dp_menu2               = "Menu            ";//7
const u8 *dp_group_select2       = "Group Select    ";//8
const u8 *dp_gps_information2    = "GPS Information ";//9
const u8 *dp_backlight_time2     = "Backlight Time  ";//10
const u8 *dp_keypad_lock_time2   = "Keypad Lock Time";//11
const u8 *dp_software_version2   = "Software Version";//12
const u8 *dp_press_well2         = "Press #         ";//13
const u8 *dp_latitude02          = "Lat :00.000000  ";//14
const u8 *dp_longitude02         = "Lng :000.000000 ";//15
const u8 *dp_backlight2          = "Backlight       ";//16
const u8 *dp_keypad_lock2        = "Keypad Lock     ";//17
const u8 *dp_close2              = "Close           ";//18
const u8 *dp_individual_call2    = "Individual Call ";//19
const u8 *dp_version2            = "Version:        ";//20
const u8 *dp_press_ok_then_well2 = "Press OK then # ";//21
const u8 *dp_status_offline2     = "Status:Offline  ";//22

const u8 *dp_SimCardError2       = "SIM card error  ";//
const u8 *dp_cs_unknow_network2  = "CS NET unknow   ";//
const u8 *dp_gprs_unknow_network2= "GPRS NET unknow ";//
const u8 *dp_gprs_refuse_enroll2 = "GPRS Signup Fail";//
const u8 *dp_eps_unknow_network2 = "EPS NET unknow  ";//
const u8 *dp_eps_refuse_enroll2  = "EPS Signup Fail ";//
const u8 *dp_all_unknow_network2 = "All NET unknow  ";//
const u8 *dp_no_service2         = "No service      ";//
const u8 *dp_getting_info2       = "getting info... ";

void DISPLAY_Show(DISPLAY_TYPE id)
{
  switch(AtCmdDrvobj.language_set)
  {
  case m_CHINESE:
    switch(id)
    {
    case d_ABELL:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_abell);
        break;
    case d_PersonalMode:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_personal_mode);
      break;
    case d_AllGroupName:
      api_lcd_pwr_on_hint(0,2,GBK,"                ");//清屏
      api_lcd_pwr_on_hint(0,2,UNICODE,GetAllGroupNameForDisplay(groupCallingcount));//显示当前选中的群组名
      break;
    case d_NoSimCard:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_no_sim_card);
      break;
    case d_NetworkSearching:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_network_search);
      break;
    case d_AllUserName:
      api_lcd_pwr_on_hint(0,2,GBK,"                ");//清屏
      api_lcd_pwr_on_hint(0,2,UNICODE,GetAllUserNameForDisplay(PersonalCallingNum));//显示当前选中的群组名
      break;
    case d_LoggingIn:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_account_config);
      break;
    case d_PowerLowPleaseCharge:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_please_charge);
      break;
    case d_menu:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_menu);
      break;
    case d_menu_groupselect:
      api_lcd_pwr_on_hint(13,0,GBK,"1/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_group_select);
      break;
    case d_menu_gpsinformation:
      api_lcd_pwr_on_hint(13,0,GBK,"2/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_gps_information);
      break;
    case d_menu_backlighttime:
      api_lcd_pwr_on_hint(13,0,GBK,"3/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_backlight_time);
      break;
    case d_menu_keypadlocktime:
      api_lcd_pwr_on_hint(13,0,GBK,"4/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_keypad_lock_time);
      break;
    case d_menu_softwareversion:
      api_lcd_pwr_on_hint(13,0,GBK,"5/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_software_version);
      break;
    case d_presswell:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_press_well);
      break;
    case d_longitude_and_latitude0:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_latitude0);//清屏
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_longitude0);//清屏
      break;
    case d_backlight:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_backlight);
      break;
    case d_keypadlock:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_keypad_lock);
      break;
    case d_close:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_close);
      break;
    case d_individualcall:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_individual_call);//Individual Call临时群组
      break;
    case d_version:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_version);
      break;
    case d_press_ok_then_well:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_press_ok_then_well);
      break;
    case d_status_offline:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_status_offline);
      break;
    case d_SimCardError:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_SimCardError);
      break;
    case d_cs_unknow_network:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_cs_unknow_network);
      break;
    case d_gprs_unknow_network:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_gprs_unknow_network);
      break;
    case d_gprs_refuse_enroll:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_gprs_refuse_enroll);
      break;
    case d_eps_unknow_network:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_eps_unknow_network);
      break;
    case d_eps_refuse_enroll:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_eps_refuse_enroll);
      break;
    case d_all_unknow_network:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_all_unknow_network);
      break;
    case d_no_service:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_no_service);
      break;
    case d_getting_info:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_getting_info);
      break;
    default:
      break;
    }
    break;
  case m_ENGLISH:
    switch(id)
    {
    case d_ABELL:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_abell2);
        break;
    case d_PersonalMode:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_personal_mode2);
      break;
    case d_AllGroupName:
      api_lcd_pwr_on_hint(0,2,GBK,"                ");//清屏
      api_lcd_pwr_on_hint(0,2,UNICODE,GetAllGroupNameForDisplay(groupCallingcount));//显示当前选中的群组名
      break;
    case d_NoSimCard:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_no_sim_card2);
      break;
    case d_NetworkSearching:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_network_search2);
      break;
    case d_AllUserName:
      api_lcd_pwr_on_hint(0,2,GBK,"                ");//清屏
      api_lcd_pwr_on_hint(0,2,UNICODE,GetAllUserNameForDisplay(PersonalCallingNum));//显示当前选中的群组名
      break;
    case d_LoggingIn:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_account_config2);
      break;
    case d_PowerLowPleaseCharge:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_please_charge2);
      break;
    case d_menu:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_menu2);
      break;
    case d_menu_groupselect:
      api_lcd_pwr_on_hint(13,0,GBK,"1/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_group_select2);
      break;
    case d_menu_gpsinformation:
      api_lcd_pwr_on_hint(13,0,GBK,"2/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_gps_information2);
      break;
    case d_menu_backlighttime:
      api_lcd_pwr_on_hint(13,0,GBK,"3/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_backlight_time2);
      break;
    case d_menu_keypadlocktime:
      api_lcd_pwr_on_hint(13,0,GBK,"4/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_keypad_lock_time2);
      break;
    case d_menu_softwareversion:
      api_lcd_pwr_on_hint(13,0,GBK,"5/5");
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_software_version2);
      break;
    case d_presswell:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_press_well2);
      break;
    case d_longitude_and_latitude0:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_latitude02);//清屏
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_longitude02);//清屏
      break;
    case d_backlight:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_backlight2);
      break;
    case d_keypadlock:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_keypad_lock2);
      break;
    case d_close:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_close2);
      break;
    case d_individualcall:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_individual_call2);//Individual Call临时群组
      break;
    case d_version:
      api_lcd_pwr_on_hint(0,0,GBK,(u8 *)dp_version2);
      break;
    case d_press_ok_then_well:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_press_ok_then_well2);
      break;
    case d_status_offline:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_status_offline2);
      break;
    case d_SimCardError:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_SimCardError2);
      break;
    case d_cs_unknow_network:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_cs_unknow_network2);
      break;
    case d_gprs_unknow_network:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_gprs_unknow_network2);
      break;
    case d_gprs_refuse_enroll:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_gprs_refuse_enroll2);
      break;
    case d_eps_unknow_network:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_eps_unknow_network2);
      break;
    case d_eps_refuse_enroll:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_eps_refuse_enroll2);
      break;
    case d_all_unknow_network:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_all_unknow_network2);
      break;
    case d_no_service:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_no_service2);
      break;
    case d_getting_info:
      api_lcd_pwr_on_hint(0,2,GBK,(u8 *)dp_getting_info2);
      break;
    default:
      break;
    }
    break;
  }
}
/*******************************************************************************
* Description	: char-data display process
* Input			: CharInfo	: char data location information and dispaly char type
				: CharData	: display char data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_disp_char_output(DISP_CHAR CharInfo, u8 *CharData)
{	
	DISP_MulTypePro(CharInfo, CharData);
	return;
}
void api_disp_char_output2(DISP_CHAR CharInfo, u8 *CharData)//UNICODE显示，群组信息显示屏显示使用
{	
	DISP_MulTypePro2(CharInfo, CharData);
	return;
}
void api_lcd_pwr_on_hint(u8 x,u8 y,ENCODETYPE encode,u8 *CharData)
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = y;
	stCharInfo.DispAddX  = x;//一行16个英文字符
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
        if(encode==GBK)
        {
          api_disp_char_output(stCharInfo,CharData);
        }
        else//encode==UNICODE
        {
          switch(AtCmdDrvobj.language_set)
          {
          case m_CHINESE:
            api_disp_char_output2(stCharInfo,CharData);//UNICODE显示，群组信息显示屏显示使用
            break;
          case m_ENGLISH:
            api_disp_char_output(stCharInfo,CharData);
            break;
          }
        }
	api_disp_all_screen_refresh();// 全屏统一刷新
}
void api_lcd_pwr_on_hint4(u8 *CharData)//UNICODE显示
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x02;//左上角显示汉字
	stCharInfo.DispAddX  = 0;//一行16个英文字符
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output2(stCharInfo,CharData);//UNICODE显示，群组信息显示屏显示使用

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// 全屏统一刷新
}
void api_lcd_pwr_on_hint5(u8 *CharData)//UNICODE显示
{
	DISP_CHAR stCharInfo;
	stCharInfo.DispType  = DISP_IDCNASC816;
	//stCharInfo.DispAddX  = 0;
	stCharInfo.DispAddY  = 0x00;//左上角显示汉字
	stCharInfo.DispAddX  = 0;//一行16个英文字符
        stCharInfo.DispLenth = LCD_DISP_LEN_MAX;
        
	api_disp_char_output2(stCharInfo,CharData);//UNICODE显示，群组信息显示屏显示使用

	//MCU_LCD_BACKLIGTH(OFF);
	api_disp_all_screen_refresh();// 全屏统一刷新
}
/*******************************************************************************
* Description	: 刷新屏幕数据显示
* Input		: void
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_disp_all_screen_refresh(void)
{
	u8 Page, XAlias;
	u16 iPoint;

	for (Page = 0; Page < PAGE_MAX; Page++)
	{
		iPoint = Page * 0x80;
		drv_htg_set_address(0, Page);
		for (XAlias = 0; XAlias < COL_MAX; XAlias++)
		{
			drv_htg_write_data(DisDataBuf[iPoint]);
			iPoint++;
		}
	}
	//bDisDataBufUsed = FALSE;
}





/*******************************************************************************
* Description	: Mixed type display process
* Input			: CharInfo : char display location and type information
				: *CharData: display char data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
static void DISP_MulTypePro(DISP_CHAR CharInfo, u8 *CharData)
{
  u8 CharCodeH;
  u8 CharCodeL;
  u16 CharCode;
  DISP_CHAR DisInfo;
  u8  iLen = 0;//34
  u8  CharBuf[34];
  DisInfo = CharInfo;
  if ((CharInfo.DispType & 0x80) != 0x00)
  {
    DisInfo.DispLenth = 0x00;
    for (; *CharData != 0x00; DisInfo.DispLenth++)
    {
      if (DisInfo.DispLenth >= CharInfo.DispLenth) { return; }//（修改当显示长度为16时的显示问题）
      CharCode = *CharData;
      DisInfo.DispAddX = CharInfo.DispAddX + iLen;
      DisInfo.DispType = (DISP_TYPE)(CharInfo.DispType & BASETYPE);
      if (*CharData >= 0x80)//为中文字符
      {
        iLen++;
        CharData++;
        CharCode <<= 0x08;
        CharCode |= (*CharData);
        DisInfo.DispType = DISP_IDCN1516;
        DisInfo.DispLenth++;//当显示中文，长度为16时的显示问题
        CharCodeH=(CharCode&0xff00)>>8;
        CharCodeL=CharCode&0x00ff;
      }
      //GB2312_16_GetData(0xa3,0x65+0x80,CharBuf);//CharData[0]
      //GB2312_16_GetData(0xa3,0x42+0x80,CharBuf);
      drv_gt20_data_output(DisInfo.DispType, CharCode, CharBuf);
      //UNICODE_16_GetData(0xff42,CharBuf);
			DISP_DataBuf(DisInfo, CharBuf);
                        
			CharData++;
			iLen++;
		}
	}

	return;
}
static void DISP_MulTypePro2(DISP_CHAR CharInfo, u8 *CharData)//UNICODE显示
{
  u8  CharBuf2[32];
  DISP_CHAR DisInfo;
  u8  iLen = 0;//34
  DisInfo = CharInfo;
  if ((CharInfo.DispType & 0x80) != 0x00)
  {
    DisInfo.DispLenth = 0x00;
    for (; (*CharData != 0x00)||(*(CharData+1) != 0x00); DisInfo.DispLenth++)//添加*(CharData+1)判断，导致0x00也会进入循环，故在循环内处理0x0000
    {
      if (DisInfo.DispLenth >= CharInfo.DispLenth) { return; }//（修改当显示长度为16时的显示问题）
      CharCode = *CharData;
      if(CharCode!=0x0000)//解决0x00显示屏占半个字符的问题
      {
        DisInfo.DispAddX = CharInfo.DispAddX + iLen;
        DisInfo.DispType = (DISP_TYPE)(CharInfo.DispType & BASETYPE);
        if (*CharData >= 0x4E)//为中文字符//UNICODE范围：4E00-9FA5
        {
          iLen++;
          CharData++;
          CharCode <<= 0x08;
          CharCode |= (*CharData);
          DisInfo.DispType = DISP_IDCN1516;
          DisInfo.DispLenth++;//当显示中文，长度为16时的显示问题
        }
        drv_gt20_data_output2(DisInfo.DispType, CharCode, CharBuf2);
        DISP_DataBuf(DisInfo, CharBuf2);
        iLen++;
      }
      CharData++;
    }
  }
  return;
}
/*******************************************************************************
* Description	: display data process
* Input			: DisInfo : display char location and type information
				: *CharData: display char data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
static void DISP_DataBuf(DISP_CHAR DisInfo, u8 *CharData)
{
	u16 iPt;
	u8  High, Width, xCol, yPage, nType, bType;
        //DisInfo.DispType = DISP_IDCN1516;//添加后数字和字母可以显示一半
	//bDisDataBufUsed = TRUE;
	yPage = DisInfo.DispAddY;
	nType = (DisInfo.DispType & NROWTYPE);//汉字：0x01&0x10=0 字母：0x05&0x10=0
	bType = (DisInfo.DispType & BASETYPE);//汉字：0x01&0x0f=1 字母：0x05&0x0f=5
	// if (DISP_OverBound(DisInfo) != TRUE) return;
	xCol  = (nType != 0x00) ? DisInfo.DispAddX : (DisInfo.DispAddX * 0x08);//一直取DisInfo.DispAddX * 0x08
	for (High = 0; High < CHAR_HIGH[0][bType]; High++)//汉字：CHAR_HIGH=2 字母：CHAR_HIGH=2
	{
		iPt = xCol + yPage * 0x80;//iPt为8*8像素
		for (Width = 0; Width < CHAR_WIDTH[0][bType]; Width++)//汉字：CHAR_WIDTH=16 字母：CHAR_WIDTH=8
		{
			if (iPt < 512)//修改当显示长度为16时的显示问题
			{
				if (DisDataBuf[iPt] != (*CharData))
				{
					if (iPt == 3*0x80)
					{
						iPt = iPt;
					}
					DisDataBuf[iPt] = *CharData;//[512]
					DisDataBit[iPt / 0x08] |= BitTab[iPt % 0x08];//[64]
				}
				CharData++;
				iPt++;
			}
		}
		yPage++;
	}
	//bDisDataBufUsed = FALSE;
}

//显示屏图标
/*******************************************************************************
* Description   : ico-id display process
* Input		: IcoInfo : ico data location information
		: IcoID : display ico-id
		: IcoDefault : ico display default location
* Output	: void
* Return	: void
* Others	: void
********************************************************************************/
void api_disp_icoid_output(u8 IcoIdIndex, bool IcoDefault,bool on_off)
{
	u16 IcoDataLen;
	ICO_INF	IcoInf;
	DISP_ICO IcoInfo;
	u8 IcoDataBuf[LCD_ICO_BUF_LEN];//LCD_ICO_BUF_LEN=32
        /* 两个字节存一个像素 */
        
        IcoDataLen = ((u16)(IcoInf.xLen) * (u16)(IcoInf.yLen)) / 0x08;
        if(IcoDataLen > LCD_ICO_BUF_LEN)
        {
          IcoDataLen = LCD_ICO_BUF_LEN;
        }
        if (on_off == TRUE)
        {
          //api_read_eeprom_data(IcoInf.iAdr, IcoDataLen, &IcoDataBuf[0]);
          memcpy(IcoDataBuf, &Ico_DataBuf[IcoIdIndex][0], LCD_ICO_BUF_LEN);
        }
        else
        {
          memset(&IcoDataBuf[0], 0x00, LCD_ICO_BUF_LEN);
        }
	if (IcoDefault == TRUE)	//使用默认图标位置配置信息
	{
          api_diap_ico_pos_get(&IcoInfo, IcoIdIndex);
	}
	api_disp_ico_output(IcoInfo, &IcoDataBuf[0]);
	//api_disp_all_screen_refresh();// 全屏统一刷新//屏蔽，添加此指令会使开机时获取群组名发生错误
	return;
}

/*******************************************************************************
* Description	: ico data display process
* Input			: IcoInfo : ico display location information
				: *IcoData: display ico data
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_disp_ico_output(DISP_ICO IcoInfo, u8 *IcoData)
{
	u16 iPt;
	u8  High, Width, yPage, xAxis;
	u8 *ptr;
        
	yPage = IcoInfo.DispAddY;
	xAxis = IcoInfo.DispAddX * 0x08;
	ptr = IcoData;
	for (High = 0; High < IcoInfo.DispHigh / 0x08; High++)
	{
		iPt = xAxis + yPage * 0x80;
		for (Width = 0; Width < IcoInfo.DispWidth; Width++)
		{
			if (DisDataBuf[iPt] != (*ptr))
			{
				DisDataBuf[iPt] = *ptr;
				//DisDrvObj.Msg.Bit.bRefresh = DISP_RUN;
				DisDataBit[iPt / 0x08] |= BitTab[iPt % 0x08];
			}
			ptr++;
			iPt++;
		}
		yPage++;
	}
}
/*******************************************************************************
* Description	: ico default location display process
* Input			: IcoInfo : ico display location information
				: IcoID : ico-id
* Output		: void
* Return		: void
* Others		: void
********************************************************************************/
void api_diap_ico_pos_get(DISP_ICO *pIcoInfo, u16 IcoID)
{
	switch (IcoID)
	{
        case eICO_IDMESSAGE://0格信号
        case eICO_IDRXFULL://1格信号
        case eICO_IDRXNULL://2格信号
        case eICO_IDSCAN://3格信号
        case eICO_IDSCANPA://4格信号
        case eICO_IDSPEAKER://5格信号
          pIcoInfo->DispAddX = 0x00;
          break;
        case eICO_IDSPEAKEROff://图标×
        case eICO_IDTALKAROff://图标T
        case eICO_IDLOCKEDOff://图标H+
        case eICO_IDSCANOff://图标H
        case eICO_IDVOXOff://图标E
        case eICO_IDOffStart://图标4G
	case eICO_IDEmergency://图标：3G
	case eICO_IDPOWERL://图标：2G
          pIcoInfo->DispAddX = 0x02;
          break;
        case eICO_IDTemper://免提模式
        case eICO_IDMONITER://听筒模式
          pIcoInfo->DispAddX = 0x04;//0x04
          break;
	case eICO_IDPOWERM://图标：组呼
	case eICO_IDPOWERH://图标：个呼
          pIcoInfo->DispAddX = 0x06;//0x04
          break;
	case eICO_IDBANDWIDTHW://锁屏图标
	case eICO_IDBANDWIDTHN://空图标
          pIcoInfo->DispAddX = 0x0c;
          break;
	case eICO_IDTALKAR://无发射无接收状态（无图标）
        case eICO_IDTX://发射信号图标
	case eICO_IDVOX://图标：接收，听状态
          pIcoInfo->DispAddX = 0x0a;
          break;
        
	
	
	
	
	
	case eICO_IDMONITEROff://空电池图标

		
	
	case eICO_IDMESSAGEOff://空图标
        case eICO_IDLOCKED://选或S
		pIcoInfo->DispAddX = 0x08;
		break;
		
	
	
		pIcoInfo->DispAddX = 0x0c;
		break;
		
	case eICO_IDBATT://空电池图标
	case eICO_IDBATT1:
	case eICO_IDBATT2:
	case eICO_IDBATT3:
	case eICO_IDBATT4:
	case eICO_IDBATT5://--------------------
		pIcoInfo->DispAddX = 0x0e;
		break;
		
	default:
		break;
	}
	
	pIcoInfo->DispAddY = 0x00;
	pIcoInfo->DispWidth= 0x10;
	pIcoInfo->DispHigh = 0x10;
}
//附：从字库中读数据函数说明 u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)实现参考代码。
/****************************************************
u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)
Address  ： 表示字符点阵在芯片中的字节地址。
byte_long： 是读点阵数据字节数。
*p_arr   ： 是保存读出的点阵数据的数组。
*****************************************************/
//u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)
unsigned char r_dat_bat(unsigned long address,unsigned long byte_long,unsigned char *p_arr)
{
	unsigned int j=0;
	MCU_GT20_CS(LO);
	SendByte(address); //发送指令及地址
	for(j=0;j<byte_long;j++)
	{
	 p_arr[j]=ReadByte();//读取数据到数组中
	}
	MCU_GT20_CS(HI);
	return p_arr[0];	
}
void SendByte(u32 cmd)
{
	u8 i;
	cmd=cmd|0x03000000;
	for(i=0;i<32;i++)
	{
		MCU_GT20_CLK(LO);
		if(cmd&0x80000000)
			MCU_GT20_SI(HI);
		else 
			MCU_GT20_SI(LO);
		MCU_GT20_CLK(HI);
		cmd=cmd<<1;
	}					
}

u8 ReadByte(void)
{
	u8 i;
	u8 dat=0;
	MCU_GT20_CLK(HI); 	
	for(i=0;i<8;i++)
	{
		MCU_GT20_CLK(LO); 
		dat=dat<<1;
		if(MCU_GT20_SO_Read)
			dat=dat|0x01;
		else 
			dat&=0xfe;
		MCU_GT20_CLK(HI); 	
	}	
	return dat;
        
        

}
