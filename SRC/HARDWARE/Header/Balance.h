/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _BALANCE_H_
#define _BALANCE_H_

#include "L3G4200D.h"
#include "MMA7361.h"
#include "Data.h"

void Get_Angel(void);
int kalman(int data);
int mult_r(int get_data,int mult_get);
void Kalman_Filter(float angle_m,float gyro_m);	//gyro_m:gyro_measure
void AngleCalculate(void);
void Get_Value(void);
int Kalman_Gyro(int data);



#endif
