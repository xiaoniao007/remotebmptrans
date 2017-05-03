/**
  ******************************************************************************
  * @file    spi.c
  * $Author: 
  * $Revision: 17 $
  * $Date:: 2014-10-25 11:16:48 +0800 #$
  * @brief   SPI��������ʵ��.
  ******************************************************************************
  * @attention
  *
 
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ʹ��SPI2ʱ��
  * @retval None
  */
static void SPI_RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
}
/**
  * @brief  ����ָ��SPI������
  * @retval None
  */
static void SPI_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
			 					 
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13| GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//��ʼ��Ƭѡ�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
/**
  * @brief  �����ⲿSPI�豸����SPI��ز���
  * @retval None
  */
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStruct;

	SPI_RCC_Configuration();

	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI_InitStruct);
	
	SPI_GPIO_Configuration();

	SPI_SSOutputCmd(SPI2, ENABLE);
	SPI_Cmd(SPI2, ENABLE);
}
/**
  * @brief  д1�ֽ����ݵ�SPI����
  * @param  TxData д�����ߵ�����
  * @retval None
  */
void SPI_WriteByte(uint8_t TxData)
{				 
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==0);	//�ȴ���������		  
	SPI2->DR=TxData;	 	  									//����һ��byte 
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0); //�ȴ�������һ��byte  
	SPI2->DR;		
}
/**
  * @brief  ��SPI���߶�ȡ1�ֽ�����
  * @retval ����������
  */
uint8_t SPI_ReadByte(void)
{			 
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==0);	//�ȴ���������			  
	SPI2->DR=0xFF;	 	  										//����һ�������ݲ����������ݵ�ʱ�� 
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0); //�ȴ�������һ��byte  
	return SPI2->DR;  						    
}
/**
  * @brief  �����ٽ���
  * @retval None
  */
void SPI_CrisEnter(void)
{
	__set_PRIMASK(1);
}
/**
  * @brief  �˳��ٽ���
  * @retval None
  */
void SPI_CrisExit(void)
{
	__set_PRIMASK(0);
}

/**
  * @brief  Ƭѡ�ź�����͵�ƽ
  * @retval None
  */
void SPI_CS_Select(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}
/**
  * @brief  Ƭѡ�ź�����ߵ�ƽ
  * @retval None
  */
void SPI_CS_Deselect(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
/*********************************END OF FILE**********************************/
