/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _LED_H_
#define _LED_H_

#include "Data.h"


#define LED_1_Portx     	 PTC
#define LED_1_Pin          GPIO_Pin_0

#define LED_2_Portx     	 PTA
#define LED_2_Pin          GPIO_Pin_17

#define LED_3_Portx        PTD
#define LED_3_Pin          GPIO_Pin_15

#define LED_4_Portx     	 PTE
#define LED_4_Pin          GPIO_Pin_26

#define LED_Test_Portx     PTA
#define LED_Test_Pin       GPIO_Pin_11
 
#define Start_in		       PBin(3)
#define Srart2_in					 PEin(4)

#define Black1_in					 PBin(11)
#define Black2_in          PBin(10)

#define Beep()						 PBout(21) = 1
#define NoBeep()					 PBout(21) = 0

//#define Infrared_Black_Map       ADC1_SE7_PE3
#define Infrared_Podao_Map       ADC0_SE15_PC1


void LED_Init(void);   
void LED_Test_Init(void);

#endif
