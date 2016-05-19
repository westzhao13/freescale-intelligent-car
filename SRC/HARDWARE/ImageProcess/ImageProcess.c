/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "ImageProcess.h"

//blackstring
uint8_t LeftBlack = 0,RightBlack = 0;       //这一次左右黑线的坐标
uint8_t Q_LeftBlack = 7,Q_RightBlack = 120; //上一次左右黑线的坐标
uint8_t begin = 64; //扫描起始点
uint8_t mid = 64;   //赛道中线
uint8_t LxQ,RxQ = 0; //missing black flag 
uint8_t Threshold = 5;         //利用跳变沿算法的跳变阈值
uint8_t Threshold2;
uint8_t track_width = 0;        //赛道宽度
uint8_t trackwidth[10] = {0};      //存放赛道宽度的数组
uint8_t LeftWhite=0,RightWhite = 0;  //左边右边全白的标志位
int8_t offset = 0,last_offset = 0; //一阶偏差
int8_t offset_two_order  = 0; //二阶偏差
uint8_t CCD_lowview_switch = 0; //低前瞻ccd开关
uint8_t CCD_black = 0;
uint8_t Mid_Threshold = 25; //单线阈值
int8_t obstacle_axis = 0;
uint8_t single_black = 0;
uint8_t single;//单线模式
uint8_t zhijiao;
uint8_t Left_zhijiao;
uint8_t Left_zhijiao_flag = 0;
uint8_t Right_zhijiao;
uint8_t Right_zhijiao_flag = 0;
uint8_t LeftBlack2 = 0,RightBlack2 = 0,begin2 = 64;
uint8_t Q_LeftBlack2,Q_RightBlack2;
int8_t  offset2,lastoffset2,offset_two_order2;
uint8_t LxQ2,RxQ2;
uint8_t LxQ3,RxQ3;
uint8_t LxQ4,RxQ4;
uint8_t Down_sec_Q;
uint8_t  Lc = 0,Rc = 0; //左雍谝线宽度
int8_t  Lc2= 0;
uint8_t  obstacle_width = 0;
uint8_t  Lc_last = 0,Rc_last = 0; //上一次左右黑线宽度
uint8_t  Up_Threshold = 0;
uint8_t out_zhijiao;
uint8_t average_threshold = 0;
uint8_t black_area_flag = 0;
uint8_t black_area = 0;
uint16_t black_angle_time = 0;
uint16_t black_angle_time_out = 0;
uint8_t first_black = 0;
uint8_t second_black = 0;
uint8_t black_count = 0;
uint16_t average = 0;
uint8_t black = 0;
uint8_t mid_left = 0,mid_right = 0,mid_left2 = 0,mid_right2  =0;
int8_t single_mid = 0,single_mid2,single_mid_fix = 0;
uint8_t mid_black = 0;
uint8_t obstacle_black = 0;
uint8_t out_black = 0;
uint8_t obstacle = 0,obstacle_flag,obstacle_flag_down_sec;
uint8_t obstacle_conut = 2;
uint8_t obstacle_left_flag= 0,obstacle_right_flag = 0;
uint8_t Tubian = 0;
uint8_t Tubian_L,Tubian_R;
uint8_t L_ObstacleCnt,R_ObstacleCnt;
uint8_t L_Obstacle,R_Obstacle;
uint32_t S_bar;
uint16_t S_bar_cnt = 1000;

uint32_t zhijiao_s_bar;
uint16_t zhijiao_s_bar_cnt = 1000;

uint8_t obstacle_mid;
uint16_t mid_black_time = 3;
uint8_t  obstacle_down,obstacle_up,obstacle_down_sec;
uint8_t  zhijiao_count = 13;
uint8_t zhijiao_num = 1; //直角个数
uint8_t zhijiao_flag = 0;

//元素开关
uint8_t Obstacle_Switch;
uint8_t Single_Mid_Switch;
uint8_t Zhijiao_Switch;
uint8_t Podao_Switch;

uint8_t Auto = 0; //动态阈值

uint8_t error_field = 0; //错误场

 /**
*  @name   : PictureDiff
*  @author : ZX
*  @tips   : CCD一维图像一阶差分
*/
uint8_t ImageDiff[127];
void PictureDiff(uint8_t *image)
{
	uint8_t i;
	for(i = 0;i < 128 - 1;i++)
	{
		*(ImageDiff+i) = abs(*(image+i) - *(image+i+1)); //保存数据 绝对值
		
	}
}

