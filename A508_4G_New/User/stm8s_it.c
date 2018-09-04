/**
  ******************************************************************************
  * @file    stm8s_it.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service 
  *          routine.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

#include "stm8s_it.h"
#include "AllHead.h"
u8 u=0;
u8 tone_count=0;

#ifdef _COSMIC_
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{}
#endif /*_COSMIC_*/

INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{}

INTERRUPT_HANDLER(TLI_IRQHandler, 0)
{}

INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{}

INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{}

INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{}

INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{}

INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{}

INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{}

INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{}

#if defined (STM8S903) || defined (STM8AF622x) 

 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
 {}
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)

 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
 {}

 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
 {}
#endif /* (STM8S208) || (STM8AF52Ax) */

INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
}

INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
#if 0
  if(poc_receive_sos_statas()==TRUE)
  {
    tone_count++;
    if(tone_count>=7)
    {
      tone_count=0;
      Test_PWM_LED(); //WCDMA
    }
  }
  else
  {
    tone_count=0;
  }
#endif
    TIM1_ClearITPendingBit (TIM1_IT_UPDATE);	//必须要清除中断标志位

}

INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{}

#if defined (STM8S903) || defined (STM8AF622x)

 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
 {}
 
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
 {}

#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */

 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {}

 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {}
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
 {
   DEL_Interrupt();
   TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
 }

 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
 {}
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)

 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 {
   //DrvMC8332_UART_TxInterrupt();
 }

 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 {
   if(UART1_GetITStatus(UART1_IT_RXNE )!= RESET)  
   {
     DrvZM389_uart1_interrupt();
   }
 }

#endif /* (STM8S208) || (STM8S207) || (STM8S103) || (STM8S903) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8AF622x)
 INTERRUPT_HANDLER(UART4_TX_IRQHandler, 17)
 {}

 INTERRUPT_HANDLER(UART4_RX_IRQHandler, 18)
 {}
#endif /* (STM8AF622x) */

INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
 {}

 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 {}
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
 {}

 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
 {
   //UART3_Interrupt();
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
 {}
#else /* STM8S105 or STM8S103 or STM8S903 or STM8AF626x or STM8AF622x */

 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
 {}
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8S903) || defined (STM8AF622x)
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
 {}
#else /* STM8S208 or STM8S207 or STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
 {}
#endif /* (STM8S903) || (STM8AF622x)*/

INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/