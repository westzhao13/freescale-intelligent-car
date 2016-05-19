/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/

#include "Balance.h"
#include "math.h"

int16_t GYRO_X;
int16_t GYRO_Y;
int16_t GYRO_Z;

float GYRO_X_Fix;
float GYRO_Y_Fix;
float Angle_Speed;

uint16_t ACC_X;
uint16_t ACC_Y;
uint16_t ACC_Z;

float   ACC_Y_Fix;
float   ACC_Z_Fix;

uint16_t ACC_ZeroY = 26800; //49600
uint16_t ACC_ZeroZ = 36400;//32700

float    Angle_ACC = 0;
float    Angle_IGYRO = 0;
float    Angle = 0;
float    Angle_Error = 0;
float    Angle_Stop = 150;
float    Angle_Start = 0;


uint16_t Angle_Set = 5440;

#define FILTER_NUM 20
int16_t ACC_X_BUF[FILTER_NUM],ACC_Y_BUF[FILTER_NUM],ACC_Z_BUF[FILTER_NUM]; //加速度计平滑滤波数组
int16_t ACC_OFFSET_X,ACC_OFFSET_Y,ACC_OFFSET_Z;
uint8_t ACC_OFFSET_OK = 0;
int16_t ACC_Average_X,ACC_Average_Y,ACC_Average_Z;

uint16_t GRAVITY_ADJUST_TIME_CONSTANT = 362;
uint16_t GYROSCOPE_ANGLE_SIGMA_FREQUENCY = 500;

uint16_t CAR_ACCE_RATIO = 550;//500;//50;
uint16_t CAR_GYRO_RATIO = 190;//180;//180;//28;


uint8_t Stop = 0;
uint8_t Stop_Flag = 1;

void QingHua_AngleCalaulate(int16_t G_angle,int16_t Gyro)
{
	int16_t fDeltaValue;
	
	Angle = Angle_IGYRO;     //最终的角度融合
	fDeltaValue = (G_angle - Angle) / GRAVITY_ADJUST_TIME_CONSTANT;  //时间系数矫正
	Angle_IGYRO += (Gyro + fDeltaValue) / GYROSCOPE_ANGLE_SIGMA_FREQUENCY; //融合角度
}

/**
*  @name   : getangle()
*  @author : ZX
*/
uint16_t start_angle_time = 0;
uint8_t  chupo = 0;
void AngleCalculate(void)
{
	float fDeltaValue;
	
	//加速度计
	ACC_Y = Read_ACC_Y();
	ACC_Y_Fix = ACC_Y + ACC_ZeroY;
	ACC_Z = Read_ACC_Z();
	ACC_Z_Fix = -ACC_Z + ACC_ZeroZ;
	
	//角度倾角控制
	
	if(black_angle_time)
	{
		Angle_Start = Angle_Set - zhijiao_Angle;
		black_angle_time--;
		if(black_angle_time<=1)
			black_angle_time = 0;
	}
	else if(start_angle_time) {
		Angle_Start = Angle_Set + 45;
		start_angle_time --;
		if(start_angle_time <= 1)
		{
			start_angle_time = 0;
		}
	}
	else if(podao_angle_time) {
		Angle_Start = Angle_Set -  podao_angle;// - 15;
		podao_angle_time --;
		if(podao_angle_time <= 1)
		{
			podao_angle_time = 0;
		}
	}		
	else{
		Angle_Start = Angle_Set;
	}
	
	Angle_ACC = 125*atan2(ACC_Z_Fix,ACC_Y_Fix) - Angle_Start / 10.0; //175
	//Angle_ACC = ACC_Z_Fix / 15.0;
	//Angle_ACC = kalman(Angle_ACC) - Angle_Set / 10.0;
	//陀螺仪
	GYRO_X = Read_Gyro_X();
	GYRO_X_Fix = mult_r(GYRO_X,300);
	Angle_Speed = GYRO_X_Fix;
	//GYRO_Y = Read_Gyro_Y();
	//GYRO_Y_Fix = mult_r(GYRO_Y,100);
	//互补
	Angle = Angle_IGYRO;
	fDeltaValue = (Angle_ACC - Angle) * CAR_ACCE_RATIO / 1000.0;
	Angle_IGYRO += (GYRO_X_Fix+fDeltaValue) * CAR_GYRO_RATIO / 10000.0;

	
	if(Stop == 0 && podao == 0 && Stop_Flag == 1) {
		if((Angle > 9 || Angle < -6 ) && podao == 0)
		{
			Stop = 1;
		}
	}
	
}