/**
*  @name   : CCD_Average
*  @author : ZX
*  @tips   : CCD一维图像 AD值排序
*/
uint8_t ImageSort[128];
void PictureADSort(uint8_t* image)
{		
	int i,j,temp;
	for(i = 0; i<128;i++)
	{
		*(ImageSort+i) = *(image+i);
	}
	
	for(j = 0;j < 128-1;j++)
			for(i=0;i<128-1-j;i++)
			{
					if( *(ImageSort+i) > *(ImageSort+i+1) )
					{
							temp=ImageSort[i];
							ImageSort[i]=ImageSort[i+1];
							ImageSort[i+1]=temp;
					}
			}
}

/**
*  @name   : CCD_Average
*  @author : ZX
*  @tips   : 
*/
uint16_t CCD_Average(uint8_t *image)
{
	uint8_t i;
	uint16_t sum = 0;
	uint16_t ccd_average = 0;
	
		for(i = 6; i <= 118; i++)
		{
			sum += *(image + i);
			
		}

	ccd_average = sum / 112;
	return ccd_average;
}

/**
*  @name   : Auto_Threshold
*  @author : ZX
*  @tips   : 动态阈值计算
*/
uint8_t Diff = 0;
uint16_t Sum = 0;
uint16_t Diff_Average = 0;
uint16_t AD_Average = 0;
uint8_t  Auto_Threshold(uint8_t *image)
{
	uint8_t i;
	uint8_t New_Threshold;
	Sum = 0;
	for(i = 6;i < 121 - 3;i++)
	{
		*(ImageDiff+i) = abs(*(image+i) - *(image+i+3)); //保存数据 绝对值
		Sum += *(ImageDiff + i); //求和
	}
	Diff_Average = Sum / 112; //平均差分
	AD_Average = CCD_Average(image); //CCD平均电压值
	New_Threshold = AD_Average / 6 + Diff_Average; //阈值
	
	return New_Threshold;
}

//-----------------控制代码-------------------//
/**
*  @name   : GetBlackString
*  @author : ZX
*  @tips   : 
*/
void GetBlackString(uint8_t *line)
{                                               
	uint8_t i;           
	uint8_t flag1=0,flag3=0;    //flag1左边计数标记，flag3右边计数标记
	
	uint8_t exl=0,exr=0; //scan width 扫描宽度
  
	//Threshold = Auto_Threshold(line); //动态阈值计算 每一场刷新一次
	
	if(Auto == 1) { //开启动态阈值
		Threshold = Auto_Threshold(line);
	}
	
	Q_LeftBlack = LeftBlack; 
	Q_RightBlack = RightBlack;
	
	// set scan width
	
	if(begin==6)              
		 exr=50;            
	if(begin==121)
		 exl=50;
	
  //clean the flag
	LxQ = 0;
	RxQ = 0;

	

	
 //scan left
	for(i = begin;i > 3 + exl;i--)  
	{
		 if((*(line + i)-*(line + i - 3)) >= Threshold)  //隔四点做差 利用阈值找到跳变点     
		 { 
				flag1++;                                   
				if(flag1 >= 3)                                          
				{
					 LeftBlack = i;
					 LxQ=1;
					 break;
				}
		 }
		 else
		 {
			flag1 = 0;
		 }	 
	}
	
	//scan right
	for(i = begin;i < 118 - exr;i++)
	{
		if((*(line + i)-*(line + i + 3)) >= Threshold)
		{ 
				flag3++;
				if(flag3 >= 3)
				{
					RightBlack = i;
					RxQ=1;
					break;
				}
		}
		else 
		{
			flag3 = 0;
		}
	}
  
	//missing black
  if(flag3 < 3)                          
  {
    if(begin > 6)
		{			
       RightBlack = 127; // no black
    }
		else
		{			
      RightBlack = 0; //missing black and hold on
			//CCD_lowview_switch = 1; //打开低前瞻ccd
		}
	}
     
  if(flag1 < 3)
  {
   	if(begin < 121)                      
    {
			LeftBlack = 0; //white
		}
		else
		{			
      LeftBlack = 127; // missing black and hold on
			//CCD_lowview_switch = 1;
		}  
	}
	
  //reset the next begin point	
  begin = (LeftBlack+RightBlack) >> 1; 
  
	//参数有待修改 此处可能有BUG
 
	if(begin < 30) {   //28                      
    begin = 6; 
	}
  if(begin > 97) {   //99                           
    begin = 121;
	}
}

