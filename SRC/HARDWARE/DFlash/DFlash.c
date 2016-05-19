/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "DFlash.h"

uint16_t Data[DataLength] = {0};
uint32_t ReadAddressOffset = 0;
uint32_t WriteAddressOffset = 0;
uint8_t  ArrayOffset = 0;
/**
*  @name   : DFlash_Init
*  @author : ZX
*  @tips   : 
*/
void DFlash_Init(void)
{
	//清除FMC缓冲区
	FMC->PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;
	FMC->PFB1CR |= FMC_PFB0CR_S_B_INV_MASK;
	// 禁止看门狗
	WDOG->UNLOCK = 0xC520;
	WDOG->UNLOCK = 0xD928;
	WDOG->STCTRLH = 0;    // 禁止看门狗
	//检查Flash访问错误
  if(FTFL->FSTAT & FTFL_FSTAT_ACCERR_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_ACCERR_MASK;       //清除错误标志
  }
  //检查保护错误
  else if (FTFL->FSTAT & FTFL_FSTAT_FPVIOL_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_FPVIOL_MASK;
  }
  //检查读冲突错误
  else if (FTFL->FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;
  }
  //禁用Flash模块的数据缓存
  FMC->PFB0CR &= ~FMC_PFB0CR_B0DCE_MASK;
  FMC->PFB1CR &= ~FMC_PFB1CR_B1DCE_MASK;
}

/**
*  @name   : DFlash_Write_BLOCK1
*  @author : ZX
*  @tips   : 31个参数
*/
uint8_t DFlash_Write_BLOCK1(uint16_t const *buffer )
{
		union
		{
			uint32_t  word;
			uint8_t   byte[4];
		} dest;
	
		dest.word = (uint32_t)(BLOCK_NUM1*BLOCK_SIZE);
		// 设置写入命令
		FTFL->FCCOB0 = PGM4;
		// 四字节对齐
	
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[1]);
		FTFL->FCCOB5 = BYTE0(buffer[1]);
		FTFL->FCCOB6 = BYTE1(buffer[0]);
		FTFL->FCCOB7 = BYTE0(buffer[0]);
		dest.word+=4; 
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[3]);
		FTFL->FCCOB5 = BYTE0(buffer[3]);
		FTFL->FCCOB6 = BYTE1(buffer[2]);
		FTFL->FCCOB7 = BYTE0(buffer[2]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[5]);
		FTFL->FCCOB5 = BYTE0(buffer[5]);
		FTFL->FCCOB6 = BYTE1(buffer[4]);
		FTFL->FCCOB7 = BYTE0(buffer[4]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[7]);
		FTFL->FCCOB5 = BYTE0(buffer[7]);
		FTFL->FCCOB6 = BYTE1(buffer[6]);
		FTFL->FCCOB7 = BYTE0(buffer[6]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[9]);
		FTFL->FCCOB5 = BYTE0(buffer[9]);
		FTFL->FCCOB6 = BYTE1(buffer[8]);
		FTFL->FCCOB7 = BYTE0(buffer[8]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[11]);
		FTFL->FCCOB5 = BYTE0(buffer[11]);
		FTFL->FCCOB6 = BYTE1(buffer[10]);
		FTFL->FCCOB7 = BYTE0(buffer[10]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[13]);
		FTFL->FCCOB5 = BYTE0(buffer[13]);
		FTFL->FCCOB6 = BYTE1(buffer[12]);
		FTFL->FCCOB7 = BYTE0(buffer[12]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[15]);
		FTFL->FCCOB5 = BYTE0(buffer[15]);
		FTFL->FCCOB6 = BYTE1(buffer[14]);
		FTFL->FCCOB7 = BYTE0(buffer[14]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[17]);
		FTFL->FCCOB5 = BYTE0(buffer[17]);
		FTFL->FCCOB6 = BYTE1(buffer[16]);
		FTFL->FCCOB7 = BYTE0(buffer[16]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[19]);
		FTFL->FCCOB5 = BYTE0(buffer[19]);
		FTFL->FCCOB6 = BYTE1(buffer[18]);
		FTFL->FCCOB7 = BYTE0(buffer[18]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[21]);
		FTFL->FCCOB5 = BYTE0(buffer[21]);
		FTFL->FCCOB6 = BYTE1(buffer[20]);
		FTFL->FCCOB7 = BYTE0(buffer[20]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[23]);
		FTFL->FCCOB5 = BYTE0(buffer[23]);
		FTFL->FCCOB6 = BYTE1(buffer[22]);
		FTFL->FCCOB7 = BYTE0(buffer[22]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[25]);
		FTFL->FCCOB5 = BYTE0(buffer[25]);
		FTFL->FCCOB6 = BYTE1(buffer[24]);
		FTFL->FCCOB7 = BYTE0(buffer[24]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[27]);
		FTFL->FCCOB5 = BYTE0(buffer[27]);
		FTFL->FCCOB6 = BYTE1(buffer[26]);
		FTFL->FCCOB7 = BYTE0(buffer[26]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[29]);
		FTFL->FCCOB5 = BYTE0(buffer[29]);
		FTFL->FCCOB6 = BYTE1(buffer[28]);
		FTFL->FCCOB7 = BYTE0(buffer[28]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
		
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = BYTE1(buffer[31]);
		FTFL->FCCOB5 = BYTE0(buffer[31]);
		FTFL->FCCOB6 = BYTE1(buffer[30]);
		FTFL->FCCOB7 = BYTE0(buffer[30]);
		dest.word+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
			return FLASH_ERROR;
  
  return FLASH_OK;
}

