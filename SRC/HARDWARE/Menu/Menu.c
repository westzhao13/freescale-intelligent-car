/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "Menu.h"
/*	菜单选项的个数*/
#define MenuNum 8//一页的行数
#define MenuPage 4//设置页数
#define BaseNum 500

/*				*/

int8_t Select=1;	//由于Select定义成8位的，则Select的最大值为255,即最大只能为30页

int8_t PageNum=1;

uint8_t FirstPage = 1;




/**
* @name: LCD_DisplayCCD_2Column
* @author: ZX
* @tips : display CCD AD Pixels
*/

void LCD_DisplayCCD_2Column(uint8_t X,uint8_t AD1,uint8_t AD2) 
{
     uint8_t x1,x2,y1,y2,row1,row2,DATA,data;
     x1=X;
     x2=X+1;
     y1=63-AD1/4;
     y2=63-AD2/4;
     row1=y1/8;
     row2=y2/8;
     DATA=y1%8;
     OLED_Set_XY(x1,row1);
     switch (DATA)
     {
        case 0: data=0x01;break;
        case 1: data=0x02;break;
        case 2: data=0x04;break;
        case 3: data=0x08;break;
        case 4: data=0x10;break;
        case 5: data=0x20;break;
        case 6: data=0x40;break;
        case 7: data=0x80;break;    
     }
     OLED_WrDat(data);
     if(row2!=row1)
     {
        OLED_Set_XY(x2,row2);
        DATA=y2%8;
     }
     switch ( DATA )
     {
        case 0: data=0x01;break;
        case 1: data=0x02;break;
        case 2: data=0x04;break;
        case 3: data=0x08;break;
        case 4: data=0x10;break;
        case 5: data=0x20;break;
        case 6: data=0x40;break;
        case 7: data=0x80;break;    
     }
     OLED_WrDat(data);
}
/**
* @name: ShowCCDAD
* @author: ZX
* @tips : display CCD AD Pixels
*/
void ShowCCDAD(uint8_t* data) 
{
  uint8_t i;
  OLED_Clear();
  for(i=0;i<127;i++) 
  {
			LCD_DisplayCCD_2Column(i,*(data+i),*(data+i+1));
	}
		
}

