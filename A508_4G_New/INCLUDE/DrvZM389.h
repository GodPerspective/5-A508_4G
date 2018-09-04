#ifndef __DRVZM389_H
#define __DRVZM389_H

extern void DrvZM389_uart1_interrupt(void);


extern void DrvMC8332_Software_Initial(void);
extern void DrvMC8332_TxPort_SetValidable(IO_ONOFF onoff);
extern void DrvGD83_UART_TxCommand(u8 * pBuf, u16 len);
extern bool DrvMc8332_UART_TxTail(void);
extern void DrvMC8332_UART_TxInterrupt(void);
extern u8 DrvMC8332_CaretNotify_Queue_front(u8 **pBuf);
extern u8 DrvMC8332_AtNotify_Queue_front(u8 **pBuf);
extern u8 DrvMC8332_PocNotify_Queue_front(u8 **pBuf);
extern u8 DrvMC8332_GpsNotify_Queue_front(u8 **pBuf);
#endif