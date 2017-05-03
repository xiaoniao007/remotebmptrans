/**
  ******************************************************************************
  * @file    usart.c
  * $Author: –¶ƒÒ007
  * $Revision: 17 $
  * $Date:: 2017-05-025 
  * @brief   usart3 configeration  file 
  ******************************************************************************
  * @attention
  *
  *
  * 
  *********/
/* Includes ------------------------------------------------------------------*/

#include "usart.h"

/** @addtogroup USART
  * @brief ¥Æø⁄ƒ£øÈ
  * @{
  */

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/**
  * @brief  ø™∆ÙGPIOA,¥Æø⁄1 ±÷” 
  * @param  None
  * @retval None
  * @note  ∂‘”⁄ƒ≥–©GPIO…œµƒƒ¨»œ∏¥”√π¶ƒ‹ø…“‘≤ªø™∆Ù∑˛”√ ±÷”£¨»Áπ˚”√µΩ∏¥”√π¶ƒ‹µƒ÷ÿ
           ”≥…‰£¨‘Ú–Ë“™ø™∆Ù∏¥”√ ±÷”
  */
void USART_RCC_Configuration(void)
{
	/* config USART3 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE );
}

/**
  * @brief  …Ë÷√¥Æø⁄3∑¢ÀÕ”ÎΩ” ’“˝Ω≈µƒƒ£ Ω 
  * @param  None
  * @retval None
  */
void USART_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  ≈‰÷√¥Æø⁄3£¨≤¢ πƒ‹¥Æø
  * @param  None
  * @retval None
  */
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_RCC_Configuration();
	USART_GPIO_Configuration();
  
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART3, ENABLE);
  USART_ClearFlag(USART3, USART_FLAG_TC);
	NVIC_USART_Configuration();
}

void NVIC_USART_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	
		/* Enable the USARTy Interrupt */  // ≈‰÷√USART3Ω” ’÷–∂œ
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//≤ª π”√∞Î÷˜ª˙ƒ£ Ω
#if 0 //»Áπ˚√ª”–’‚∂Œ£¨‘Ú–Ë“™‘⁄target—°œÓ÷–—°‘Ò π”√USE microLIB
#pragma import(__use_no_semihosting)
struct __FILE  
{  
	int handle;  
};  
FILE __stdout;  

_sys_exit(int x)  
{  
	x = x;  
}
#endif


PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART3,(uint8_t)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	return ch;
}
/**
  * @}
  */

/*********************************END OF FILE**********************************/