/**
*  @name   : GetBlackString
*  @author : ZX
*  @tips   : 
*/
void GetBlackString2(uint8_t *line)
{                                               
	uint8_t i;           
	uint8_t flag1=0,flag3=0;    //flag1左边计数标记，flag3右边计数标记
	
	uint8_t exl=0,exr=0; //scan width 扫描宽度
  
	//Threshold = Auto_Threshold(line); //动态阈值计算 每一场刷新一次
	
	if(Auto == 1) { //开启动态阈值
		Threshold = Auto_Threshold(line);
	}
	
	Q_LeftBlack2 = LeftBlack2; 
	Q_RightBlack2 = RightBlack2;
	
	// set scan width
	
	if(begin2 == 6)              
		 exr=50;            
	if(begin2 == 121)
		 exl=50;
	
  //clean the flag
	LxQ3 = 0; 
	RxQ3 = 0;
	
 //scan left
	for(i = begin2;i > 3 + exl;i--)  
	{
		 if((*(line + i)-*(line + i - 3)) >= Threshold)  //隔四点做差 利用阈值找到跳变点     
		 { 
				flag1++;                                   
				if(flag1 >= 3)                                          
				{
					 LeftBlack2 = i;
					 LxQ3=1;
					 break;
				}
		 }
		 else
		 {
			flag1 = 0;
		 }	 
	}
	
	//scan right
	for(i = begin2;i < 118 - exr;i++)
	{
		if((*(line + i)-*(line + i + 3)) >= Threshold)
		{ 
				flag3++;
				if(flag3 >= 3)
				{
					RightBlack2 = i;
					RxQ3=1;
					break;
				}
		}
		else 
		{
			flag3 = 0;
		}
	}
  
	//missing black
  if(flag3 < 3)                          
  {
    if(begin2 > 6)
		{			
       RightBlack2 = 127; // no black
    }
		else
		{			
      RightBlack2 = 0; //missing black and hold on
			//CCD_lowview_switch = 1; //打开低前瞻ccd
		}
	}
     
  if(flag1 < 3)
  {
   	if(begin2 < 121)                      
    {
			LeftBlack2 = 0; //white
		}
		else
		{			
      LeftBlack2 = 127; // missing black and hold on
			//CCD_lowview_switch = 1;
		}  
	}
	
  //reset the next begin point	
  begin2 = (LeftBlack2 + RightBlack2) >> 1; 
  
	//参数有待修改 此处可能有BUG
 
	if(begin2 < 30) {   //28                      
    begin2 = 6; 
	}
  if(begin2 > 97) {   //99                           
    begin2 = 121;
	}
}
/**
*  @name   : MidBlack_Deal
*  @author : ZX
*  @tips   : 
*/
uint8_t LxQ5 = 0, RxQ5 = 0;
void MidBlack_Deal(uint8_t *line)
{
	uint8_t mid_flag1 = 0,mid_flag2 = 0; //中线扫描
	uint8_t mid_flag3 = 0,mid_flag4 = 0;
	uint8_t i = 0;
	
	LxQ2 = 0; //单线
	RxQ2 = 0;
	LxQ5= 0;
	RxQ5 = 0;
	
	Lc = 0; //单线宽度
	Lc2 = 0;
	single_mid = 0; //单线标志清零
	single_mid2 = 0;
	
	//从左边扫黑槽
	for(i = 6 ; i < 121; i++)
	{
		if((*(line + i)-*(line + i + 2)) >= Mid_Threshold) 
		{ 
			mid_flag1++;                                   
			if(mid_flag1 >= 2)                              
			{
					mid_left = i;
					LxQ2 = 1;
					for(i = mid_left; i < 120 ; i++) { 
							if((*(line + i + 2)-*(line + i)) >= Mid_Threshold)      
							{  
									mid_flag2++ ;                                  
									if(mid_flag2 >= 2)                                           
									{
										 mid_right = i;
										 Lc = mid_right - mid_left;                        
										 if(Lc >= 1 && Lc <= 8) {           
												single_mid = (mid_right + mid_left )>> 1;
												RxQ2 = 1;	
										 }
										 
										 break;
									}
							}
							else {
								 mid_flag2 = 0;
							}
					}
					break;
			}
		}
		else{
			mid_flag1 = 0;
		}
	}
 //从右边扫黑槽 
	for(i = 121 ; i > 6; i--)
	{
		if((*(line + i)-*(line + i - 2)) >= Mid_Threshold) 
		{ 
			mid_flag3++;                                   
			if(mid_flag3 >= 2)                              
			{
					mid_left2 = i;
					LxQ5 = 1;
					for(i = mid_left2; i > 6 ; i--) { 
							if((*(line + i - 2)-*(line + i)) >= Mid_Threshold)      
							{  
									mid_flag4 ++ ;                                  
									if(mid_flag4 >= 2)                                           
									{
										 mid_right2 = i;
										 Lc2 = mid_left2 - mid_right2;                        
										 if(Lc2 >= 1 && Lc2 <= 8) {           
												single_mid2 = (mid_right2 + mid_left2 )>> 1;
												RxQ5 = 1;
										 }
										 
										 break;
									}
							}
							else {
								 mid_flag4 = 0;
							}
					}
					break;
			}
		}
		else{
			mid_flag3 = 0;
		}
	}
	
	
	if(Single_Mid_Switch && shizi == 0 && zhijiao == 0 && podao == 0 && black_area == 0) { //单线开关
		if(LxQ2 !=0 && RxQ2 != 0 && (abs(single_mid2 - single_mid) <= 5 )&& LxQ5 != 0 && RxQ5 != 0) //单线
		{
			mid_black = 1;
			offset2 = single_mid  - 64;
			begin = 64;
			LeftBlack = 0;
			RightBlack = 127;
			//NoBeep();
		}
		else
		{
			mid_black = 0;
			//NoBeep();
		}
	}
	
	
}


