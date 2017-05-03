/**
  ******************************************************************************
  * @file    savefile.h
  * $Author: Ð¦Äñ007
  * $Revision: 17 $
  * $Date:: 2017-05-025 
  * @brief   header file of save bmp data to sd.
  ******************************************************************************
  * @attention
  *
  *<
  * 
  *********/
	
#ifndef _SAVEFILE_H
#define _SAVEFILE_H

#include <stm32f10x.h>
#include "spi.h"
#include "socket.h"	// Just include one header for WIZCHIP
#include "usart.h"
#include "ff.h"
#include "sdio_sdcard.h"

extern FIL fsave;													/* file objects */
extern UINT  bw;            					/* File R/W count */
extern BYTE buffer_save[4096];       		  /* file copy buffer */
extern uint8_t rec_data[120];             //data read from sd
extern char bmp_name[20];
extern int count_bmp;
/*
*open a file and ready to save
*/
int Ready_Save(void);
/*
*Save a file to .bmp
*/
int Save_Pack(uint8_t *p,int aa);
	
	
#endif
