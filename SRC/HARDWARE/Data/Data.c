/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "Data.h"
/**
*  @name   : ALL_Init
*  @author : ZX
*  @tips   : 
*/
void ALL_Init(void)
{
  SystemClockSetup(
									ClockSource_EX50M,
									CoreClock_100M);
											//初始化系统时钟 使用外部50M晶振 PLL倍频到200M
	
	Uart_Init(); 				//串口初始
	
	DelayInit();				//初始化延时模块
	
	#if(TESTTIME==OFF)
		ISR_Init();       //周期定时器中断初始化
	#endif
	
	Encoder_Init();			//编码器测速初始化
	
	CCD1_Init();		    //CCD1初始化
	
	OLED_Init();        //OLED初始化
	 
//	OLED2_Init();       //OLED2初始化
	
	KeyInit();					//按键初始化
	
 //DelayMs(2);
	
	InitL3G4200D();     //陀螺仪初始化
	
	MMA7361_Init(); 		//加速度计初始化
	
	Motor_Init();       //电机初始化
	
	//LED_Init();       //指示灯初始化
	
	LED_Test_Init();    //测试灯初始化
	
	DFlash_Init();      //DFlash初始化
	
	UART_printf("Init Success!");
}


