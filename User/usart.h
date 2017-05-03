/**
  ******************************************************************************
  * @file    usart.c
  * $Author: Ð¦Äñ007
  * $Revision: 17 $
  * $Date:: 2017-05-025 
  * @brief   usart3 configeration header file 
  ******************************************************************************
  * @attention
  *
  *<
  * 
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USART_H
#define _USART_H
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stm32f10x.h>

/* Exported Functions --------------------------------------------------------*/
void USART_RCC_Configuration(void);
void USART_GPIO_Configuration(void);
void USART_Configuration(void);
void NVIC_USART_Configuration(void);

#endif /*_usart_H*/

/*********************************END OF FILE**********************************/