/**
*  @name   : Obstacle_Deal()
*  @author : ZX
*  @tips   : 
*/
void Obstacle_Deal(uint8_t *line)
{
	uint8_t obstacle_flag_down = 0,obstacle_flag_up = 0; //障碍扫描
	uint8_t i = 0;
	
	LxQ4 = 0; //宽度 障碍
	RxQ4 = 0;
	Down_sec_Q = 0;
	
	
  obstacle_width = 0; //障碍宽度
	obstacle_axis = 0;  //障碍坐标
	CCD3_width = RightBlack2 - LeftBlack2;
	
	
		for(i = 24; i < 103; i++)
		{
			if((*(line + i)-*(line + i + 3)) >= Threshold2) 
			{ 
				obstacle_flag_down++;                                   
				if(obstacle_flag_down >= 3)                                          
				{
						obstacle_down = i;
						LxQ4 = 1;
						for(i = obstacle_down; i < 100 ; i++) { 
								if((*(line + i + 3)-*(line + i)) >= Threshold2)
								{  
										obstacle_flag_up++ ;                                  
										if(obstacle_flag_up >= 3)                                           
										{
											 obstacle_up = i;
											 obstacle_width = obstacle_up - obstacle_down;
											 obstacle_axis = (obstacle_up + obstacle_down ) >> 1;
											 if(obstacle_down > 64 && obstacle_up > 64)
											 {
												 L_Obstacle = 0;
												 R_Obstacle = 1;
											 }
											 else if(obstacle_down < 64 && obstacle_up < 64)
											 {
												 L_Obstacle = 1;
												 R_Obstacle = 0;
											 }
											 RxQ4 = 1;
											
											break;
										}
								}
								else {
									 obstacle_flag_up = 0;
								}
						}
						break;
					}
			}
			else{
				obstacle_flag_down = 0;
			}
		}
		
		if(obstacle_flag_up < 3)
		{
			obstacle_up = 0;
		}
		if(obstacle_flag_down < 3)
		{
			obstacle_down = 0;
		}
		
		if(Obstacle_Switch && mid_black == 0 && zhijiao == 0 && black_area == 0 && podao == 0 && Start != 0) { //障碍开关
			if(obstacle_width >= 10 && obstacle_width <= 18 && abs(offset) <= 15 && CCD3_width <= 45 && CCD3_width >= 10)
			{
				obstacle_flag++;
			}
			else
			{
				obstacle_flag = 0;
			}
			
			if(obstacle_flag >= obstacle_conut)
			{
				obstacle = 1;
			}
			
			if(obstacle)  //防止单线时候判断到障碍
			{
				if(L_Obstacle)
				{
					LeftBlack = 90;
				}
				else
				{
					RightBlack = 43;
				}
				//Beep();
			}
			else {	
				//NoBeep();
			}
			
			//出障碍
			if(obstacle==1)
			{
				 S_bar+=CarSpeed;
				 if(S_bar>=S_bar_cnt)
				 {
					 S_bar=0;
					 obstacle = 0;
				 }
			}
		}
}

