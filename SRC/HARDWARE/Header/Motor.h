/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "Data.h"

//#define Left_Forward_MAP  	FTM0_CH4_PD4
//#define Left_Backward_MAP 	FTM0_CH5_PD5
//#define Right_Forward_MAP		FTM0_CH6_PD6
//#define Right_Backward_MAP 	FTM0_CH7_PD7

#define Right_Forward_MAP  	FTM0_CH5_PD5
#define Right_Backward_MAP 	FTM0_CH4_PD4
#define Left_Forward_MAP		FTM0_CH6_PD6
#define Left_Backward_MAP 	FTM0_CH7_PD7

#define Forward   1
#define Backward  0

void SpeedOut(int PTCx,int PWMx);

void Motor_Init(void);

#endif
