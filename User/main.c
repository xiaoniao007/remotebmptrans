/**
  ******************************************************************************

  ******************************************************************************
  * @attention
  *

  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "delay.h"
#include "spi.h"
#include "socket.h"	// Just include one header for WIZCHIP
#include "savefile.h"
#include "ff.h"
#include "sdio_sdcard.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SOCK_TCPS        0
#define DATA_BUF_SIZE   2048
/* Private macro -------------------------------------------------------------*/
uint8_t gDATABUF[DATA_BUF_SIZE]={1,2,3,3,3,3};
// Default Network Configuration
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0x7f, 0x4e},
                            .ip = {192, 168, 1, 166},
                            .sn = {255,255,255,0},
                            .gw = {192, 168 ,1,254},
                            .dns = {192, 168 ,1,254},
                            .dhcp = NETINFO_STATIC };
/* Private variables ---------------------------------------------------------*/
														
FIL fnew;													/* file objects */
FATFS fs;													/* Work area (file system object) for logical drives */
FRESULT res; 
UINT br;            					/* File R/W count */
uint8_t picture_data[240*3];             //data read from sd
UINT btr=240*3;

extern uint8_t Ent_Falg;

uint16_t  pic_flag=1;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void platform_init(void);								// initialize the dependent host peripheral
void network_init(void);								// Initialize Network information and display it
void fs_open_send(void);
/**
  * @brief  ´®¿Ú´òÓ¡Êä³ö
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t tmp;
	int32_t ret = 0;
	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
	uint8_t DstIP[]={120,25,251,116};
	int port=5000;
//		uint8_t DstIP[]={192,168,1,105};
	//Host dependent peripheral initialized
	
	platform_init();
	// First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP 
	/* Critical section callback */
	reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);	//×¢²áÁÙ½çÇøº¯Êý
	/* Chip selection call back */
#if   _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
	reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);//×¢²áSPIÆ¬Ñ¡ÐÅºÅº¯Êý
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
	reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);  // CS must be tried with LOW.
#else
   #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
   #else
      reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
   #endif
#endif
	/* SPI Read & Write callback function */
	reg_wizchip_spi_cbfunc(SPI_ReadByte, SPI_WriteByte);	//×¢²á¶ÁÐ´º¯Êý

	/* WIZCHIP SOCKET Buffer initialize */
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1){
		 printf("WIZCHIP Initialized fail.\r\n");
		 while(1);
	}

	/* PHY link status check */
	do{
		 if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1){
				printf("Unknown PHY Link stauts.\r\n");
		 }
	}while(tmp == PHY_LINK_OFF);

	/* Network initia lization */
	network_init();
	
  while(1)
	{		
			if(Ent_Falg==1)
			{		
			 //ÐÂ½¨Ò»¸öSocket²¢°ó¶¨±¾µØ¶Ë¿Ú5000	
          Ent_Falg=0;	
          port++;				
					ret = socket(SOCK_TCPS,Sn_MR_TCP,port+1,0x00);
					if(ret != SOCK_TCPS)
					{
						 printf("%d:Socket Error\r\n",SOCK_TCPS);
						 
					}
					else
					{
							printf("%d:Opened\r\n",SOCK_TCPS);
					}
					//Á¬½ÓTCP·þÎñÆ÷
					ret = connect(SOCK_TCPS,DstIP,84);
					if(ret != SOCK_OK)
					{
						printf("%d:Socket Connect Error\r\n",SOCK_TCPS);					
					}		
					else 
					{
						res = f_open( &fnew ,bmp_name, FA_OPEN_EXISTING | FA_READ);
						fs_open_send();					
						close(SOCK_TCPS);						
						Ent_Falg=0;
					}
					
			}
	}
}

/**
  * @brief  Intialize the network information to be used in WIZCHIP
  * @retval None
  */
void network_init(void)
{
   uint8_t tmpstr[6];
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
}

/**
  * @brief  Loopback Test Example Code using ioLibrary_BSD	
  * @retval None
  */
void platform_init(void)
{
	SystemInit();//ÏµÍ³Ê±ÖÓ³õÊ¼»¯
	USART_Configuration();//´®¿Ú1³õÊ¼»¯

//	printf("\r\n");
	//Config SPI
	SPI_Configuration();
	//ÑÓÊ±³õÊ¼»¯
	delay_init();
  SD_Init();
	NVIC_Configuration();       //SD NVIC
	
	if((res=f_mount(0,&fs))==FR_OK)
	{
		  printf("SDcard initalize successful!\n");
	}
}

void fs_open_send()
{
  
	uint16_t i,width;     //Í¼Æ¬µÄ¿í¶È£¬ÓÃÓÚ·¢ËÍÊý¾
	i=0;
	width=320;

	if(res==FR_OK)
	{

			for(i=0;i<=width;i++)
			{
				if(i==0)
				{
					f_read(&fnew,picture_data,54,&br); 					
					 send(SOCK_TCPS,picture_data,720);
					
				}	
       else{	

						if((res=f_read(&fnew,picture_data,btr,&br))==FR_OK)
								{
									send(SOCK_TCPS,picture_data,br);									
								}				 

				  	else printf("Read error!\r\n");
				  
			      }
		}
		
		
	}
	else printf("Open error!\r\n");
	
	
}

/*********************************END OF FILE**********************************/
