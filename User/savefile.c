/**
  ******************************************************************************
  * @file    savefile.c
  * $Author: 笑鸟007
  * $Revision: 17 $
  * $Date:: 2017-05-025 
  * @brief   save bmp data to sd.
  ******************************************************************************
  * @attention
  *
  *<
  * 
  *********/
#include "savefile.h"
	
/*
*open a file and ready to save
*/
FIL fsave;													/* file objects */
UINT  bw;            					/* File R/W count */

uint8_t rec_data[122];             //data read from sd
char bmp_name[20];
int count_bmp=0;//计算bmp图片接收量
 uint8_t header1[54] =
	{
		0x42, 0x4d, 0x36, 0x84, 0x03, 0, 0, 0, 0, 0, 0x36, 0, 0, 0, 0x28,0, 0, 0, 0x40, 0x01, 0, 0, 0xF0, 0, 
		0, 0, 0x01, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int Ready_Save()
{
	
	int res;	
	sprintf(bmp_name,"%d.bmp",count_bmp);
	res=f_open( &fsave ,bmp_name, FA_WRITE | FA_CREATE_ALWAYS);
	if(-1!=res)
	{
				f_write(&fsave, header1,54,&bw);
				if(bw==54)
				{
						printf("Create a new file successfull %s",bmp_name);
						count_bmp++;
				}
				else 
				{			
					printf("write in a new file failed %d",bw);
					
				}
	}
  else printf("Create a new file failed");
	return bw;
}

/*
*Save a file to .bmp
*/
int Save_Pack(uint8_t *p,uint16_t aa)
{
		f_lseek(&fsave,(120*aa)+54);
		f_write (&fsave, p,120,&bw);	
		return bw;
}