/**
*  @name   : DFLASH_ReadByte_BLOCK1
*  @author : ZX
*  @tips   : 
*/
void DFLASH_ReadByte_BLOCK1(uint16_t len,uint16_t *pbuffer)
{	
	union
	{
		uint16_t  word;
		uint8_t   byte[2];
	} data;
	uint8_t i = 0;
	uint8_t j = 0;
	
	for(i = 0;i < len;i++)
	{
	  data.byte[0] = *(uint8_t *)(BLOCK_NUM1*BLOCK_SIZE+j);		//读取指定地址的数据
		data.byte[1] = *(uint8_t *)(BLOCK_NUM1*BLOCK_SIZE+j+1);		//读取指定地址的数据
		*(pbuffer+i) = data.word;
		j = j + 2;
	}
}

/**
*  @name   : DFLASH_EraseSector_BLOCK1
*  @author : ZX
*  @tips   : 
*/
uint8_t DFLASH_EraseSector_BLOCK1(void)
{
	union
	{
		uint32_t  word;
		uint8_t   byte[4];
	} dest;
	
	dest.word = (uint32_t)(BLOCK_NUM1*BLOCK_SIZE);
	
	// 设置擦除命令
	FTFL->FCCOB0 = ERSSCR; // 擦除扇区命令
	// 设置目标地址
	FTFL->FCCOB1 = dest.byte[2];
	FTFL->FCCOB2 = dest.byte[1];
	FTFL->FCCOB3 = dest.byte[0];
	//检测命令是否执行正常
		
	if(FLASH_OK == CommandLaunch())
	{
		return FLASH_OK;
	}
	else
	{
		return FLASH_ERROR;
	}
}

/**
*  @name   : Data_Init
*  @author : ZX
*  @tips   : 
*/
void Data_Read(void)
{
	DFLASH_ReadByte_BLOCK1(DataLength,Data); // 将Flash中参数放回数组
	//将参数初始化
	//EndlongP    = Data[0];
	//EndlongD    = Data[1];
	Speed_P     = Data[2];
	Speed_I     = Data[3];
	Speed_D     = Data[4];
	DirectionP  = Data[5];
	S_bar_cnt  = Data[6];
	DirectionD  = Data[7];
	Angle_Set   = Data[8];
	AmSpeed     = Data[9];
	Threshold   = Data[10];
	LeftMotorDead  = Data[11];
	RightMotorDead = Data[12];
	Mid_Threshold     = Data[13];
	Threshold2 = Data[14];
	black_threshold = Data[15];
	podao_threshold = Data[16];
	zhijiao_count = Data[17];
	
	Obstacle_Switch = Data[18];
	Single_Mid_Switch = Data[19];
	Zhijiao_Switch = Data[20];
	Podao_Switch = Data[21];
	StartSwitch = Data[22];
	obstacle_conut= Data[23];
	StopTime = Data[24]; 
	zhijiao_s_bar_cnt = Data[25]; 
	podao_last_time = Data[26];
	Stop_Flag = Data[27];
	White_value = Data[28];
}
/**
*  @name   : Motor_Init
*  @author : ZX
*  @tips   : 
*/
void Data_Write(void)
{
	
	//将参数传回数组
	//Data[0] = EndlongP;
	//Data[1] = EndlongD;
	Data[2] = Speed_P;
	Data[3] = Speed_I;
	Data[4] = Speed_D;
	Data[5] = DirectionP;
	Data[6] = S_bar_cnt;
	Data[7] = DirectionD;
	Data[8] = Angle_Set;
	Data[9] = AmSpeed;
	Data[10] = Threshold;
	Data[11] = LeftMotorDead;
	Data[12] = RightMotorDead;
	Data[13] = Mid_Threshold;
	Data[14] = Threshold2;
	Data[15] = black_threshold;
	Data[16] = podao_threshold;
	Data[17] = zhijiao_count;
	
	Data[18] = Obstacle_Switch;
	Data[19] = Single_Mid_Switch;
	Data[20] = Zhijiao_Switch;
	Data[21] = Podao_Switch;
	Data[22] = StartSwitch;
	Data[23] = obstacle_conut;
	Data[24] = StopTime;
	Data[25] = zhijiao_s_bar_cnt;
	Data[26] = podao_last_time;
	Data[27] = Stop_Flag;
	Data[28] = White_value;
	
	//数组写入flash
	DFlash_Write_BLOCK1(Data);
}