/**
*  @name   : Zhijiao_Deal()
*  @author : ZX
*  @tips   : 
*/
uint8_t zhijiao_Angle = 0;
uint8_t zhijiao_cnt = 0;
void Zhijiao_Deal(void)
{
	if(CarSpeed >= 2100)
	{
		zhijiao_count = 3;
		zhijiao_Angle = 20;
		podao_angle = 10;
	}
	else if(CarSpeed >= 2000 && CarSpeed < 2100)
	{
		zhijiao_count = 4;
		zhijiao_Angle = 15;
		podao_angle = 8;
	}
	else if(CarSpeed >= 1900 && CarSpeed < 2000)
	{
		zhijiao_count = 5;
		zhijiao_Angle = 13;
		podao_angle = 7;
	}
	else if(CarSpeed >= 1800 && CarSpeed < 1900)
	{
		zhijiao_count = 5;
		zhijiao_Angle = 12;
		podao_angle = 6;
	}
	else if(CarSpeed >= 1700 && CarSpeed < 1800)
	{
		zhijiao_count = 6;
		zhijiao_Angle = 11;
		podao_angle = 5;
	}
	else if(CarSpeed >= 1600 && CarSpeed < 1700)
	{
		zhijiao_count = 7;
		zhijiao_Angle = 10;
		podao_angle = 3;
	}
	else if(CarSpeed >= 1500 && CarSpeed < 1600)
	{
		zhijiao_count = 8;
		zhijiao_Angle = 9;
		podao_angle = 2;
	}
	else if(CarSpeed >= 1400 && CarSpeed < 1500)
	{
		zhijiao_count = 10;
		zhijiao_Angle = 6;
		podao_angle = 0;
	}
	else if(CarSpeed >= 1200 && CarSpeed < 1400)
	{
		zhijiao_count = 13;
		zhijiao_Angle = 4;
		podao_angle = 0;
	}
	else if(CarSpeed >= 1000 && CarSpeed < 1200)
	{
		zhijiao_count = 14;
		zhijiao_Angle = 0;
		podao_angle = 0;
	}
	else if(CarSpeed <1000)
	{
		zhijiao_count = 16;
		zhijiao_Angle = 0;
		podao_angle = 0;
	}
	
	
	if(black_area == 1 && Zhijiao_Switch) //黑带 直角开关
	{
		if(zhijiao == 0 && zhijiao_cnt == 0) { //非直角
			//NoBeep();
			//右直角
			if(LeftBlack < 45 && LeftBlack > 0 && RightBlack == 127)
			{
				Left_zhijiao_flag ++;
			}
			else
			{
				Left_zhijiao_flag = 0;
			}
			
			//左直角
			if(RightBlack > 72 && RightBlack < 127 && LeftBlack == 0)
			{
				Right_zhijiao_flag ++;
			}
			else
			{ 
				Right_zhijiao_flag = 0;
			}
			
			
			if(Right_zhijiao_flag >= zhijiao_count || Left_zhijiao_flag >= zhijiao_count)
			{
				zhijiao = 1;
				zhijiao_cnt ++ ;
				if(Left_zhijiao_flag >= zhijiao_count)
				{
					begin = 64;
					//offset = 64;
					Left_zhijiao = 1;
					//Beep();
					Left_zhijiao_flag = 0;
				}
				if(Right_zhijiao_flag >= zhijiao_count)
				{	
					begin = 64;
					//offset = -64;
					Right_zhijiao = 1;
					//Beep();
					Right_zhijiao_flag = 0;
				}
			}
		}
		else //直角
		{
			//出直角
			zhijiao_s_bar+=CarSpeed;
			 if(zhijiao_s_bar>=zhijiao_s_bar_cnt)
			 {
				 zhijiao_s_bar=0;
				 zhijiao = 0;
				 Right_zhijiao = 0;
				 Left_zhijiao = 0;
				 zhijiao_cnt = 0;
			 }
			/*
			if(Left_zhijiao)
			{
				if(Q_LeftBlack > LeftBlack)
				{
					out_zhijiao ++;
				}
				else
				{
					out_zhijiao = 0;
				}
				
				if(out_zhijiao >= 5)
				{
					zhijiao = 0;
					Left_zhijiao = 0;
				}
			}
			
			if(Right_zhijiao)
			{
				if(RightBlack > Q_RightBlack)
				{
					out_zhijiao ++;
				}
				else
				{
					out_zhijiao = 0;
				}
				
				if(out_zhijiao >= 5)
				{
					zhijiao = 0;
					Right_zhijiao = 0;
				}
			}
			*/
			
		}
	}
}

