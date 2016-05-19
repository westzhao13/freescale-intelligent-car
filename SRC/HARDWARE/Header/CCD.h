/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _CCD_H_
#define _CCD_H_

#include "Data.h"

// 驱动文件宏定义
/************************CCD****************************/



//CCD SWITCH
#define CCD1_SWITCH       ON
#define CCD3_SWITCH       ON

//CCD Pin Map
#define CCD_PORTx        	PTC
#define CCD1_PIN_SI  			GPIO_Pin_6
#define CCD1_PIN_CLK  	  GPIO_Pin_10



#define CCD3_PIN_SI       GPIO_Pin_15
#define CCD3_PIN_CLK      GPIO_Pin_17

#define CCD1_ADC_Channel 	ADC0_SE14_PC0
#define CCD3_ADC_Channel  ADC0_SE12_PB2

#define SI_SetVal()   		PCout(6) 	= 1
#define SI_ClrVal()   		PCout(6) 	= 0
#define CLK_ClrVal()  		PCout(10) = 0
#define CLK_SetVal()  		PCout(10) = 1


#define SI_SetVal3()   		PCout(15) = 1
#define SI_ClrVal3()   		PCout(15) = 0
#define CLK_ClrVal3()  		PCout(17) = 0
#define CLK_SetVal3()  		PCout(17) = 1

void CCD1_Init(void);
void CCDStartIntegration1(void);
void ImageCapture(uint8_t * ImageData,uint8_t N);
void RD_TSL1(uint8_t *ADV,uint32_t ADC_MAP);
void RD_TSL2(uint8_t *ADV,uint32_t ADC_MAP);
void Read_CCD1(void);
void Read_CCD2(void);
void Read_CCD3(void);

#endif