uint8_t FirstPageSelect = 1;
uint8_t CCDView = 0;
uint8_t DataChange = 0;
uint8_t CCDflag = 1;
void KeyScan2(void)
{
	uint8_t key;
	uint8_t flag=0;
	int8_t temp;//用于翻页处理
	
	key = GetKey();			//按键扫描
	
	if(FirstPage == 1)
	{
		if(key ==RightUp)															//Select的边界处理,以及PageNum的计算(是翻页的重要部分)
		{
			FirstPageSelect--;
			if(FirstPageSelect<=1)
				FirstPageSelect = 1;
			OLED_Clear();
		}
		if(key == RightDown)
		{
			FirstPageSelect++;
			if(FirstPageSelect>=2)
				FirstPageSelect = 2;
			OLED_Clear();
		}
		if(key == Confirm)
		{
			FirstPage = 0;
			if(FirstPageSelect == 1)
			{
				DataChange = 1;
				CCDView = 0;
			}
			else if(FirstPageSelect == 2)
			{
				CCDView = 1;
				DataChange = 0;
			}
			OLED_Clear();
		}
	}
	else
	{
		if(DataChange == 1)
		{
		if(key ==RightUp)															//Select的边界处理,以及PageNum的计算(是翻页的重要部分)
		{
			Select--;
			if(Select<1)
				Select=MenuNum * MenuPage;
			temp = Select;
			temp = temp & 0x07;
			if(temp == 0)			//根据8*N对应的二进制数判断是否为8的整数倍
			{
				PageNum = Select/MenuNum;
			}
			else
			{
				PageNum = Select/MenuNum + 1;
			}
			OLED_Clear();
		}
		if(key == RightDown)
		{
			Select++;
			if(Select > MenuNum*MenuPage)
				Select = 1;
			temp = Select;
			temp = temp & 0x07;
			if(temp == 0)			//根据8*N对应的二进制数判断是否为8的整数倍
			{
				PageNum = Select/MenuNum;
			}
			else
			{
				PageNum = Select/MenuNum + 1;
			}
			OLED_Clear();
		}
		
		if(PageNum == 1)													//对应页数的内容
		{
			flag = Select;
			switch(flag)
			{
				case 1:
					if(key == LeftUp)
					{EndlongP++;}
					if(key == LeftDown)
					{EndlongP--;}
					if(EndlongP <= 0)
					{EndlongP =0;}
					break;
				case 2:
					if(key == LeftUp)
					{EndlongD++;}
					if(key == LeftDown)
					{EndlongD--;}
					if(EndlongD <= 0)
					{EndlongD = 0;}
					break;
				case 3:
					if(key == LeftUp)
					{CAR_ACCE_RATIO++;}
					if(key == LeftDown)
					{CAR_ACCE_RATIO--;}
					if(CAR_ACCE_RATIO <= 0)
					{CAR_ACCE_RATIO = 0;}
					break;
				case 4:
					if(key == LeftUp)
					{CAR_GYRO_RATIO++;}
					if(key == LeftDown)
					{CAR_GYRO_RATIO--;}
					if(CAR_GYRO_RATIO <= 0)
					{CAR_GYRO_RATIO = 0;}
					break;
				case 5:
					if(key == LeftUp)
					{Angle_Set++;}
					if(key == LeftDown)
					{Angle_Set--;}
					if(Angle_Set<=0)
					{Angle_Set = 0;}
			break;
				case 6:
					if(key == LeftUp)
						 {black_threshold+=100;}
					 if(key == LeftDown)
						 {black_threshold-=100;}
					 if(black_threshold<=0)
						 {black_threshold = 0;}
			break;
				case 7:		 
					if(key == LeftUp)
						{podao_threshold+=100;}
					if(key == LeftDown)
						{podao_threshold-=100;}
					if(	podao_threshold<=0)
						{podao_threshold=0;}
			break;
				case 8:																			//页选项的处理，下面的case 8: 是同样的原理
					if(key == LeftUp)
					{	
						PageNum++;
						Select = Select + MenuNum;
						OLED_Clear();
					}
					if(key == LeftDown)
					{
						PageNum--;
						Select = Select - MenuNum;
						OLED_Clear();
					}
					if(PageNum <= 0)
					{
						PageNum = MenuPage;
						Select = MenuNum * PageNum;
					}
					if(PageNum > MenuPage)
					{
						PageNum = 1;
						Select = MenuNum * PageNum;
					}
					break;
				default:break;
			}
		}
		else if(PageNum ==2)
		{
			flag = Select - MenuNum*(PageNum - 1);
			switch(flag)
			{
				case 1:
					 if(key == LeftUp)
						 {AmSpeed+=5;}
					 if(key == LeftDown)
						 {AmSpeed-=5;}
					 if(AmSpeed<=0)
						 {AmSpeed = 0;}	
				break;
				case 2:break;
				case 3:
					 if(key == LeftUp)
						 {Up_Threshold++;}
					 if(key == LeftDown)
						 {Up_Threshold--;}
					 if(Up_Threshold<=0)
						 {Up_Threshold = 0;}	
				break;
				case 4:
					if(key == LeftUp)
						{Speed_P++;}
					if(key == LeftDown)
						{Speed_P--;}
					if(Speed_P<=0)
						{Speed_P= 0;}		
				break;
				case 5:
					if(key == LeftUp)
						{Speed_I++;}
					if(key == LeftDown)
						{Speed_I--;}
					if(Speed_I<=0)
						{Speed_I= 0;}	
				break;
				case 6:
					if(key == LeftUp)
						{Speed_D++;}
					if(key == LeftDown)
						{Speed_D--;}
					if(Speed_D<=0)
						{Speed_D= 0;}	
				break;
				case 7:
					if(key == LeftUp)
						{S_bar_cnt+=100;}
					if(key == LeftDown)
						{S_bar_cnt-=100;}
					if(S_bar_cnt<=0)
						{S_bar_cnt = 0;}			
				break;
				case 8:
					if(key == LeftUp)
					{
						PageNum++;
						Select = Select + MenuNum;
						OLED_Clear();
					}
					if(key == LeftDown)
					{
						PageNum--;
						Select = Select - MenuNum;	
						OLED_Clear();
					}
					if(PageNum <= 0)
					{
						PageNum = MenuPage;
						Select = MenuNum * PageNum;
					}
					if(PageNum > MenuPage)
					{
						PageNum = 1;
						Select = MenuNum * PageNum;
					}
					break;
				default:break;
			}
		}
		else if(PageNum == 3)
		{
			flag = Select - MenuNum*(PageNum - 1);
			switch(flag)
			{
				case 1:
			if(key == LeftUp)
			{DirectionP++;}
			if(key == LeftDown)
			{DirectionP--;}
			if(DirectionP<=0)
				{DirectionP = 0;}		
				break;
				case 2:
				if(key == LeftUp)
				{DirectionD++;}
				if(key == LeftDown)
				{DirectionD--;}
				if(DirectionD<=0)
				{DirectionD = 0;}		
				break;
				case 3:break;
				case 4:break;
				case 5:
					if(key == LeftUp)
					{zhijiao_count++;}
					if(key == LeftDown)
					{zhijiao_count--;}
					if(zhijiao_count<=0)
					{zhijiao_count = 0;}		
					break;
				case 6:break;
				case 7:break;
				case 8:
					if(key == LeftUp)
					{
						PageNum++;
						Select = Select + MenuNum;
						OLED_Clear();
					}
					if(key == LeftDown)
					{
						PageNum--;
						Select = Select - MenuNum;	
						OLED_Clear();
					}
					if(PageNum <= 0)
					{
						PageNum = MenuPage;
						Select = MenuNum * PageNum;
					}
					if(PageNum > MenuPage)
					{
						PageNum = 1;
						Select = MenuNum * PageNum;
					}
					break;
				default:break;
			}
		}
		else if(PageNum ==4)
		{
			flag = Select - MenuNum*(PageNum - 1);
			switch(flag)
			{
				case 1:
					 
				break;
				case 2:
					if(key == LeftUp)
						 {LeftMotorDead++;}
					if(key == LeftDown)
						 {LeftMotorDead--;}
					if(LeftMotorDead<=0)
						 {LeftMotorDead = 0;}	
				break;
				case 3:
					if(key == LeftUp)
						 {RightMotorDead++;}
					if(key == LeftDown)
						 {RightMotorDead--;}
					if(RightMotorDead<=0)
						 {RightMotorDead = 0;}	
				break;
				case 4:
					if(key == LeftUp)
						 {Threshold++;}
					if(key == LeftDown)
						 {Threshold--;}
					if(Threshold<=0)
						 {Threshold = 0;}	
				break;
				case 5:
					if(key==LeftUp)
					{DirectionDL++;}
					if(key==LeftDown)
					{DirectionDL--;}
					if(DirectionDL<=0)
					{DirectionDL=0;}					
				break;
				case 6:
					if(key==LeftUp)
					{Mid_Threshold++;}
					if(key==LeftDown)
					{Mid_Threshold--;}
					if(Mid_Threshold<=0)
					{Mid_Threshold=0;}
					break;
				case 7:
					if(key==LeftUp)
					{Threshold2++;}
					if(key==LeftDown)
					{Threshold2--;}
					if(Threshold2<=0)
					{Threshold2=0;}
					break;
				case 8:
					if(key == LeftUp)
					{
						PageNum++;
						Select = Select + MenuNum;
						OLED_Clear();
					}
					if(key == LeftDown)
					{
						PageNum--;
						Select = Select - MenuNum;	
						OLED_Clear();
					}
					if(PageNum <= 0)
					{
						PageNum = MenuPage;
						Select = MenuNum * PageNum;
					}
					if(PageNum > MenuPage)
					{
						PageNum = 1;
						Select = MenuNum * PageNum;
					}
					break;
				default:break;
				}
			}
			if(key == Confirm)
			{
				FirstPage = 1;
				DataChange = 0;
				CCDView = 0;
				OLED_Clear();
				DFLASH_EraseSector_BLOCK1();
				Data_Write(); //写入Flash
			}
		}
		if(CCDView == 1)
		{
			
			if(key == LeftUp)	
			{
					CCDflag++;
					if(CCDflag>=3)
					{
						CCDflag = 3;
					}
			}
			
			if(key == LeftDown)
			{
				CCDflag--;
				if(CCDflag<=1)
				{
					CCDflag = 1;
				}
			}
			
			if(key == Confirm)
			{
				FirstPage = 1;
				DataChange = 0;
				CCDView = 0;
				OLED_Clear();
			}
		}
	}
}

