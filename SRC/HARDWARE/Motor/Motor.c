/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "Motor.h"
/**
*  @name   : Motor_Init
*  @author : ZX
*  @tips   : 
*/
void Motor_Init(void)
{
	FTM_InitTypeDef PWM1_Structure;
	PWM1_Structure.Frequency = 20000; //20k
	PWM1_Structure.FTM_Mode = FTM_Mode_EdgeAligned;
	
	PWM1_Structure.InitalDuty = 0;
	PWM1_Structure.FTMxMAP = Left_Forward_MAP;
	FTM_Init(&PWM1_Structure);
	
	PWM1_Structure.InitalDuty = 0;
	PWM1_Structure.FTMxMAP = Left_Backward_MAP;
	FTM_Init(&PWM1_Structure);
	
	PWM1_Structure.InitalDuty = 0;
	PWM1_Structure.FTMxMAP = Right_Forward_MAP;
	FTM_Init(&PWM1_Structure);
	
	PWM1_Structure.InitalDuty = 0;
	PWM1_Structure.FTMxMAP = Right_Backward_MAP;
	FTM_Init(&PWM1_Structure);
}
/**
*  @name   : Left_Forward_ChangePWM
*  @author : ZX
*  @tips   : 
*/
void Left_Forward_ChangePWM(uint16_t PWM)
{
	FTM_PWM_ChangeDuty(Left_Forward_MAP,PWM);
}
/**
*  @name   : Left_Backward_ChangePWM
*  @author : ZX
*  @tips   : 
*/
void Left_Backward_ChangePWM(uint16_t PWM)
{
	FTM_PWM_ChangeDuty(Left_Backward_MAP,PWM);
}
/**
*  @name   : Right_Forward_ChangePWM
*  @author : ZX
*  @tips   : 
*/
void Right_Forward_ChangePWM(uint16_t PWM)
{
	FTM_PWM_ChangeDuty(Right_Forward_MAP,PWM);
}
/**
*  @name   : Right_Backward_ChangePWM
*  @author : ZX
*  @tips   : 
*/
void Right_Backward_ChangePWM(uint16_t PWM)
{
	FTM_PWM_ChangeDuty(Right_Backward_MAP,PWM);
}
/**
*  @name   : SpeedOut
*  @author : ZX
*  @tips   : 
*/
void SpeedOut(int PTCx,int PWMx)
{
	 int temp = 0;
	
   if(PWMx==0)
	 {
			temp = 0;	
	 }	
   switch(PTCx)
	 {
		 case	1 : 
		 if(PWMx <= 0)
		 {
				Left_Backward_ChangePWM(0);
				Left_Forward_ChangePWM(-PWMx+temp);//左正*adjust
				
		 }
		 else
		 {	
		    Left_Forward_ChangePWM(0);
				Left_Backward_ChangePWM(PWMx+temp);  //左负 
						
		 }		
		 break;
	   case	2 : 
		 if(PWMx <= 0)
		 {
		    Right_Backward_ChangePWM(0);
				Right_Forward_ChangePWM(-PWMx+temp); //右正
				
		 }
		 else
		 {
		    Right_Forward_ChangePWM(0); 
				Right_Backward_ChangePWM(PWMx+temp); //右负  
				
		 }
		 break;
	 }
}
