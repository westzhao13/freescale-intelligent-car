/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "Data.h"

uint8_t IntegrationTime = 0;

uint8_t  Time = 0;
uint8_t  DisPlayTime = 0,DisPlay = 0;
uint8_t  UartTime = 0,UartOK = 0;
uint16_t StartTime = 0;  //启动后直立时间	
uint8_t  Start = 0;	//启动标志位

uint8_t  AngleOutPeriod = 0;
uint8_t  SpeedOutPeriod = 0;

int16_t  AnglePWM = 0;
int16_t  SpeedPWM = 0;
int16_t  TurnPWM = 0;
int16_t  TurnPWML = 0;
int16_t  TurnPWMR = 0;
uint8_t  CCDCount = 0;
uint8_t  SpeedCount = 0;
uint8_t  TurnCount=0;

uint8_t  SpeedSwitch = 0;
uint8_t  TurnSwitch = 0;

uint16_t blacktime = 0;
uint16_t podao_time;
uint16_t infrared_value = 0;
uint16_t podao_value = 0;
uint16_t podao_flag = 0;
uint16_t podao = 0;
uint16_t black_threshold = 3300;
uint16_t podao_threshold = 900;
uint16_t podao_start_time = 0;
uint8_t  podao_start = 0;
uint16_t podao_angle = 3;
uint16_t podao_last_time = 1500;


uint16_t podao_angle_time = 0;
uint8_t  CCD_Mode = 0;// ccd扫线模式 0为打脚采跳变 单线凹槽 障碍宽度
																	// 1为打脚采宽度 单线凹槽 障碍宽度

uint8_t Start_Mode = 0; //0模式为正常检测起跑线 1模式为关闭检测停车 2模式为关闭起跑停车 延时起跑

/**
*  @name   : ISR_Init
*  @author : ZX
*  @tips   : 中断初始化
*/
void ISR_Init(void)
{
	PIT_InitTypeDef PIT0_Structure;
	PIT0_Structure.PITx = PIT0;
	PIT0_Structure.PIT_Interval = 1; //1ms
	PIT_Init(&PIT0_Structure);
	PIT_ITConfig(PIT0,PIT_IT_TIF,ENABLE);
	NVIC_EnableIRQ(PIT0_IRQn);
}

