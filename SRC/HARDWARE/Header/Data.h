/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _DATA_H_
#define _DATA_H_

//头文件
#include "includes.h"
#include "Balance.h"
#include "CCD.h"
#include "ImageProcess.h"
#include "Key.h"
#include "L3G4200D.h"
#include "Led.h"
#include "Menu.h"
#include "Oled.h"
#include "UartToPC.h"
#include "Motor.h"
#include "Encoder.h"
#include "SpeedControl.h"
#include "DFlash.h"

//引用的外部全局变量在此处引用

extern uint8_t g_aucImageTable1[MAIN_OLED_ROW][MAIN_OLED_COLUMN];
extern uint8_t g_aucImageTable2[MAIN_OLED_ROW][MAIN_OLED_COLUMN];

extern int8_t mid_ave;
extern uint8_t ImageDiff[127];
extern uint8_t ImageSort[128];
extern uint16_t Diff_Average;
extern uint16_t AD_Average;
//extern uint8_t New_Threshold;
extern uint8_t  zhijiao_count;
extern uint16_t StopFlag;  //检测到停车信号后延时一段时间
extern uint16_t  StopTime;
extern uint8_t L_Obstacle,R_Obstacle;
extern uint8_t Auto;
extern uint8_t CCD_Mode;
extern uint8_t CCD3_width;
extern uint8_t zhijiao_num;
/**** ISR ****/
extern 	uint8_t  Time;
extern 	uint8_t  DisPlayTime;
extern 	uint8_t	 DisPlay;
extern	uint8_t  UartTime;
extern 	uint8_t	 UartOK;
extern  uint8_t  Start;
extern  uint32_t S_bar;
extern  uint8_t  mid_Lc;
extern  uint8_t  obstacle_down_sec;
extern  uint8_t  Down_sec_Q;
extern  uint8_t  obstacle_conut;
extern  uint8_t  Obstacle_Switch;
extern  uint8_t  Single_Mid_Switch;
extern  uint8_t  Zhijiao_Switch;
extern  uint8_t  Podao_Switch;
extern  uint8_t  StartSwitch;
/**** Speed *****/
//getspeed
extern  int16_t  LeftSpeedNow;
extern  int16_t  RightSpeedNow;
extern 	int32_t  LeftSpeedSigma; 					//左轮采集的速度
extern	int32_t  RightSpeedSigma;					//右轮采集的速度
//pid
extern 	uint16_t Speed_P;
extern 	uint16_t Speed_I;
extern 	uint16_t Speed_D;
//pwm
extern  int16_t  PWMLeft;           //左轮占空比
extern  int16_t  PWMRight;          //右轮占空比
extern  int16_t  PWMLEFTERROR;
extern  int16_t  PWMRIGHTERROR;
extern  uint16_t LeftMotorDead;
extern  uint16_t RightMotorDead;



//speed
extern  int16_t  Speed;
extern  int16_t  LastSpeed;
extern  float    CarSpeed;
extern  uint16_t AmSpeed;
extern  float    SpeedP;
extern  float    SpeedI;
extern  float    SpeedD;
extern  int16_t  AnglePWM;
extern  int16_t  SpeedPWM;
extern  int16_t  TurnPWM;



/**** GYRO ******/
extern 	int16_t  GYRO_X;
extern	int16_t  GYRO_Y;
extern	int16_t  GYRO_Z;
extern	float    GYRO_X_Fix;
extern  float    GYRO_Y_Fix;
extern  float    Angle_Speed;

/***** ACC *****/
extern	uint16_t ACC_X;
extern	uint16_t ACC_Y;
extern	uint16_t ACC_Z;
extern	float    ACC_Y_Fix;
extern	float    ACC_Z_Fix;

