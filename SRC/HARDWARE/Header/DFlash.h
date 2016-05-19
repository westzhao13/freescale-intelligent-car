/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN512 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _DFLASH_H_
#define _DFLASH_H_

#include "Data.h"

#define  BLOCK_SIZE     (2*1024) //块大小  2048Bytes 2KB 

//512KB 
#ifdef MDK60DN512
	#define  BLOCK_NUM1      250
	#define  BLOCK_NUM2      251
	#define  BLOCK_NUM3      252
#endif

#ifdef MDK60DN256
	#define  BLOCK_NUM1      125
	#define  BLOCK_NUM2      126
	#define  BLOCK_NUM3      127
#endif

#define DataLength 31

void DFlash_Init(void);
uint8_t DFlash_Write_BLOCK1(uint16_t const *buffer );
void DFLASH_ReadByte_BLOCK1(uint16_t len,uint16_t *pbuffer);
uint8_t DFLASH_EraseSector_BLOCK1(void);
void Data_Read(void);
void Data_Write(void);
#endif