/**
*  @name   : Get_Angel()
*  @author : ZX
*/

void Get_Value(void)
{
	ACC_Y = Read_ACC_Y();//- ACC_ZeroY;
	ACC_Y_Fix = ACC_Y- ACC_ZeroY;
	ACC_Z = Read_ACC_Z();//- ACC_ZeroZ;
  ACC_Z_Fix = ACC_Z- ACC_ZeroZ;
	
	GYRO_X = Read_Gyro_X();
}

/**************************************************************************************************
函数名：kalman              
函数功能·：陀螺仪卡尔曼滤波
***************************************************************************************************/
int kalman(int data)
{
  static int p1=6;
  static int c1=5;
  int q=5;
  int r=10;
  float kg,c2,p2;
  
	
  p2=p1+q;
  kg=p2*p2/(p2*p2+r*r);
  c2=c1+kg*(data-c1);
  p1=(1-kg)/p2;
  c1=c2;
	
  return c1; 
}

int Kalman_Gyro(int data)
{
  static int a1=1;
  static int c1=1;
  int q=1;
  int r=1;
  float kg,c2,a2;
  
	
  a2=a1+q;
  kg=a2*a2/(a2*a2+r*r);
  c2=c1+kg*(data-c1);
  a1=(1-kg)/a2;
  c1=c2;

  return c1; 
}

/**************************************************************************************************
函数名：mult_r()              
函数功能·：参数归一化
***************************************************************************************************/
int mult_r(int get_data,int mult_get)
{

	float temp;
	

	temp = get_data/mult_get;
	return temp;

}
/************************?????*******************************/                               
 float C_0 = 1;                   
 float Q_angle=0.001;               /*  对加速度计的信任指数 */
 float Q_gyro=0.003;                /*  对陀螺仪的信任指数   */
 float R_angle=0.5;                 /*   测量过程中的误差    */
 float dt=0.005;                    /*       积分时间         */

 float P[2][2] = {{ 1, 0 },{ 0, 1 }};
 float Pdot[4] ={0,0,0,0};
 float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
/*************************?????*********************************************/
void Kalman_Filter(float angle_m,float gyro_m)	//gyro_m:gyro_measure
{                                      
	Angle+=(gyro_m - q_bias) * dt; //通过陀螺仪求得的角度相当于预测值，而加速度计测得的值是测量值
	
	Pdot[0] = Q_angle - P[0][1] - P[1][0];
	Pdot[1] -= P[1][1];
	Pdot[2] -= P[1][1];
	Pdot[3] = Q_gyro;
	
	P[0][0] += Pdot[0] * dt;
	P[0][1] += Pdot[1] * dt;
	P[1][0] += Pdot[2] * dt;
	P[1][1] += Pdot[3] * dt;
		
	angle_err = angle_m - Angle;  

	 PCt_0 = P[0][0];         //C_0*
	 PCt_1 = P[1][0];         //C_0*

	 E = R_angle + PCt_0;      //C_0*

	 K_0 = PCt_0 / E;
	 K_1 = PCt_1 / E;

	 t_0 = PCt_0;
	 t_1 = P[0][1];           //C_0*
	 P[0][0] -= K_0 * t_0;
	 P[0][1] -= K_0 * t_1;
	 P[1][0] -= K_1 * t_0;
	 P[1][1] -= K_1 * t_1;
	
	
	Angle	+= K_0 * angle_err;
	q_bias	+= K_1 * angle_err;
  Angle_Speed = gyro_m-q_bias;
}
