/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/

#ifndef _OLED_H_
#define _OLED_H_


#include "gpio.h"
#include <string.h>


#define OLED_PORTx       PTB
#define OLED_SCL_PIN     GPIO_Pin_17
#define OLED_SDA_PIN     GPIO_Pin_18
#define OLED_RST_PIN     GPIO_Pin_19
#define OLED_DC_PIN      GPIO_Pin_20

#define OLED_SCL   			 PBout(17)   //24   //D0
#define OLED_SDA         PBout(18)   //26   //D1
#define OLED_RST         PBout(19)   //25   //RST
#define OLED_DC          PBout(20)   //28   //DC

/**************** End of Device Config *****************/

#define  X_WIDTH         128
#define  Y_WIDTH         64
#define  XLevelL		     0x00
#define  XLevelH		     0x10
#define  XLevel		       ((XLevelH&0x0F)*16+XLevelL)
#define  Max_Column	     128
#define  Max_Row		     64
#define	 Brightness	     0xCF 


#define LED_IMAGE_WHITE       1
#define LED_IMAGE_BLACK       0

#define LED_MAX_ROW_NUM      1
#define LED_MAX_COLUMN_NUM  128


#define MAIN_OLED_ROW     1 
#define MAIN_OLED_COLUMN 128



/* Function Prototypes */

void OLED_WrDat(unsigned char data);
void OLED_WrCmd(unsigned char cmd);
void OLED_Set_XY(unsigned char x,unsigned char y);
void OLED_Clear(void);
void OLED_Delay_ms(unsigned int ms);
void OLED_Init(void);        
void OLED_Write_Char(unsigned char x,unsigned char y,unsigned char c);
void OLED_Write_String(unsigned char x,unsigned char y,unsigned char *s);
void OLED_Write_Num3(unsigned char x,unsigned char y,unsigned int num);
void OLED_Write_Num4(unsigned char x,unsigned char y,unsigned int num);
void LCD_PutPixel(uint8_t x,uint8_t y);
void OLED_Write_Num5(unsigned char x,unsigned char y,uint32_t num);
void LED_PrintImage(uint8_t *pucTable, uint16_t usRowNum, uint16_t usColumnNum);
void MAIN_ConvertData(uint8_t *pInputImageData, uint8_t *pOutputImageBuf);

#endif