/**
*  @name   : Cross_Deal(uint8_t *line,uint8_t *line2)
*  @author : ZX
*  @tips   : 
*/
int8_t ImageArray[6] = {0};
int8_t mid_ave = 0;
uint8_t White_value = 200;
void Cross_Deal(uint8_t *line,uint8_t *line2)
{
	uint8_t i = 0;
	//保存前十场的左右中心值
	uint8_t mid = 0;
	//uint16_t sum  = 0;
	mid = (LeftBlack + RightBlack) >> 1;
	
	for( i = 0; i<= 4 ;i++)
	{
		ImageArray[i + 1] = ImageArray[i];
	}
	ImageArray[0] = mid; //刷新
	
	//求平均来判断是否为全白
	ccd_average = (*(line2 + 10) + *(line2 + 20) + *(line2 + 40) + *(line2 + 50) + *(line2 + 60) +
								 *(line2 + 70) + *(line2 + 80) + *(line2 + 100) + *(line2 + 110)) / 9;
	/*
	for(i = 0; i<=9 ;i++)
	{
		sum += ImageArray[i];
	}
	*/
	
	//加权平均
	mid_ave = (ImageArray[0] * 4 + ImageArray[1] * 2  + ImageArray[2] * 1 + ImageArray[3] * 1 + ImageArray[4] * 1 + ImageArray[5] * 1) / 10;
	
	
	if(ccd_average >= White_value && !podao && !mid_black && !black_area && LxQ3 == 0 && RxQ3 == 0 && LxQ == 0 && RxQ == 0)
	{
		shizi = 1;
		begin = 64;
		//LeftBlack = 0;
	  //RightBlack = 127;
	}
	else
	{
		
		shizi = 0;
	}
}

/**
*  @name   : path_element
*  @author : ZX
*  @tips   : 
*/
uint16_t zhijiao_time = 0;
uint16_t ccd_average = 0;
uint8_t  shizi = 0;
void path_element() 
{
	
	//------------------------单线--------------------
	
	
	MidBlack_Deal(Pixels1);
	
	
	//------------------------直角---------------------
	
	
	Zhijiao_Deal();
	

	//------------------------十字--------------------
	
	
	Cross_Deal(Pixels1,Pixels3);
	
	
	//------------------------障碍---------------------
	
	
	Obstacle_Deal(Pixels3);
		

}

/**
*  @name   : errorcalculate
*  @author : ZX
*  @tips   : 
*/
void errorcalculate(void)
{
	
	A  = DirectionP;
	B  = DirectionD;
	
	last_offset = offset; //刷新上一次偏差值
	
	if(mid_black == 1) { //单线
			
		offset = offset2; //单线偏差
		
	}
	else { //非单线
		
		//普通打脚 包括 障碍
		//if(error_field != 0)
			offset = ( (RightBlack + LeftBlack) >> 1) - mid;
		
		//直角
		if(black_area) // 黑带区域
		{
			if(Left_zhijiao)
			{
				offset = 64;
			}
			else if(Right_zhijiao)
			{
				offset = -64;
			}
			else
			{
				
			}
		}
		
		
		//坡道
		if(podao)
		{
			offset = offset / 2;
			////A = DirectionP * 0.3;
			//B = DirectionD * 1.2;
		}
		
		//十字
		if(shizi)
		{
		//	offset = ( (RightBlack2 + LeftBlack2) >> 1) - mid;
		  offset = mid_ave - mid;
			//A = DirectionP * 0.8;
			//B = DirectionD * 1.1;
		}
		
	}
	
	//if(error_field != 0)
		offset_two_order  = offset - last_offset; //打脚D项
	
	
	
	
	
}



uint8_t CCD3_width = 0;

