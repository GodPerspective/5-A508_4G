#include "AllHead.h"
u8 test_buf[10]={0};
static void iic_delay(void);
static void iic_sda_out(void);
static void iic_sda_in(void);
static void set_iic_scl_state(u8 OnOff);
static void set_iic_sda_state(u8 OnOff);

void iic_init(void)
{
  
  
  //初始化SDA和SCL为输出模式，默认高电平
  GPIO_Init(GPIO_GF_SCL,GPIO_PIN_GF_SCL,GPIO_MODE_OUT_PP_HIGH_FAST);//SCL
  GPIO_Init(GPIO_GF_SDA,GPIO_PIN_GF_SDA,GPIO_MODE_OUT_PP_HIGH_FAST);//SDA
  set_iic_sda_state(1);
  set_iic_scl_state(1);
  Decive_ReadData(0xB0,0x00,test_buf,10);
#if 0//output 6.95V
  Device_WriteData(0xB0,0x64,0x2C);
  DEL_Soft1ms(200);
  Device_WriteData(0xB0,0x01,0x00);
  Device_WriteData(0xB0,0x02,0x00);
  Device_WriteData(0xB0,0x03,0x00);
  Device_WriteData(0xB0,0x03,0x00);
  Device_WriteData(0xB0,0x04,0x00);
  Device_WriteData(0xB0,0x05,0x00);
  Device_WriteData(0xB0,0x06,0x00);
  Device_WriteData(0xB0,0x07,0x00);
  Device_WriteData(0xB0,0x08,0x00);
  Device_WriteData(0xB0,0x09,0x00);
  Decive_ReadData(0xB0,0x00,test_buf,10);
  Device_WriteData(0xB0,0x00,0x9B);
  Device_WriteData(0xB0,0x01,0x07);
  Device_WriteData(0xB0,0x02,0x28);//(battery safemode enable)
  Device_WriteData(0xB0,0x03,0x07);
  Device_WriteData(0xB0,0x04,0x04);
  Device_WriteData(0xB0,0x05,0x0D);
  Device_WriteData(0xB0,0x06,0x00);
  Device_WriteData(0xB0,0x07,0x52);
  Device_WriteData(0xB0,0x08,0x28);
  Device_WriteData(0xB0,0x09,0x02);//enable AGC1,0.08ms/DB
#else
  Device_WriteData(0xB0,0x64,0x2C);
  DEL_Soft1ms(200);
  Device_WriteData(0xB0,0x01,0x00);
  Device_WriteData(0xB0,0x01,0x00);
  Device_WriteData(0xB0,0x02,0x00);
  Device_WriteData(0xB0,0x03,0x00);
  Device_WriteData(0xB0,0x04,0x00);
  Device_WriteData(0xB0,0x05,0x00);
  Device_WriteData(0xB0,0x06,0x00);
  Device_WriteData(0xB0,0x07,0x00);
  Device_WriteData(0xB0,0x08,0x00);
  Device_WriteData(0xB0,0x09,0x00);
  Decive_ReadData(0xB0,0x00,test_buf,10);
  Device_WriteData(0xB0,0x00,0x9B);
  Device_WriteData(0xB0,0x01,0x06);
  Device_WriteData(0xB0,0x02,0x00);
  Device_WriteData(0xB0,0x03,0x07);
  Device_WriteData(0xB0,0x04,0x04);
  Device_WriteData(0xB0,0x05,0x02);
  Device_WriteData(0xB0,0x06,0x0B);
  Device_WriteData(0xB0,0x07,0x52);
  Device_WriteData(0xB0,0x08,0xA8);
  Device_WriteData(0xB0,0x09,0x03);
#endif
  Decive_ReadData(0xB0,0x00,test_buf,10);
}

//SCL高电平期间，SDA出现一个下降沿表示起始信号
void iic_start(void)//-ok
{
  iic_sda_out();//SDA设为输出模式
  
  set_iic_sda_state(1);
  set_iic_scl_state(1);
  iic_delay();//4us
  set_iic_sda_state(0);
  iic_delay();//4us
  set_iic_scl_state(0);
}

void iic_stop(void)//-ok
{
  iic_sda_out();//SDA设为输出模式
  
  set_iic_sda_state(0);
  set_iic_scl_state(1);
  iic_delay();
  set_iic_sda_state(1);
  iic_delay();
}

