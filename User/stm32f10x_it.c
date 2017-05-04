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
uint8_t data_count_save=0; //data count in a package
uint16_t number_pack; //count the number of a pcakage
uint8_t Ent_Falg=0;
uint8_t End_Flag=0;
uint8_t End_Flag1=0;
uint8_t num_pack[2];

uint8_t save_flag=0;

uint8_t st_pack=0,st_pack1=0;  //接收一包新的数据

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
			
					//判断是哪一包数据
					if((data_count<2) && (st_pack1==1))
					{
								 num_pack[data_count]=ch;
								 data_count++;
								if(data_count>=2)
								{
							
											save_flag=1;
											data_count_save=0;
								}
										
								
					}
					
			
			 else save_flag=1;
			
			
			if(data_count_save<120&&(save_flag==1))
				{
								rec_data[data_count_save]=ch;
								data_count_save++;
							
							
							if(data_count_save>=120)
							{
									data_count=0;	
									number_pack=num_pack[0]+(num_pack[1]*256);
								
									if(number_pack>0x77f)
									{
										number_pack=0x77f;
									}
									printf("%d  ",number_pack);
									Save_Pack(rec_data,number_pack);
													
							}	
				
	  	}	
		}

		if(ch==0x13)
		{
			  st_pack1=1;  //标志开始新的一个数据包
			  st_pack=0;
			  data_count=0;
		}
		else st_pack=0;
		
		if(ch==0x7E)
		{
			  st_pack=1;
		}
		
		if((ch==0x6E)&&(End_Flag==1))
		{
				f_close(&fsave); //close file
			
				data_count=0;   //header count clear;
				number_pack=0;          //number 欧
				save_flag=0;       //save falg clear;
				Ent_Falg=1;
				Rec_Flag=0;
				End_Flag=0;
				End_Flag1=1;
		}
		else End_Flag=0;
		
		if((ch==0x64)&&(End_Flag==1))
		{
				End_Flag1=1;
		}
		else End_Flag=0;
		
		if(ch==0x65)
		{		
				End_Flag=1;	
	 
		}
			
		
		if((ch==0x73)&&(Rec_Flag==0))
		{
				Rec_Flag=1; //标识数据开始传输，数据传输之前请以s开头
				number_pack=0;
				data_count=0;
				Ready_Save();
				data_count=0;   //header count clear;
				st_pack1=0;
				st_pack=0;
				number_pack=0;          //number 欧
				save_flag=0;       //save falg clear;
			
		}
//			
//  USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除终端标志位
}




/*********************************END OF FILE**********************************/
