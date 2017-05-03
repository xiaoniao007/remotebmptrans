/**
  ******************************************************************************
  * @file    stm32f10x_it.c.c
  * $Author: 笑鸟007
  * $Revision: 17 $
  * $Date:: 2017-05-02 
  * @brief   中断函数.
  ******************************************************************************
  * @attention
  *
  *
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "sdio_sdcard.h"
#include "savefile.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 char ch;
uint8_t Rec_Flag=0;// The instructions begin to accept
uint8_t data_count=0; //data count in a package
uint16_t number_pack; //count the number of a pcakage
uint8_t Ent_Falg=0;
uint8_t End_Flag=0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
 void SDIO_IRQHandler(void) 
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/*USART3中断处理*/
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 	
		  ch = USART_ReceiveData(USART3);		 
	}
	
	if(Rec_Flag==1)
	{
			if(data_count<119)
			{
				rec_data[data_count]=ch;
				data_count++;
			}
			else 
			{
					rec_data[data_count]=ch;
				  data_count=0;	
					printf("receivr%d",number_pack);
			    Save_Pack(rec_data,number_pack);
				  number_pack++;				
			}		
	}	
	
	if((ch==0x64)&&(End_Flag==1))
	{
		  f_close(&fsave);
		  number_pack=0;
	    Ent_Falg=1;
		  Rec_Flag=0;
		  End_Flag=1;
	}
	else End_Flag=0;
	
	if(ch==0x74)
	{		
		  End_Flag=1;	
 
	}
		
	
	if((ch==0x73)&&(Rec_Flag==0))
	{
			Rec_Flag=1; //标识数据开始传输，数据传输之前请以s开头
			number_pack=0;
			data_count=0;
		  Ready_Save();
	}
//			
//  USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除终端标志位
}




/*********************************END OF FILE**********************************/
