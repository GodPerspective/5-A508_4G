#include "AllHead.h"

static void iic_delay(void);
static void iic_sda_out(void);
static void iic_sda_in(void);

void iic_init(void)
{
  //��ʼ��SDA��SCLΪ���ģʽ��Ĭ�ϸߵ�ƽ
  GPIO_Init(GPIO_GF_SCL,GPIO_PIN_GF_SCL,GPIO_MODE_OUT_PP_HIGH_FAST);//SCL
  GPIO_Init(GPIO_GF_SDA,GPIO_PIN_GF_SDA,GPIO_MODE_OUT_PP_HIGH_FAST);//SDA
  set_iic_sda_state(1);
  set_iic_scl_state(1);
}

//SCL�ߵ�ƽ�ڼ䣬SDA����һ���½��ر�ʾ��ʼ�ź�
void iic_start(void)//-ok
{
  iic_sda_out();//SDA��Ϊ���ģʽ
  
  set_iic_sda_state(1);
  set_iic_scl_state(1);
  iic_delay();//4us
  set_iic_sda_state(0);
  iic_delay();//4us
  set_iic_scl_state(0);
}

void iic_stop(void)//-ok
{
  iic_sda_out();//SDA��Ϊ���ģʽ
  
  set_iic_sda_state(0);
  set_iic_scl_state(1);
  iic_delay();
  set_iic_sda_state(1);
  iic_delay();
}

void iic_send_byte(u8 txd)//-ok
{
  u8 i;
  iic_sda_out();//SDA��Ϊ���ģʽ
  set_iic_scl_state(0);
  iic_delay();
  for(i=0;i<8;i++)
  {
    set_iic_sda_state((txd&0x80)>> 7);//��λ��SDA��ֵ �ȷ���λ�ٷ���λ
    txd <<= 1;//��֤��Ҫ���͵�λһֱ�����λ
    set_iic_scl_state(1);//ʱ�������ߣ���ʱ�����߲��øı䣬���ڶԷ���ȡ����
    iic_delay();
    set_iic_scl_state(0);
    iic_delay();
  }
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 iic_read_Byte(u8 ack)//-ok
{
  u8 i = 0;
  u8 receive = 0;

  iic_sda_in();//��ʱҪ�������ߵ�ģʽ�л�Ϊ����ģʽ
  
  for(i = 0;i < 8; i ++)
  {
    iic_delay();
    set_iic_scl_state(1);//ʱ�������� �����ݱ�֤�Է����ݲ��ı�
    receive <<= 1;//�����ƽ��ձ�������ֹѭ������ʱ�ı�ñ���
    if(READ_IIC_SDA)//�ж������ߵ�ƽ
    {
      receive ++;//�ߵ�ƽ�Ļ����ձ����Լӣ��͵�ƽ���仯ֻ���ƣ�����֤�˸�λΪ0
    }
    iic_delay();
    set_iic_scl_state(0);//ʱ�������ͣ�������һλ���ݸı�
  }
  if(!ack)
  {
    iic_no_ack();//����ҪӦ�� �������Ӧ���źţ����ټ���
  }
  else
  {
    iic_ack();//��ҪӦ�� ���Ӧ��
  }
  return receive;
}

//����Ӧ����
void iic_ack(void)//-ok
{
  iic_sda_out();//SDA��Ϊ���ģʽ
  
  set_iic_scl_state(0);
  set_iic_sda_state(0);//������һֱ����Ϊ�͵�ƽ��ʱ���߳��������ؼ�ΪӦ��
  iic_delay();
  set_iic_scl_state(1);
  iic_delay();
  set_iic_scl_state(0);//Ӧ����ɺ� ��ʱ�������� ���������޸�
}

//������Ӧ��
void iic_no_ack(void)//-ok
{
  iic_sda_out();//SDA��Ϊ���ģʽ

  set_iic_scl_state(0);
  set_iic_sda_state(1);//��Ӧ���෴ ��Ӧ������Ϊ�����߱��ָߵ�ƽ����
  iic_delay();
  set_iic_scl_state(1);
  iic_delay();
  set_iic_scl_state(0);//���Ҫ��ʱ�������� �������ݱ仯
}

//�ȴ�Ӧ��(1������Ӧ��ʧ��  0������Ӧ��ɹ�)
#if 1
u8 iic_wait_ack(void)//-ok
{
  u8 READ_SDA=0;
  iic_sda_in();
  set_iic_scl_state(1);//ʱ��������
  iic_delay();
  READ_SDA=READ_IIC_SDA;
  set_iic_scl_state(0);//ʱ��������
  iic_delay();

  return READ_SDA;
}
#else
u8 iic_wait_ack(void)//-ok
{
  u16 ackTime;
  iic_sda_in();//�Ƚ�������Ҫ���ó�����ģʽ����Ӧ���������½���
  set_iic_scl_state(1);//ʱ��������
  iic_delay();
  while(!READ_IIC_SDA)//�ȴ�����������Ӧ��
  {
    ackTime ++;
    if(ackTime > 250)//����ڸ�ʱ������δ����
    {
      iic_stop();//��Ϊ��Ӧ�� ֹͣ�ź�
      return 1;
    }
  }
  set_iic_scl_state(0);
  return 0 ;
}
#endif

//д����
void  Device_WriteData(u8 DeciveAddr,u8 DataAddr,u8 Data)//-ok
{
    iic_start();//��ʼ�ź�
    iic_send_byte(DeciveAddr);//����������ַ 
    iic_wait_ack();//�ȴ�Ӧ��
    iic_send_byte(DataAddr);//�������ݵ�ַ
    iic_wait_ack();//�ȴ�Ӧ��
    iic_send_byte(Data);//��������
    iic_wait_ack();//�ȴ�Ӧ��
    iic_stop();//�����ź�
}

void Decive_ReadData(u8 DeciveAddr,u8 DataAddr,u8 *ReciveData,u8 num)
{
    u8 i;
    iic_start();//��ʼ�ź�
    iic_send_byte(DeciveAddr);//����������ַ
    iic_wait_ack();//�ȴ�Ӧ��
    iic_send_byte(DataAddr);//�������ݵ�ַ
    iic_wait_ack();//�ȴ�Ӧ��
    iic_start();//��ʼ�ź�
    //����������ַ��ģʽ
    iic_send_byte(DeciveAddr + 1);
    //�ȴ�Ӧ��
    iic_wait_ack();
    //������
    for(i = 0;i < (num-1);i ++)
    {
        *ReciveData= iic_read_Byte(1);//ǰnum-1λ����ʱ��Ҫ��Ӧ�����ΪҪ������
        ReciveData++;
    }
    *ReciveData = iic_read_Byte(0);//���һλ���ݲ���Ҫ��Ӧ�� ��Ϊ���ö���
    iic_stop();//ֹͣ�ź�
}

static void iic_sda_out(void)
{
  GPIO_Init(GPIO_GF_SDA,GPIO_PIN_GF_SDA,GPIO_MODE_OUT_PP_HIGH_FAST);
}

static void iic_sda_in(void)
{
  GPIO_Init(GPIO_GF_SDA,GPIO_PIN_GF_SDA,GPIO_MODE_IN_FL_NO_IT);
}

static void iic_delay(void)//��ʱһС�� ��֤һ���ɿ��ĵ�ƽ
{
    nop();
    nop();
    nop();
    nop();
    nop();
    //5��nop�ӽ�4us
}

