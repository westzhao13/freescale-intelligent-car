/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*						Edited on 2014.9.11. Added ccd,uart,ftm,adc,lptmr driver. Used to test CCD.
*						Edited on 2014.9.21. Fixed FTM bug and added the pulse width transformed to duty
																 and added the sevro control duty.Tested the CCD_Deal 's time.
																 edited the adc.c on the hardwareaverage_32.

						Edited on 2014.10.27 added MMA8452 and L3G4200D
						Edited on 2014.11.06 added MMA7361L
							 
				MAP : 
							 编码器 C5 A5
							 CCD:
								 CCD1 SI:C6    CLK:C10   AD:C0
								 CCD2 SI:C4    CLK:C8    AD:C1
								 CCD3 SI:C15   CLK:C17   AD:B2
							 电机 左:D4 D5  右:D6 D7
							 OLED
									SCL   			B23   //D0
									SDA         B21   //D1
									RST         B19   //RST
									DC          B17   //DC
							 L3G4200D     
									SCL:B0    SDA:B1
							 MMA7361
							 X:B10  Y:B11  Z:A17 GS:C9 SL:C11
							 串口
									RX:E24   TX:E25
							 KEY
							 S8:A4 S7:A16 S3:A12 S5:A13 S6:B9
							 
							 endlongp 3071	endlongd 200 angelset 225 amspeed 1260 speedp 170 speedi 60 speedd0
						turnp 340 turnd 620   lmd570 rmd200 threshold 10
						
						一档 Amspeed 
						二挡 Amspeed 1450 SpeedP 140 I 70 TurnP 324 D620 LM250 RM325 zhijiaoflag 13
						三挡         1250        140   70       340  500   240   300             16
						===========================================================================================
						国赛 策略！！！  edited on 7-26  ------- By ZX
						
						1.改变机械结构 更精致 并且改变电池的架法 绑在电路板上  或者 嵌入到车模底部仿去年南师大 
						2.元素的优化 单线的判断条件 应加入黑线的跳变 再进行凹槽扫描 并且凹槽旁边为白 单线的丢线处理
							直角的zhijiao_count应随着速度不一样而改变 出直角可尝试使用编码器 
							出弯黑带的处理
						3.ccd提取黑线的算法需要优化 比如应该丢弃一些错误的图像 可简单判断出出入十字 图像主要是十字的处理 
						4.打脚的处理 普通pd 加 转向陀螺仪 再加上曲率系数 曲率计算可参考去年北邮技术报告 进一步了解一阶衰减输出 并且可加入动态pd 
						5.两个ccd的处理需要做好 坡道可尝试用ccd判断 将两个ccd利用起来 远前瞻主要起到判断的作用 障碍可靠远前瞻 弯道类型也可靠远前瞻
						6.改变陀螺仪的初始化 完善滤波可尝试对陀螺仪读回来的值进行处理 比如 进行一阶衰减 高通低通滤波 
						7.去除CCD暗电流 软件方法硬件方法都行

						在CCD图像算法方面可参考第九届郑州轻工业 北邮的技术报告 
						CCD个数尽量已少为好，方案1 一个打脚加单线 一个判断元素 一个弥补丢线最近前瞻
																 方案2 一个打脚加单线 一个判断元素 丢线靠打死
						
*/
#include "includes.h" //相关头文件 函数声明 宏定义 外部全局变量 都包含在此头文件中