/**
* @name: KeyScan
* @author: ZX
* @tips :
*/
void menu(void)
{
	int flag=0;
	flag = Select - (PageNum-1)*MenuNum;
	
	if(FirstPage == 1)
	{
		OLED_Write_String(2,0,(uint8_t*)"DataChange");
		OLED_Write_String(2,1,(uint8_t*)"CCDView");
		
		if(FirstPageSelect == 1)      OLED_Write_Char(0,0,43);
		else if(FirstPageSelect == 2) OLED_Write_Char(0,1,43);
	}
	else
	{
		if(DataChange == 1)
		{
			if(PageNum == 1)
			{
				OLED_Write_String(2,0,(uint8_t*)"endlongP");
				OLED_Write_Num5(12,0,EndlongP);
				OLED_Write_String(2,1,(uint8_t*)"endlongD");
				OLED_Write_Num5(12,1,EndlongD);
				OLED_Write_String(2,2,(uint8_t*)"CAR_ACCE_RATIO");
				OLED_Write_Num5(12,2,CAR_ACCE_RATIO);
				OLED_Write_String(2,3,(uint8_t*)"CAR_GYRO_RATIO");
				OLED_Write_Num5(12,3,CAR_GYRO_RATIO);
				OLED_Write_String(2,4,(uint8_t*)"AngSet");
				OLED_Write_Num4(10,4,Angle_Set);
				OLED_Write_Num3(16,4,(int16_t)Angle);
				OLED_Write_String(2,5,(uint8_t*)"blackthreshold");
				OLED_Write_Num5(12,5,black_threshold);
				OLED_Write_String(2,6,(uint8_t*)"podaothreshold");
				OLED_Write_Num4(12,6,podao_threshold);
				OLED_Write_String(2,7,(uint8_t*)"Page");
				OLED_Write_Num3(12,7,PageNum);
			}
			OLED_Write_Num4(15,7,CarSpeed);
			if(PageNum == 2)
			{
				OLED_Write_String(2,0,(uint8_t*)"AmSpeed");
				OLED_Write_Num5(12,0,AmSpeed);
				OLED_Write_String(2,1,(uint8_t*)"infrared_value");
				OLED_Write_Num5(12,1,infrared_value);
				OLED_Write_String(2,2,(uint8_t*)"podao_value");
				OLED_Write_Num5(12,2,podao_value);
				OLED_Write_String(2,3,(uint8_t*)"SpeedP");
				OLED_Write_Num5(12,3,Speed_P);
				OLED_Write_String(2,4,(uint8_t*)"SpeedI");
				OLED_Write_Num5(12,4,Speed_I);
				OLED_Write_String(2,5,(uint8_t*)"obstacle_axis");
				OLED_Write_Num5(12,5,obstacle_axis);
				OLED_Write_String(2,6,(uint8_t*)"S_bar_cnt");
				OLED_Write_Num5(12,6,S_bar_cnt);
				OLED_Write_String(2,7,(uint8_t*)"Page");
				OLED_Write_Num3(12,7,PageNum);
			}
			if(PageNum ==3)
			{
				OLED_Write_String(2,0,(uint8_t*)"TurnP");
				OLED_Write_Num5(12,0,DirectionP);
				OLED_Write_String(2,1,(uint8_t*)"TurnD");
				OLED_Write_Num5(12,1,DirectionD);
				OLED_Write_String(2,2,(uint8_t*)"Left");
				OLED_Write_Num5(12,2,LeftBlack2);
				OLED_Write_String(2,3,(uint8_t*)"Right");
				OLED_Write_Num5(12,3,RightBlack2);
				OLED_Write_String(2,4,(uint8_t*)"zhijiao_count");
				OLED_Write_Num5(12,4,zhijiao_count);
				OLED_Write_String(2,5,(uint8_t*)"offset");
				if(offset<0) {
					OLED_Write_Char(11,5,38);
					OLED_Write_Num5(12,5,abs(offset));
				}
				else
				{
					OLED_Write_Char(11,5,36);
					OLED_Write_Num5(12,5,offset);
				}
				OLED_Write_String(2,6,(uint8_t*)"offset2");
				if(offset2<0) {
					OLED_Write_Char(11,6,38);
					OLED_Write_Num5(12,6,abs(offset2));
				}
				else
				{
					OLED_Write_Char(11,6,36);
					OLED_Write_Num5(12,6,offset2);
				}
				OLED_Write_String(2,7,(uint8_t*)"AD_Average");
				OLED_Write_Num5(12,7,AD_Average);
			}
			if(PageNum == 4)
			{
				OLED_Write_String(2,0,(uint8_t*)"Lc");
				OLED_Write_Num5(12,0,Lc);
				OLED_Write_String(2,1,(uint8_t*)"LeftMotor");
				OLED_Write_Num5(12,1,LeftMotorDead);
				OLED_Write_String(2,2,(uint8_t*)"RightMotor");
				OLED_Write_Num5(12,2,RightMotorDead);
				OLED_Write_String(2,3,(uint8_t*)"Threshold");
				OLED_Write_Num5(12,3,Threshold);
				OLED_Write_String(2,4,(uint8_t*)"obstacle_width");
				OLED_Write_Num5(12,4,obstacle_width);
				OLED_Write_String(2,5,(uint8_t*)"Mid_Threshold");
				OLED_Write_Num5(12,5,Mid_Threshold);
				OLED_Write_String(2,6,(uint8_t*)"Threshold2");
				OLED_Write_Num3(12,6,Threshold2);
				OLED_Write_String(2,7,(uint8_t*)"Page");
				OLED_Write_Num3(12,7,PageNum);
			}

			if(flag==1)      OLED_Write_Char(0,0,43);
			else if(flag==2) OLED_Write_Char(0,1,43);
			else if(flag==3) OLED_Write_Char(0,2,43);
			else if(flag==4) OLED_Write_Char(0,3,43);
			else if(flag==5) OLED_Write_Char(0,4,43);
			else if(flag==6) OLED_Write_Char(0,5,43);
			else if(flag==7) OLED_Write_Char(0,6,43);
			else if(flag==8) OLED_Write_Char(0,7,43);
		}
		if(CCDView == 1)
		{
			if(CCDflag == 1)
			{
				ShowCCDAD(Pixels1);
			}
			if(CCDflag == 2)
			{
				ShowCCDAD(Pixels3);
			}
			if(CCDflag == 3)
			{
				ShowCCDAD(ImageSort);
			}
		}
	}
}