/*
void GetBlackString2(uint8_t *line)
{                                               
	uint8_t i;           
	uint8_t flag1=0,flag3=0;    //flag1左边计数标记，flag3右边计数标记
	uint8_t exl=0,exr=0; //scan width 扫描宽度
	uint8_t obstacle_flag_down = 0,obstacle_flag_up = 0;
  
	//Threshold2 = Auto_Threshold(line); //动态阈值计算 每一场刷新一次
	
	
	Q_LeftBlack2 = LeftBlack2;  //前一次黑线刷新
	Q_RightBlack2 = RightBlack2;
	
	// set scan width
	if(begin2==6)              
		 exr=50;            
	if(begin2==121)
		 exl=50;
	
  //clean the flag
	LxQ3 = 0; //扫线
	RxQ3 = 0;
	LxQ4 = 0; //宽度
	RxQ4 = 0;
	Down_sec_Q = 0;
	
	obstacle_width = 0;
	obstacle_axis = 0;
	CCD3_width = RightBlack2 - LeftBlack2;
	
 //scan left
	for(i = begin2;i > 3 + exl;i--)  
	{
		 if((*(line + i)-*(line + i - 3)) >= Threshold2)  //隔四点做差 利用阈值找到跳变点     
		 { 
				flag1++;                                   
				if(flag1 >= 3)                                          
				{
					 LeftBlack2 = i;
					 LxQ3=1;
					 break;
				}
		 }
		 else
		 {
			flag1 = 0;
		 }	 
	}
	
	//scan right
	for(i = begin2;i < 118 - exr;i++)
	{
		if((*(line + i)-*(line + i + 3)) >= Threshold2)
		{ 
				flag3++;
				if(flag3 >= 3)
				{
					RightBlack2 = i;
					RxQ3=1;
					break;
				}
		}
		else 
		{
			flag3 = 0;
		}
	}
  
	//missing black
  if(flag3 < 3)                          
  {		
     RightBlack2 = 127; // no black
	}
     
  if(flag1 < 3)
  {
		LeftBlack2 = 0; //white
	}
	
  //reset the next begin point	
  begin2 = (LeftBlack2 + RightBlack2) >> 1; 
  
	//参数有待修改 此处可能有BUG
 
	if(begin2 < 30) {   //28                      
    begin2 = 6; 
	}
  if(begin2 > 97) {   //99                           
    begin2 = 121;
	}
	
	//障碍
	for(i = 24; i < 103; i++)
	{
		if((*(line + i)-*(line + i + 3)) >= Threshold2) 
		{ 
			obstacle_flag_down++;                                   
			if(obstacle_flag_down >= 3)                                          
			{
					obstacle_down = i;
					LxQ4 = 1;
					for(i = obstacle_down; i < 100 ; i++) { 
							if((*(line + i + 3)-*(line + i)) >= Threshold2)
							{  
									obstacle_flag_up++ ;                                  
									if(obstacle_flag_up >= 3)                                           
									{
										 obstacle_up = i;
										 obstacle_width = obstacle_up - obstacle_down;
										 obstacle_axis = (obstacle_up + obstacle_down ) >> 1;
										 if(obstacle_down > 64 && obstacle_up > 64)
										 {
											 L_Obstacle = 0;
											 R_Obstacle = 1;
										 }
										 else if(obstacle_down < 64 && obstacle_up < 64)
										 {
											 L_Obstacle = 1;
											 R_Obstacle = 0;
										 }
										 RxQ4 = 1;
										for(i = obstacle_up; i < 97 ; i++)
										{
												if((*(line + i )-*(line + i+3)) >= Threshold2)
												{
													obstacle_flag_down_sec++;
													if(obstacle_flag_down_sec >= 3)
													{
														obstacle_down_sec = i;
														Down_sec_Q = 1;
														break;
													}
												}
												else
												{
													obstacle_flag_down_sec = 0;
												}
										}
										break;
									}
							}
							else {
								 obstacle_flag_up = 0;
							}
					}
					break;
				}
		}
		else{
			obstacle_flag_down = 0;
		}
	}
	
	
	if(obstacle_flag_down_sec < 3)
	{
		obstacle_down_sec = 0;
	}
	if(obstacle_flag_up < 3)
	{
		obstacle_up = 0;
	}
	if(obstacle_flag_down < 3)
	{
		obstacle_down = 0;
	}
	
	if(Obstacle_Switch && mid_black == 0) { //障碍开关
		if(obstacle_width >= 13 && obstacle_width <=30 && abs(offset) <= 15 && CCD3_width <= 55 && CCD3_width >= 10)
		{
			obstacle_flag++;
		}
		else
		{
			obstacle_flag = 0;
		}
		
		if(obstacle_flag >= obstacle_conut)
		{
			obstacle = 1;
		}
		
		if(obstacle)  //防止单线时候判断到障碍
		{
			if(L_Obstacle)
			{
				LeftBlack = 90;
			}
			else
			{
			  RightBlack = 43;
			}
			//Beep();
		}
		else {	
			//NoBeep();
		}
		
		
		if(obstacle==1)
		{
			 S_bar+=CarSpeed;
			 if(S_bar>=S_bar_cnt)
			 {
				 S_bar=0;
				 obstacle = 0;
			 }
		}
	}
	
}
*/

