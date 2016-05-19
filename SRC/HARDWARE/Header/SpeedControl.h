#ifndef _SPEEDCONTROL_H_
#define _SPEEDCONTROL_H_

#include "Data.h"

#define EndlongMax 		9000
#define EndlongMin 	 -9000
#define PWMMax				9000
#define PWMMin			 -9000
#define SpeedMax      6500
#define SpeedMin     -6500
#define MotorDeadVal 	250
#define LeftMotorDeadVal  320  //250
#define RightMotorDeadVal 250  //250
#define DirectionMax  1500
#define DirectionMin -1500

void Balace_Control(void);
void PWM_Out(void);
void Speed_ControlOut(int16_t MotorEndlong,int16_t MotorSpeed,int16_t MotorTurn);
int16_t AAangPWMOut(int16_t NewAangPWM ,int16_t LastAangPWM,uint8_t PeriodCount);
int16_t SpeedPWMOut(int16_t NewspeedPWM ,int16_t LastspeedPWM,uint8_t PeriodCount);
int16_t TurnPWMOut(int16_t NewturnPWM ,int16_t LastturnPWM,uint8_t PeriodCount);
void GetMotorSpeed(void);
void Speed_Stratage(void);
void SpeedCalculate(void);
void TurnCalculate(void);
#endif

