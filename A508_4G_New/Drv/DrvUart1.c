#include "ALLHead.h"

void DrvUart1_init(void)
{
    UART1_DeInit();		/* ���Ĵ�����ֵ��λ */
	
	/*
	 * ��UART1����Ϊ��
	 * ������ = 115200
	 * ����λ = 8
	 * 1λֹͣλ
	 * ��У��λ
	 * ʹ�ܽ��պͷ���
	 * ʹ�ܽ����ж�
	 */
    UART1_Init((u32)345600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, \
    	UART1_PARITY_NO , UART1_SYNCMODE_CLOCK_DISABLE , UART1_MODE_TXRX_ENABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    UART1_Cmd(ENABLE);
    
#if 0 //WCDMA ׿�Ǵ�
    GPIO_Init(GPIOA,GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT );//����
#endif
}

/*******************************************************************************
 * ����: UART1_ReceiveByte
 * ����: UART1����һ���ַ�
 * �β�: ��
 * ����: ���յ����ַ�
 * ˵��: �� 
 ******************************************************************************/
u8 DrvUart1_receiveByte(void)
{
	u8 USART1_RX_BUF; 
	
	/* �ȴ�������� */
	while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
	  	;
	
	USART1_RX_BUF = UART1_ReceiveData8();
	
	return  USART1_RX_BUF;
}