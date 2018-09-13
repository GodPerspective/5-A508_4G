#ifndef __DEVI2CAW87319_H
#define __DEVI2CAW87319_H

#include "stm8s.h"


extern void iic_init(void);
extern void iic_start(void);
extern void iic_stop(void);
extern void iic_send_byte(u8 txd);
extern u8 iic_read_Byte(u8 ack);
extern void iic_ack(void);
extern void iic_no_ack(void);
extern u8 iic_wait_ack(void);

extern void  Device_WriteData(u8 DeciveAddr,u8 DataAddr,u8 Data);
extern void Decive_ReadData(u8 DeciveAddr,u8 DataAddr,u8 *ReciveData,u8 num);

#endif