uint8_t ModeSelect = 1;
uint8_t DebugSelect = 1;
uint8_t Debug = 0;
uint8_t DataSelect = 1;
uint8_t SwitchSelect = 1;
uint8_t StartSwitch = 1;
uint8_t CmtSelect = 1;

void MenuDisplay(void)
{
	if(FirstPage)
	{
		OLED_Write_String(2,0,(uint8_t*)"Debug");
		
		OLED_Write_String(2,2,(uint8_t*)"Switch");
		
		OLED_Write_String(2,4,(uint8_t*)"Comptition");
		/*
		OLED_Write_String(2,2,(uint8_t*)"DangWei");
		OLED_Write_Char(2,8,1);
		OLED_Write_String(2,4,(uint8_t*)"DangWei");
		OLED_Write_Char(2,8,2);
		OLED_Write_String(2,6,(uint8_t*)"DangWei");
		OLED_Write_Char(2,8,3);
		*/
		
		//OLED_Write_Num4(0,1,LeftSpeedNow);
		//OLED_Write_Num4(6,1,RightSpeedNow);
		
		
		if(Black1_in == 1) OLED_Write_Char(4,6,1);
		if(Black1_in == 0) OLED_Write_Char(4,6,0);
		
		
		if(Black2_in == 1) OLED_Write_Char(8,6,1);
		if(Black2_in == 0) OLED_Write_Char(8,6,0);
		
		if(ModeSelect == 1)      OLED_Write_Char(0,0,43);
		else if(ModeSelect == 2) OLED_Write_Char(0,2,43);
		else if(ModeSelect == 3) OLED_Write_Char(0,4,43);
	}
	else
	{
		if(ModeSelect == 1)
		{
			if(Debug == 0)
			{
					OLED_Write_String(2,2,(uint8_t*)"DataChange");
					OLED_Write_String(2,4,(uint8_t*)"CCDView");
					OLED_Write_String(2,6,(uint8_t*)"Back");
				
				if(DebugSelect == 1)      OLED_Write_Char(0,2,43);
				else if(DebugSelect == 2) OLED_Write_Char(0,4,43);
				else if(DebugSelect == 3) OLED_Write_Char(0,6,43);
			}
			else
			{
				if(DebugSelect == 1)
				{
					if(DataSelect == 1)
					{
						OLED_Write_String(0,0,(uint8_t*)"Angle");
						OLED_Write_Num4(6,0,Angle);
						OLED_Write_String(4,2,(uint8_t*)"AngleSet");
						OLED_Write_Num4(6,4,Angle_Set);
					}
					else if(DataSelect == 2)
					{
						OLED_Write_String(4,2,(uint8_t*)"AmSpeed");
						OLED_Write_Num4(6,4,AmSpeed);
					}
					else if(DataSelect == 3)
					{
						OLED_Write_String(4,2,(uint8_t*)"SpeedP");
						OLED_Write_Num3(6,4,Speed_P);
					}
					else if(DataSelect == 4)
					{
						OLED_Write_String(4,2,(uint8_t*)"SpeedI");
						OLED_Write_Num3(6,4,Speed_I);
					}
					else if(DataSelect == 5)
					{
						OLED_Write_String(4,2,(uint8_t*)"DirectionP");
						OLED_Write_Num3(6,4,DirectionP);
					}
					else if(DataSelect == 6)
					{
						OLED_Write_String(4,2,(uint8_t*)"DirectionD");
						OLED_Write_Num3(6,4,DirectionD);
					}
					else if(DataSelect == 7)
					{
						OLED_Write_String(0,2,(uint8_t*)"LeftMotorDead");
						OLED_Write_Num3(6,4,LeftMotorDead);
					}
					else if(DataSelect == 8)
					{
						OLED_Write_String(0,2,(uint8_t*)"RightMotorDead");
						OLED_Write_Num3(6,4,RightMotorDead);
					}
					else if(DataSelect == 9)
					{
						OLED_Write_String(4,2,(uint8_t*)"Threshold");
						OLED_Write_Num3(6,4,Threshold);
					}
					else if(DataSelect == 10)
					{
						OLED_Write_String(4,2,(uint8_t*)"Threshold2");
						OLED_Write_Num3(6,4,Threshold2);
					}
					else if(DataSelect == 11)
					{
						OLED_Write_String(0,2,(uint8_t*)"White_value");
						OLED_Write_Num5(6,4,White_value);
					}
					else if(DataSelect == 12)
					{
						OLED_Write_String(0,2,(uint8_t*)"podaothreshold");
						OLED_Write_Num5(6,4,podao_threshold);
					}
					else if(DataSelect == 13)
					{
						OLED_Write_String(4,2,(uint8_t*)"Sbarcnt");
						OLED_Write_Num5(6,4,S_bar_cnt);
					}
					else if(DataSelect == 14)
					{
						OLED_Write_String(3,2,(uint8_t*)"Stop_Flag");
						OLED_Write_Num4(6,4,Stop_Flag);
					}
					else if(DataSelect == 15)
					{
						OLED_Write_String(3,2,(uint8_t*)"obstacnt");
						OLED_Write_Num4(6,4,obstacle_conut);
					}
					else if(DataSelect == 16)
					{
						OLED_Write_String(3,2,(uint8_t*)"MidThreshold");
						OLED_Write_Num4(6,4,Mid_Threshold);
					}
					else if(DataSelect == 17)
					{
						OLED_Write_String(3,2,(uint8_t*)"zhijiaobarcnt");
						OLED_Write_Num5(6,4,zhijiao_s_bar_cnt);
					}
					else if(DataSelect == 18)
					{
						OLED_Write_String(3,2,(uint8_t*)"podao_last_time");
						OLED_Write_Num4(6,4,podao_last_time);
					}
					else if(DataSelect == 19)
					{
						OLED_Write_String(0,0,(uint8_t*)"GYROY");
						OLED_Write_Num4(6,0,GYRO_Y_Fix);
						OLED_Write_String(3,2,(uint8_t*)"StopTime");
						OLED_Write_Num5(6,4,StopTime);
					}
				}
				else
				{
					
					if(CCDflag == 1)
					{
						ShowCCDAD(Pixels1);
					}
					if(CCDflag == 2)
					{
						ShowCCDAD(Pixels3);
						
					}
					if(CCDflag == 3)
					{
						OLED_Write_String(0,0,(uint8_t*)"ccdw");
						OLED_Write_Num3(3,0,CCD3_width);
						OLED_Write_String(7,0,(uint8_t*)"Rx");
						OLED_Write_Num3(10,0,abs(offset));
						
						OLED_Write_String(0,2,(uint8_t*)"obw");
						OLED_Write_Char(2,2,2);
						OLED_Write_Num3(4,2,obstacle_width);
						OLED_Write_String(8,2,(uint8_t*)"Lx2");
						OLED_Write_Char(10,2,2);
						OLED_Write_Num3(12,2,LeftBlack2);
						
						OLED_Write_String(0,4,(uint8_t*)"Rx2");
						OLED_Write_Num5(6,4,RightBlack2);
						OLED_Write_String(0,6,(uint8_t*)"podao");
						OLED_Write_Num4(6,6,podao_value);
						
					}
					if(CCDflag == 4)
					{
						OLED_Write_String(0,0,(uint8_t*)"offset");
						if(offset<0) {
							OLED_Write_Char(8,0,38);
							OLED_Write_Num3(9,0,abs(offset));
						}
						else
						{
							OLED_Write_Char(8,0,36);
							OLED_Write_Num3(9,0,offset);
						}
						/*
						OLED_Write_String(0,2,(uint8_t*)"offset2");
						if(offset2<0) {
							OLED_Write_Char(8,2,38);
							OLED_Write_Num3(9,2,abs(offset2));
						}
						else
						{
							OLED_Write_Char(8,2,36);
							OLED_Write_Num3(9,2,offset2);
						}
						*/
						OLED_Write_String(0,2,(uint8_t*)"obdsec");
						OLED_Write_Num3(7,2,obstacle_down_sec);
						
						OLED_Write_String(10,4,(uint8_t*)"dq");
						OLED_Write_Num3(12,4,Down_sec_Q);
						
						OLED_Write_String(0,4,(uint8_t*)"obwidth");
						OLED_Write_Num3(8,4,obstacle_width);
						
						OLED_Write_String(0,6,(uint8_t*)"Threshold");
						OLED_Write_Num3(10,6,Threshold);
						
					}
					
				}
				
				
				
			}
		}
		else if(ModeSelect == 2)
		{
				OLED_Write_String(1,0,(uint8_t*)"ObS");
				OLED_Write_Num3(5,0,Obstacle_Switch);
				OLED_Write_String(9,0,(uint8_t*)"Aut");
				OLED_Write_Num3(12,0,Auto);
				
				OLED_Write_String(1,2,(uint8_t*)"Mid");
				OLED_Write_Num3(5,2,Single_Mid_Switch);
				OLED_Write_String(9,2,(uint8_t*)"CMD");
				OLED_Write_Num3(12,2,CCD_Mode);
				
				OLED_Write_String(1,4,(uint8_t*)"Zhij");
				OLED_Write_Num3(5,4,Zhijiao_Switch);
				OLED_Write_String(9,4,(uint8_t*)"Pdao");
				OLED_Write_Num3(13,4,Podao_Switch);
	
				OLED_Write_String(1,6,(uint8_t*)"StartS");
				OLED_Write_Num3(7,6,StartSwitch);
				OLED_Write_String(11,6,(uint8_t*)"T");
				OLED_Write_Num4(12,6,StopTime);
			
				if(SwitchSelect == 1)      OLED_Write_Char(0,0,43);
				else if(SwitchSelect == 2) OLED_Write_Char(0,2,43);
				else if(SwitchSelect == 3) OLED_Write_Char(0,4,43);
				else if(SwitchSelect == 4) OLED_Write_Char(8,4,43);
				else if(SwitchSelect == 5) OLED_Write_Char(0,6,43);
				else if(SwitchSelect == 6) OLED_Write_Char(10,6,43);
				else if(SwitchSelect == 7) OLED_Write_Char(8,0,43);
				else if(SwitchSelect == 8) OLED_Write_Char(8,2,43);
		}
		else if(ModeSelect == 3)
		{
				OLED_Write_String(1,0,(uint8_t*)"Ag");
				OLED_Write_Num3(3,0,Angle);
				OLED_Write_String(7,0,(uint8_t*)"AmSd");
				OLED_Write_Num4(11,0,AmSpeed);
				
				OLED_Write_String(1,2,(uint8_t*)"Dp");
				OLED_Write_Num3(5,2,DirectionP);
				OLED_Write_String(9,2,(uint8_t*)"DD");
				OLED_Write_Num3(12,2,DirectionD);
				
				OLED_Write_String(1,4,(uint8_t*)"Th");
				OLED_Write_Num3(5,4,Threshold);
				OLED_Write_String(9,4,(uint8_t*)"Th2");
				OLED_Write_Num3(13,4,Threshold2);
	
				OLED_Write_String(1,6,(uint8_t*)"MidT");
				OLED_Write_Num3(5,6,Mid_Threshold);
				OLED_Write_String(9,6,(uint8_t*)"S");
				OLED_Write_Num5(10,6,StopTime);
			
				if(CmtSelect == 1)      OLED_Write_Char(0,0,43);
				else if(CmtSelect == 3) OLED_Write_Char(0,2,43);
				else if(CmtSelect == 5) OLED_Write_Char(0,4,43);
				else if(CmtSelect == 6) OLED_Write_Char(8,4,43);
				else if(CmtSelect == 7) OLED_Write_Char(0,6,43);
				else if(CmtSelect == 8) OLED_Write_Char(8,6,43);
				else if(CmtSelect == 2) OLED_Write_Char(6,0,43);
				else if(CmtSelect == 4) OLED_Write_Char(8,2,43);
			
		}
		
		
	}
}