/***** Angle ****/
extern	float 	 Angle;
extern	float    Angle_IGYRO;
extern	float    Angle_ACC;
extern	float    Angle_Error;
extern  uint16_t Angle_Set;
extern  uint16_t GRAVITY_ADJUST_TIME_CONSTANT;
extern  uint16_t GYROSCOPE_ANGLE_SIGMA_FREQUENCY;
extern  uint16_t CAR_ACCE_RATIO;
extern  uint16_t CAR_GYRO_RATIO;
extern  float    Angle_Stop;
extern  float    Angle_Start;
extern  uint16_t podao_angle_time ;
extern  int8_t obstacle_axis;
/**** Endlong ****/
extern 	uint16_t EndlongP;
extern 	uint16_t EndlongI;
extern 	uint16_t EndlongD;
extern  int16_t  endlong;
extern  int16_t  Last_endlong;

/***** Direction *****/
extern 	uint16_t DirectionP;
extern 	uint16_t DirectionI;
extern 	uint16_t DirectionD;

extern 	uint16_t DirectionPL;
extern 	uint16_t DirectionIL;
extern 	uint16_t DirectionDL;
extern 	uint16_t DirectionPR;
extern 	uint16_t DirectionIR;
extern 	uint16_t DirectionDR;

extern  uint16_t GYRO_Turn;

extern uint16_t ccd_average;

extern uint8_t  shizi;

extern  int8_t   offset; //一阶偏差
extern  int8_t   offset_two_order; //偏差


extern  uint16_t zhijiao_s_bar_cnt;

extern  int16_t  Direction;
extern  int16_t  lastDirection;
extern  int16_t  DirectionOut;

extern  int16_t  DirectionL;
extern  int16_t  lastDirectionL;
extern  int16_t  DirectionR;
extern  int16_t  lastDirectionR;
extern  uint16_t blacktime;
extern  uint16_t infrared_value;
extern  uint16_t podao_value;
extern  uint8_t  mid_black;
extern  uint16_t mid_black_time;
extern  uint16_t black_threshold;
extern  uint16_t podao_threshold;
extern  uint8_t  chupo;

/***** CCD ******/
extern 	uint8_t  Threshold;
extern  uint8_t  Threshold2;
extern  uint8_t  LeftBlack;
extern  uint8_t  RightBlack;
extern 	uint8_t  IntegrationTime;
extern 	uint8_t  Pixels1[128];
extern 	uint8_t  Pixels2[128];
extern 	uint8_t  Pixels3[128];
extern 	uint8_t  new_value[128];
extern  uint8_t  average_threshold;
extern  uint8_t  black_area_flag;
extern  uint16_t average;
extern  uint8_t  black_area;
extern  uint8_t  black;
extern  uint8_t  Up_Threshold;
extern  uint16_t infrared_value;
extern  uint16_t black_angle_time;
extern  uint16_t black_angle_time_out;
extern  uint16_t start_angle_time;
extern  uint8_t  first_black;
extern  uint8_t  second_black;
extern  uint16_t podao;
extern  uint8_t  begin;
extern  uint8_t  mid;
extern  uint8_t  Lc,Rc; //左雍谝线宽度
extern  uint8_t  Lc_last,Rc_last; //上一次左右黑线宽度
extern  uint8_t  LxQ2;
extern  uint8_t  RxQ2;
extern  int8_t   offset2,lastoffset2,offset_two_order2;
extern  uint8_t  single;//单线模式
extern  uint8_t  zhijiao;
/***** Stratage ****/
extern  uint8_t  Stop;
extern  uint8_t  SpeedSwitch;
extern  uint8_t  TurnSwitch;
extern  uint8_t  CCDflag;
extern  uint8_t  track_width;
extern  uint16_t S_bar_cnt;
extern  uint8_t  Mid_Threshold;
extern  uint8_t  obstacle_width;
extern  uint8_t  LeftBlack2;
extern  uint8_t  RightBlack2;
/***motor***/
extern  uint32_t MotorPool;
extern  uint8_t  Left_zhijiao;
extern  uint8_t  Right_zhijiao;
extern  uint8_t  obstacle;

extern  uint16_t A;
extern  uint16_t B;
extern  uint16_t C;

extern  uint8_t zhijiao_Angle;
extern  uint8_t zhijiao_cnt;
extern  uint16_t Test;
extern  uint16_t podao_angle;
extern  uint16_t podao_last_time;
extern  uint8_t  Stop_Flag;
extern  uint8_t  White_value;

//函数声明
void ISR_Init(void);
void ALL_Init(void);

#endif

