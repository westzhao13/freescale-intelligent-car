/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _MMA7361_H_
#define _MMA7361_H_

#include "Data.h"


#define MMA7361_X_Channel   ADC1_SE4_PE0
#define MMA7361_Y_Channel   ADC1_SE5_PE1
#define MMA7361_Z_Channel   ADC1_SE6_PE2




void MMA7361_Init(void);

uint16_t Read_ACC_X(void);
uint16_t Read_ACC_Y(void);
uint16_t Read_ACC_Z(void);

#endif