void iic_send_byte(u8 txd)//-ok
{
  u8 i;
  iic_sda_out();//SDA设为输出模式
  set_iic_scl_state(0);
  iic_delay();
  for(i=0;i<8;i++)
  {
    set_iic_sda_state((txd&0x80)>> 7);//按位给SDA赋值 先发高位再发低位
    txd <<= 1;//保证将要发送的位一直是最高位
    set_iic_scl_state(1);//时钟线拉高，此时数据线不得改变，用于对方读取数据
    iic_delay();
    set_iic_scl_state(0);
    iic_delay();
  }
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 iic_read_Byte(u8 ack)//-ok
{
  u8 i = 0;
  u8 receive = 0;

  iic_sda_in();//此时要把数据线的模式切换为输入模式
  
  for(i = 0;i < 8; i ++)
  {
    iic_delay();
    set_iic_scl_state(1);//时钟线拉高 读数据保证对方数据不改变
    receive <<= 1;//先左移接收变量，防止循环结束时改变该变量
    if(READ_IIC_SDA)//判断数据线电平
    {
      receive ++;//高电平的话接收变量自加，低电平不变化只左移，即保证了该位为0
    }
    iic_delay();
    set_iic_scl_state(0);//时钟线拉低，允许下一位数据改变
  }
  if(!ack)
  {
    iic_no_ack();//不需要应答 则给出非应答信号，不再继续
  }
  else
  {
    iic_ack();//需要应答 则给应答
  }
  return receive;
}

//产生应答函数
void iic_ack(void)//-ok
{
  iic_sda_out();//SDA设为输出模式
  
  set_iic_scl_state(0);
  set_iic_sda_state(0);//数据线一直保持为低电平，时钟线出现上升沿即为应答
  iic_delay();
  set_iic_scl_state(1);
  iic_delay();
  set_iic_scl_state(0);//应答完成后 将时钟线拉低 允许数据修改
}

//不产生应答
void iic_no_ack(void)//-ok
{
  iic_sda_out();//SDA设为输出模式

  set_iic_scl_state(0);
  set_iic_sda_state(1);//非应答即相反 与应答区别即为数据线保持高电平即可
  iic_delay();
  set_iic_scl_state(1);
  iic_delay();
  set_iic_scl_state(0);//最后要将时钟线拉低 允许数据变化
}

//等待应答(1：接收应答失败  0：接收应答成功)
#if 1
u8 iic_wait_ack(void)//-ok
{
  u8 READ_SDA=0;
  iic_sda_in();
  set_iic_scl_state(1);//时钟线拉高
  iic_delay();
  READ_SDA=READ_IIC_SDA;
  set_iic_scl_state(0);//时钟线拉低
  iic_delay();

  return READ_SDA;
}
#else
u8 iic_wait_ack(void)//-ok
{
  u16 ackTime;
  iic_sda_in();//先将数据线要设置成输入模式，有应答则会出现下降沿
  set_iic_scl_state(1);//时钟线拉高
  iic_delay();
  while(!READ_IIC_SDA)//等待数据线拉低应答
  {
    ackTime ++;
    if(ackTime > 250)//如果在该时间内仍未拉低
    {
      iic_stop();//认为非应答 停止信号
      return 1;
    }
  }
  set_iic_scl_state(0);
  return 0 ;
}
#endif

//写数据
void  Device_WriteData(u8 DeciveAddr,u8 DataAddr,u8 Data)//-ok
{
    iic_start();//起始信号
    iic_send_byte(DeciveAddr);//发送器件地址 
    iic_wait_ack();//等待应答
    iic_send_byte(DataAddr);//发送数据地址
    iic_wait_ack();//等待应答
    iic_send_byte(Data);//发送数据
    iic_wait_ack();//等待应答
    iic_stop();//结束信号
}

void Decive_ReadData(u8 DeciveAddr,u8 DataAddr,u8 *ReciveData,u8 num)
{
    u8 i;
    iic_start();//起始信号
    iic_send_byte(DeciveAddr);//发送器件地址
    iic_wait_ack();//等待应答
    iic_send_byte(DataAddr);//发送数据地址
    iic_wait_ack();//等待应答
    iic_start();//起始信号
    //发送器件地址读模式
    iic_send_byte(DeciveAddr + 1);
    //等待应答
    iic_wait_ack();
    //读数据
    for(i = 0;i < (num-1);i ++)
    {
        *ReciveData= iic_read_Byte(1);//前num-1位数据时需要给应答的因为要继续读
        ReciveData++;
    }
    *ReciveData = iic_read_Byte(0);//最后一位数据不需要给应答 因为不用读了
    iic_stop();//停止信号
}

static void iic_sda_out(void)
{
  GPIO_Init(GPIO_GF_SDA,GPIO_PIN_GF_SDA,GPIO_MODE_OUT_PP_HIGH_FAST);
}

static void iic_sda_in(void)
{
  GPIO_Init(GPIO_GF_SDA,GPIO_PIN_GF_SDA,GPIO_MODE_IN_FL_NO_IT);
}

static void set_iic_sda_state(u8 OnOff)
{
  switch(OnOff)
  {
  case 1:
    GPIO_WriteHigh(GPIO_GF_SDA, GPIO_PIN_GF_SDA);
    break;
  case 0:
    GPIO_WriteLow(GPIO_GF_SDA, GPIO_PIN_GF_SDA);
    break;
  default:
    GPIO_WriteHigh(GPIO_GF_SDA, GPIO_PIN_GF_SDA);
    break; 
  }
}
static void set_iic_scl_state(u8 OnOff)
{
    switch(OnOff)
  {
  case 1:
    GPIO_WriteHigh(GPIO_GF_SCL, GPIO_PIN_GF_SCL);
    break;
  case 0:
    GPIO_WriteLow(GPIO_GF_SCL, GPIO_PIN_GF_SCL);
    break;
  default:
    GPIO_WriteHigh(GPIO_GF_SCL, GPIO_PIN_GF_SCL);
    break; 
  }
}

static void iic_delay(void)//延时一小会 保证一个可靠的电平
{
    nop();
    nop();
    nop();
    nop();
    nop();
    //5个nop接近4us
}