/**
*  @name   : main
*  @author : ZX
*  @tips   : 主函数
*/
int main(void)
{
	DisableInterrupts(); //关闭总中断
	
	ALL_Init(); //所有模块初始化
	
	/**
	*@function Flash掉电存储 读数据
	*/
	Data_Read();
	
	/**
	*@Tips!!!!  校准参数
	*/
	/*
	if(EndlongP == 65535)   EndlongP = 3200;
	if(EndlongD == 65535)   EndlongD = 250;
	*/
	/*
	if(Angle_Set ==  45)  Angle_Set = 140;
	if(black_threshold == 65535) black_threshold = 58000;
	if(podao_threshold == 65535) podao_threshold = 800;
	if(AmSpeed ==  20)    AmSpeed = 1900;
	if(Speed_P ==  0)    Speed_P = 120;
	if(Speed_I ==  0)    Speed_I = 0;
	if(Speed_D ==  65535)    Speed_D = 0;
	if(S_bar_cnt == 45)   S_bar_cnt = 20000;
	if(DirectionP ==  40) DirectionP = 400;
	if(DirectionD ==  45) DirectionD = 600;
	if(zhijiao_count == 255) zhijiao_count = 13;	
	if(Threshold ==  10)    Threshold = 35;
	if(Threshold2 ==  255)    Threshold2 = 35;
	//if(Mid_Threshold ==  255)    Mid_Threshold = 40;
	if(LeftMotorDead == 10) LeftMotorDead = 250;
	if(RightMotorDead == 40) RightMotorDead = 250;
	
	if(Obstacle_Switch == 255)    Obstacle_Switch = 1;
	if(Single_Mid_Switch == 255)    Single_Mid_Switch = 1;
	if(Zhijiao_Switch == 255)    Zhijiao_Switch = 1;
	if(Podao_Switch == 255)    Podao_Switch = 1;
	if(StartSwitch == 255)    StartSwitch = 3;
	if(obstacle_conut == 255) obstacle_conut = 1;
	if(StopTime == 65535) StopTime = 1000;
	if(Auto == 255) Auto = 0;
	if(Up_Threshold == 255) Up_Threshold = 30;
	if(GYRO_Turn == 65535) GYRO_Turn = 350;
	
	*/
	/*
	Speed_P=130;
	S_bar_cnt = 20000;
	DirectionP = 380;
	DirectionD = 550;
	zhijiao_count = 13;
	Threshold = 30;
	Threshold2 = 35;
	Mid_Threshold = 40;
	Up_Threshold = 30;
  */
	//AmSpeed = 1600;
	//DirectionP = 360;
	//DirectionD = 700;
	EnableInterrupts(); //打开总中断
	while(1)
	{
		#if(TESTTIME==ON) //若选择测试程序运行时间 编译以下代码 用示波器观察IO口
			PAout(11) = 1;
			//Get_Angel();
			//RD_TSL(Pixels1);
			//RD_TSL(Pixels1
		  Get_Value();
			Get_Angel(); //获取角度
		  Balace_Control(); //角度PD控制
			PAout(11) = 0;
			//Get_Angel();
			//RD_TSL(Pixels1);
			//RD_TSL(Pixels1);
		  Get_Value(); 
		  Get_Angel(); //获取角度
		  Balace_Control(); //角度PD控制
			//
		
		#else	//正常运行
			if(UartOK) //每5ms发送一次
			{
			  /*
				if(CCDflag == 1)
					SendImageData(Pixels1); //发送CCD图像
			  else if(CCDflag == 2)
			   	SendImageData(Pixels3);
				*/
			//	Data_Send_Senser(GYRO_X,ACC_Z_Fix,ACC_Y_Fix,GYRO_Y,Angle_ACC*1000,Angle*1000,0); //匿名上位机发送六组数据看波形
			  Data_Send_Senser(LeftSpeedNow,RightSpeedNow,CarSpeed,GYRO_Y,ccd_average,mid_ave,0); //匿名上位机发送六组数据看波形
			// Data_Send_Senser(LeftBlack,RightBlack,LeftBlack2,RightBlack2,(LeftBlack+RightBlack)/2,obstacle_width,0); //匿名上位机发送六组数据看波形
				//Serial_Send(Angle_Speed,Angle_IGYRO,Angle_ACC,Angle);
				//Data_Send_Senser(SpeedP,SpeedI,Speed,SpeedPWM,endlong,Angle,Angle_IGYRO); //匿名上位机发送六组数据看波形
				UartOK = 0;
			} 
			if(DisPlay) //每50ms刷新一次显示屏  OLED使用直接调用Oled.h里面的显示函数
			{			
				
		   MenuDisplay();
			//	menu();
				DisPlay = 0;
			}
		 KeyScan(); //加入Flash 按下确认建后保存参数
		#endif
	}
}
/**
*  @name   : assert_failed
*  @author : ZX
*  @tips   :
*/
void assert_failed(uint8_t* file, uint32_t line)
{
	//断言失败检测
	UART_printf("assert_failed:line:%d %s\r\n",line,file);
	while(1);
}