uint16_t Test = 0; //检测到以后延迟一段时间再开启检测
uint16_t StopFlag = 0;  //检测到停车信号后延时一段时间
uint16_t  StopTime = 15000; //延时的时间
/**
*  @name   : PIT0_IRQHandler
*  @author : ZX
*  @tips   : 周期定时器中断服务函数
*/
void PIT0_IRQHandler(void)
{
	PIT_ClearITPendingBit(PIT0,PIT_IT_TIF);//清除中断标志位
	
	DisableInterrupts(); //关闭总中断
	
	if(StartSwitch == 1)		//正常检测模式 检测起跑停车
	{ 											
		if(Start == 0) 
		{ //等待检测发车
				if(Start_in == 1)  //检测不到 起跑
				{
					Start = 1; //起跑
					start_angle_time = 300;
				}
				else //检测到 等待
				{
					Start = 0;
				}
		}
		else if(Start == 1) //等待检测停车
		{
			Test ++;
			if(Test >= StopTime)
			{
				Test = 0;
				StopFlag = 1;
			}
			if(Start_in == 0 && StopFlag == 1) 
			{ //检测到 进入停车模式 经过计时器检测不到后停车
				/*if(++StopFlag >= StopTime) { //检测到停车信号以后延迟2s停车
					Start = 2; //停车模式
					StopFlag = 0;
				*/
				Start = 2;
			}
				
		}
		else if(Start == 2)  //异常状态下的保护
		{
			if(Start_in == 1) //检测到灯灭
			{
				Start = 3;
			}
		}
		
		if(Start == 3)
		{
			SpeedOut(1,0);
			SpeedOut(2,0);
		}
		
		if(Stop == 1)
		{
			SpeedOut(1,0);
			SpeedOut(2,0);
		}
		
	}
	else if(StartSwitch == 2) //只检测起跑 关闭停车
	{
		if(Start == 0) { //等待检测发车
				if(Start_in == 1)  //检测不到 起跑
				{
					Start = 1; //起跑
					start_angle_time = 200;
				}
				else //检测到 等待
				{
					Start = 0;
				}
		}
		if(Start == 1)
		if(Stop == 1) //异常状态下的保护  
		{
			SpeedOut(1,0);
			SpeedOut(2,0);
		}
		
	}
	else if(StartSwitch == 3) //关闭检测 延时发车
	{
			if(Start == 0) {
				StartTime++;
				if(StartTime == 3000)
				{
					Start = 1;
					start_angle_time = 200;
				}
		  }
			if(Start == 1)
				if(Stop == 1) //异常状态下的保护
				{
					SpeedOut(1,0);
					SpeedOut(2,0);
				}
	}
	
	Time++;           //平衡控制时间
	DisPlayTime++;    //显示刷新时间
	UartTime++;       //发送刷新时间
	
	SpeedOutPeriod++;
	TurnCount++;
	
	if(podao_start == 0)
		podao_start_time++;

	if(podao_start_time >= 6000)
	{
		podao_start_time = 0;
		podao_start = 1;
	}
	
	//每1ms电机输出控制一次
	//增量式控制方式
	SpeedPWM = SpeedPWMOut(Speed,LastSpeed,SpeedOutPeriod); //速度pwm拆分
	
	if((Start != 2 && Stop == 0 && StartSwitch!=1) || (StartSwitch == 1 && Start != 3 && Stop == 0)) //等待 起跑
		Speed_ControlOut(endlong,SpeedPWM,DirectionOut); //速度融合pwm输出
	
	if(Time==5) { 
		Time= 0; //清零 
	}
	
	//角度控制
	else if(Time==1) {
		AngleCalculate(); //获取角度
		Balace_Control(); //角度PD控制
	}
	
	//速度控制
	else if(Time==2) {	
			GetMotorSpeed(); //速度采集  
			SpeedCount++;
	  	if(SpeedCount == 20) {  //100ms速度控制计算
			  SpeedCalculate(); //速度pid计算
			  SpeedCount = 0;//50ms标志清零
				SpeedOutPeriod = 0;
		  }
	}
	
	//CCD采集方向控制
	else if(Time==3) {
		CCDCount++;
		if(CCDCount==2) {
			Read_CCD1(); //曝光结束 CCD采集
			Read_CCD3();
			GetBlackString(Pixels1); //上 巡双线 直角 障碍
			GetBlackString2(Pixels3); //单线 十字
			Speed_Stratage();  // 速度策略
			path_element(); //元素
			errorcalculate();  //偏差计算
			TurnCalculate(); //方向控制pid计算
			CCDCount = 0;	
			TurnCount = 0;
		}
	}
	
	else if(Time==4) {
		
	}
	
	//交互界面控制
	if(DisPlayTime==50) {
		DisPlay = 1;
		DisPlayTime = 0;
	}
	
	if(UartTime==2) {
		UartOK = 1;
		UartTime = 0;
	}
	
	if(Zhijiao_Switch && !podao) 
	{ //直角
			if(++blacktime >= 200)
			{
				blacktime = 200;
				if(Black1_in == 1 && Black2_in == 1) // black
				{
						if(black_area == 0) 
						{
							black_area = 1;
							black_angle_time = 100;
							//Beep();
						}
						else  
						{
							black_area = 0;
							zhijiao_cnt = 0;
							black_angle_time = 0;
							//NoBeep();
						}
						blacktime = 0;
				}
			}
	}
	
	// 坡道 对管不怎么准确
		if(Podao_Switch && podao_start) 
		{
				if(++podao_flag>500) 
				{
					podao_flag = 500;
					podao_value = ADC_QuickReadValue(Infrared_Podao_Map);
					if(podao_value < podao_threshold) 
					{
						if(podao==0)  
						{
							podao = 1;
							podao_angle_time = 200;
						}
						
						podao_flag = 0; 
					} 
				}
			
			 if(podao) 
			 {
				 if(++podao_time > podao_last_time)//1.5s
				 {
					 podao_time = 0;
					 podao = 0; 
					 podao_angle_time = 0;
				 }
				 //Obstacle_Switch = 0;
				// Single_Mid_Switch = 0;
			 }
	 }
	
	if( obstacle || zhijiao || podao || mid_black || shizi || black_area) Beep();
  else           NoBeep();
	
	EnableInterrupts();
}
