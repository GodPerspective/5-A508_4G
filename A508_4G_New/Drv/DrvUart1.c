#include "ALLHead.h"

void DrvUart1_init(void)
{
    UART1_DeInit();		/* 将寄存器的值复位 */
	
	/*
	 * 将UART1配置为：
	 * 波特率 = 115200
	 * 数据位 = 8
	 * 1位停止位
	 * 无校验位
	 * 使能接收和发送
	 * 使能接收中断
	 */
    UART1_Init((u32)345600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, \
    	UART1_PARITY_NO , UART1_SYNCMODE_CLOCK_DISABLE , UART1_MODE_TXRX_ENABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    UART1_Cmd(ENABLE);
    
#if 0 //WCDMA 卓智达
    GPIO_Init(GPIOA,GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT );//接收
#endif
}

/*******************************************************************************
 * 名称: UART1_ReceiveByte
 * 功能: UART1接收一个字符
 * 形参: 无
 * 返回: 接收到的字符
 * 说明: 无 
 ******************************************************************************/
u8 DrvUart1_receiveByte(void)
{
	u8 USART1_RX_BUF; 
	
	/* 等待接收完成 */
	while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
	  	;
	
	USART1_RX_BUF = UART1_ReceiveData8();
	
	return  USART1_RX_BUF;
}