void KeyScan(void)
{
	uint8_t key;
	
	key = GetKey();			//按键扫描
	
	if(FirstPage) //首页
	{
		if(key == RightUp)
		{
			ModeSelect --;
			if(ModeSelect <= 1)
			{
				ModeSelect = 1;
			}
			OLED_Clear();
		}
		if(key == RightDown)
		{
			ModeSelect++;
			if(ModeSelect >= 3)
			{
				ModeSelect = 3;
			}
			OLED_Clear();
		}
		if(key == Confirm)
		{
			FirstPage = 0;
			OLED_Clear();
		}
	}
	else //
	{
		if(ModeSelect == 1) //Debug
		{
			
			
			if(Debug == 0) { //Debug
				if(key == RightUp)
				{
					DebugSelect --;
					if(DebugSelect <= 1)
					{
						DebugSelect = 1;
					}
					OLED_Clear();
				}
				if(key == RightDown)
				{
					DebugSelect ++;
					if(DebugSelect >= 3)
					{
						DebugSelect = 3;
					}
					OLED_Clear();
				}
				if(DebugSelect != 3) {
					if(key == Confirm)
					{
						Debug = 1;
						OLED_Clear();
					}
				}
				else
				{
					if(key == Confirm)
					{
						FirstPage = 1;
						OLED_Clear();
					}
				}
			}
			else
			{
				if(DebugSelect == 1) //DataChange
				{
					if(key == RightUp)
					{
						DataSelect --;
						if(DataSelect <= 1)
						{
							DataSelect = 1;
						}
						OLED_Clear();
					}
					if(key == RightDown)
					{
						DataSelect++;
						if(DataSelect >= 19)
						{
							DataSelect = 19;
						}
						OLED_Clear();
					}
					
					if(DataSelect == 1) //AngelSet
					{
						if(key == LeftUp)
						{
							Angle_Set++;
						}
						if(key == LeftDown)
						{
							Angle_Set--;
							if(Angle_Set<= 0)
							{
								Angle_Set = 0;
							}
						}
					}
					else if(DataSelect == 2) //AmSpeed
					{
						if(key == LeftUp)
						{
							AmSpeed += 10;
						}
						if(key == LeftDown)
						{
							AmSpeed -= 10;
							if(AmSpeed<= 0)
							{
								AmSpeed = 0;
							}
						}
					}
					else if(DataSelect == 3) //SpeedP
					{
						if(key == LeftUp)
						{
							Speed_P ++;
						}
						if(key == LeftDown)
						{
							Speed_P--;
							if(Speed_P<= 0)
							{
								Speed_P = 0;
							}
						}
					}
					else if(DataSelect == 4) //SpeedI
					{
						if(key == LeftUp)
						{
							Speed_I ++;
						}
						if(key == LeftDown)
						{
							Speed_I--;
							if(Speed_I<= 0)
							{
								Speed_I = 0;
							}
						}
					}
					else if(DataSelect == 5) //TurnP
					{
						if(key == LeftUp)
						{
							DirectionP += 10;
						}
						if(key == LeftDown)
						{
							DirectionP -= 10;
							if(DirectionP<= 0)
							{
								DirectionP = 0;
							}
						}
					}
					else if(DataSelect == 6) //TurnD
					{
						if(key == LeftUp)
						{
							DirectionD +=10;
						}
						if(key == LeftDown)
						{
							DirectionD-=10;
							if(DirectionD<= 0)
							{
								DirectionD = 0;
							}
						}
					}
					else if(DataSelect == 7) //LeftMotor
					{
						
						if(key == LeftUp)
						{
							LeftMotorDead += 10;
						}
						if(key == LeftDown)
						{
							LeftMotorDead-=10;
							if(LeftMotorDead<= 0)
							{
								LeftMotorDead = 0;
							}
						}
					}
					else if(DataSelect == 8) //RightMotor
					{
						
						if(key == LeftUp)
						{
							RightMotorDead +=10;
						}
						if(key == LeftDown)
						{
							RightMotorDead-= 10;
							if(RightMotorDead<= 0)
							{
								RightMotorDead = 0;
							}
						}
					}
					else if(DataSelect == 9) //Threshold
					{
						if(key == LeftUp)
						{
							Threshold ++;
						}
						if(key == LeftDown)
						{
							Threshold --;
							if(Threshold<= 0)
							{
								Threshold = 0;
							}
						}
					}
					else if(DataSelect == 10) //Threshold2
					{
						if(key == LeftUp)
						{
							Threshold2 ++;
						}
						if(key == LeftDown)
						{
							Threshold2 --;
							if(Threshold2 <= 0)
							{
								Threshold2 = 0;
							}
						}
					}
					else if(DataSelect == 11) //black_threshold
					{
						if(key == LeftUp)
						{
							White_value += 10;
						}
						if(key == LeftDown)
						{
							White_value -= 10;
							if(White_value <= 0)
							{
								White_value = 0;
							}
						}
					}
					else if(DataSelect == 12) //podao_threshold
					{
						if(key == LeftUp)
						{
							podao_threshold += 50;
						}
						if(key == LeftDown)
						{
							podao_threshold -= 50;
							if(podao_threshold <= 0)
							{
								podao_threshold = 0;
							}
						}
					}
					else if(DataSelect == 13) //S_bar_cnt
					{
						if(key == LeftUp)
						{
							S_bar_cnt += 100;
						}
						if(key == LeftDown)
						{
							S_bar_cnt -= 100;
							if(S_bar_cnt <= 0)
							{
								S_bar_cnt = 0;
							}
						}
					}
					else if(DataSelect == 14)  //Stop_Flag
					{
						if(key == LeftUp)
						{
							Stop_Flag  = 1;
						}
						if(key == LeftDown)
						{
							Stop_Flag  = 0;
						}
					}
					else if(DataSelect == 15)  //obstacle_conut
					{
						if(key == LeftUp)
						{
							obstacle_conut ++ ;
						}
						if(key == LeftDown)
						{
							obstacle_conut --;
							if(obstacle_conut <= 0)
							{
								obstacle_conut = 0;
							}
						}
					}
					else if(DataSelect == 16)  //Mid_Threshold
					{
						if(key == LeftUp)
						{
							Mid_Threshold ++ ;
						}
						if(key == LeftDown)
						{
							Mid_Threshold --;
							if(Mid_Threshold <= 0)
							{
								Mid_Threshold = 0;
							}
						}
					}
					
					else if(DataSelect == 17)  //zhijiao_s_bar_cnt
					{
						if(key == LeftUp)
						{
							zhijiao_s_bar_cnt += 50 ;
						}
						if(key == LeftDown)
						{
							zhijiao_s_bar_cnt -= 50;
							if(zhijiao_s_bar_cnt <= 0)
							{
								zhijiao_s_bar_cnt = 0;
							}
						}
					}
					
					else if(DataSelect == 18)  //podao_last_time
					{
						if(key == LeftUp)
						{
							podao_last_time +=100;
						}
						if(key == LeftDown)
						{
							podao_last_time -=100;
							if(podao_last_time <= 0)
							{
								podao_last_time = 0;
							}
						}
					}
					
					else if(DataSelect == 19)  //StopTime
					{
						if(key == LeftUp)
						{
							StopTime += 100 ;
						}
						if(key == LeftDown)
						{
							StopTime -= 100;
							if(StopTime <= 0)
							{
								StopTime = 0;
							}
						}
					}
					
					
					if(key == Confirm)
					{
						Debug = 0;
						OLED_Clear();
						DFLASH_EraseSector_BLOCK1();
						Data_Write(); //写入Flash
					}
					
				}
				else if(DebugSelect == 2) //ShowCCD
				{
					if(key == RightDown)	
					{
							CCDflag++;
							if(CCDflag>=4)
							{
								CCDflag = 4;
							}
							OLED_Clear();
					}
					
					if(key == RightUp)
					{
						CCDflag--;
						if(CCDflag<=1)
						{
							CCDflag = 1;
						}
						OLED_Clear();
					}
					
					if(key == Confirm)
					{
						Debug = 0;
						OLED_Clear();
					}
				}
				
			}
			
		}
		else if(ModeSelect == 2) //Switch
		{
			if(key == RightUp)
			{
				SwitchSelect --;
				if(SwitchSelect <= 1)
				{
					SwitchSelect = 1;
				}
				OLED_Clear();
			}
			if(key == RightDown)
			{
				SwitchSelect ++;
				if(SwitchSelect>=8)
				{
					SwitchSelect = 8;
				}
				OLED_Clear();
			}
			
			if(key == Confirm)
			{
				FirstPage = 1;
				DFLASH_EraseSector_BLOCK1();
				Data_Write(); //写入Flash
				OLED_Clear();
			}
			
			if(SwitchSelect == 1)
			{
				if(key == LeftUp)
				{
					Obstacle_Switch = 1;
				}
				if(key == LeftDown)
				{
					Obstacle_Switch = 0;
				}
			}
			else if(SwitchSelect == 2)
			{
				if(key == LeftUp)
				{
					Single_Mid_Switch = 1;
				}
				if(key == LeftDown)
				{
					Single_Mid_Switch = 0;
				}
			}
			else if(SwitchSelect == 3)
			{
				if(key == LeftUp)
				{
					Zhijiao_Switch = 1;
				}
				if(key == LeftDown)
				{
					Zhijiao_Switch = 0;
				}
			}
			else if(SwitchSelect == 4)
			{
				if(key == LeftUp)
				{
					Podao_Switch = 1;
				}
				if(key == LeftDown)
				{
					Podao_Switch = 0;
				}
			}
			else if(SwitchSelect == 5)
			{
				if(key == LeftUp)
				{
					StartSwitch ++ ;
					if(StartSwitch >= 3)
					{
						StartSwitch  =3 ;
					}
				}
				if(key == LeftDown)
				{
					StartSwitch --;
					if(StartSwitch<=1)
					{
						StartSwitch = 1;
					}
				}
			}
			
			else if(SwitchSelect == 6)
			{
				if(key == LeftUp)
				{
					StopTime += 500 ;
				}
				if(key == LeftDown)
				{
					StopTime -= 500;
					if(StartSwitch <= 0)
					{
						StartSwitch = 0;
					}
				}
			}
			
			else if(SwitchSelect == 7)
			{
				if(key == LeftUp)
				{
					Auto  = 1 ;
				}
				if(key == LeftDown)
				{
					Auto = 0;
				}
			}
			
			else if(SwitchSelect == 8)
			{
				if(key == LeftUp)
				{
					CCD_Mode  = 1 ;
				}
				if(key == LeftDown)
				{
					CCD_Mode = 0;
				}
			}
			
			
		}
		else if(ModeSelect == 3) //Comptition
		{
			
			if(key == RightUp)
			{
				CmtSelect --;
				if(CmtSelect <= 1)
				{
					CmtSelect = 1;
				}
				OLED_Clear();
			}
			if(key == RightDown)
			{
				CmtSelect ++;
				if(CmtSelect>=8)
				{
					CmtSelect = 8;
				}
				OLED_Clear();
			}
			
			if(key == Confirm)
			{
				FirstPage = 1;
				DFLASH_EraseSector_BLOCK1();
				Data_Write(); //写入Flash
				OLED_Clear();
			}
			
			if(CmtSelect == 1) //Angle
			{
				if(key == LeftUp)
				{
					Angle_Set ++;
				}
				if(key == LeftDown)
				{
					Angle_Set --;
					if(Angle_Set <= 0)
					{
						Angle_Set = 0;
					}
				}
			}
			else if(CmtSelect == 2) //AmSpeed
			{
				if(key == LeftUp)
				{
					AmSpeed += 50;
				}
				if(key == LeftDown)
				{
					AmSpeed -= 50;
					if(AmSpeed <= 0)
					{
						AmSpeed = 0;
					}
				}
			}
			else if(CmtSelect == 3) //DirectionP
			{
				if(key == LeftUp)
				{
					DirectionP += 10;
				}
				if(key == LeftDown)
				{
					DirectionP -= 10;
					if(DirectionP <= 0)
					{
						DirectionP = 0;
					}
				}
			}
			else if(CmtSelect == 4) //DirectionD
			{
				if(key == LeftUp)
				{
					DirectionD += 10;
				}
				if(key == LeftDown)
				{
					DirectionD -= 10;
					if(DirectionD <= 0)
					{
						DirectionD = 0;
					}
				}
			}
			else if(CmtSelect == 5) //Threshold
			{
				if(key == LeftUp)
				{
					Threshold ++ ;
				}
				if(key == LeftDown)
				{
					Threshold --;
					if(Threshold <= 1)
					{
						Threshold = 1;
					}
				}
			}
			
			else if(CmtSelect == 6) //Threshold2
			{
				if(key == LeftUp)
				{
					Threshold2 ++ ;
				}
				if(key == LeftDown)
				{
					Threshold2 --;
					if(Threshold2 <= 1)
					{
						Threshold2 = 1;
					}
				}
			}
			
			else if(CmtSelect == 7) //Mid_Threshold
			{
				if(key == LeftUp)
				{
					Mid_Threshold ++ ;
				}
				if(key == LeftDown)
				{
					Mid_Threshold --;
					if(Mid_Threshold <= 1)
					{
						Mid_Threshold = 1;
					}
				}
			}
			
			else if(CmtSelect == 8) //StopTime
			{
				if(key == LeftUp)
				{
					StopTime += 50;
				}
				if(key == LeftDown)
				{
					StopTime -= 50;
					if(StopTime <= 0)
					{
						StopTime = 0;
					}
				}
			}
			
			
		}
	}
}