/**
*  @name   : GetBlack
*  @author : ZX
*  @tips   : 宽度扫描
*/
/*

uint8_t mid_Lc = 0;
void GetBlack(uint8_t *line)
{                                               
    uint8_t i;           
    uint8_t flag1 = 0,flag3 = 0,flag2 = 0,flag4 = 0;  
    uint8_t LxX = 0,LxS = 0,RxX = 128,RxS = 128;
    char exl = 0,exr = 0;
		uint8_t mid_flag1 = 0,mid_flag2 = 0;	
		
    LxQ = 0;
	  RxQ = 0;
		LxQ2 = 0;
		RxQ2 = 0;
    
    Lc_last = Lc;
		Rc_last = Rc;
    
		Lc=0;Rc=0;
    
		flag2=0;flag4=0;
    
		Q_LeftBlack = LeftBlack; 
		Q_RightBlack = RightBlack;
    
    if(begin==6)              
       exr=50;            
    if(begin==121)
       exl=50;
    for(i = begin;i > MA+exl;i--)             
    {
        if((*(line + i)-*(line + i - MA)) >= (Threshold))         
        { 
            flag1++;                                   
            if(flag1 >= MA)                                          
            {
                LxX = i;
                for(i=LxX;i>MA+1+exl;i--){
                    if((*(line + i - MA)-*(line + i)) >=Up_Threshold)      
                    {  
                        flag2++;                                  
                        if(flag2 >= MA)                                           
                        {  
                           LxS = i-MA;
                           Lc=LxX-LxS;                        
                           if(Lc>=4&&Lc<=25) {
                              LxQ=1;             
                              LeftBlack = LxS;
                           }
                           break;
                        }
                     }
                     else{
                       flag2=0;
                     }
                }
                if(flag2<MA)                               
                Lc = LxX - MA;
                break;
            }
       }
       else{
        flag1 = 0;
       }
    } 
    for(i = begin;i < 121 - MA-exr;i++)                 
    {
        if((*(line + i)-*(line + i + MA)) >= (Threshold))
        { 
            flag3++;
            if(flag3 >= MA)
            {
                RxX = i;
                for(i = RxX;i<121-MA-exr;i++)             
                {
                     if((*(line + i + MA)-*(line + i)) >=Up_Threshold)
                     { 
                        flag4++;
                        if(flag4 >= MA)
                        {  
                           RxS = i+MA;
                           Rc=RxS-RxX; 
                           if(Rc>=4&&Rc<=25){
                             RxQ=1;
                             RightBlack = RxS;
                            } 
                           break;
                        }
                     }                             
                     else{
                       flag4=0;
                     }
                }
                if(flag4<MA)
                Rc=127-MA-RxX;
                break;
            }
        }
        else {
          flag3=0;
        }
    }
    
   
 
  if(flag4<MA)                           
  {
    if(begin>6)                         
       RightBlack = 127;
    else                                
       RightBlack = 0;
  }
     
  if(flag2<MA)                           
  {                                      
    if(begin <121)                      
       LeftBlack = 0;
    else                               
       LeftBlack = 127;
  }
  
     
  begin = (LeftBlack + RightBlack)>>1;                
  
	if(begin<30)
     begin=6;
  if(begin>97)                           
     begin=121;
	
	//单线
	for(i = 6 ; i < 121; i++)
	{
		if((*(line + i)-*(line + i + 3)) >= Mid_Threshold) 
		{ 
            mid_flag1++;                                   
            if(mid_flag1 >= 3)                                          
            {
                mid_left = i;
								LxQ2 = 1;
                for(i = mid_left; i < 120 ; i++) { 
                    if((*(line + i + 3)-*(line + i)) >= Mid_Threshold)      
                    {  
                        mid_flag2++ ;                                  
                        if(mid_flag2 >= 3)                                           
                        {
                           mid_right = i;
                           mid_Lc = mid_right - mid_left;                        
                           if(mid_Lc >= 4 && mid_Lc <= 10) {           
                              single_mid = (mid_right + mid_left )>> 1;
														  RxQ2 = 1;
                           }
													 else if(mid_Lc > 15 && mid_Lc <= 35) //障碍 出弯黑带
													 {
														 out_black = 1;
													 }
													 else if(Lc > 35)
													 {
														 
													 }
                           break;
                        }
                    }
                    else {
                       mid_flag2 = 0;
                    }
                }
                break;
							}
					}
        else{
					mid_flag1 = 0;
				}
	}
	
	if(Single_Mid_Switch) { //单线开关
		if(LxQ2 !=0 && RxQ2 != 0) //单线
		{
			mid_black = 1;
			offset2 = single_mid  - 64;
			begin = 64;
			LeftBlack = 0;
			RightBlack = 127;
			//NoBeep();
		}
		else
		{
			mid_black = 0;
			//NoBeep();
		}
	}
	
	
	last_offset = offset;
	
	if(mid_black == 1) {
			
		offset = offset2;
		//offset_two_order  = offset - last_offset;
		
	}
	else {
		// calculate offset
		//last_offset = offset;
		offset = ( (RightBlack + LeftBlack) >> 1) - mid;
		//offset_two_order  = offset - last_offset;
	}
	
	offset_two_order  = offset - last_offset;
	
}

*/

