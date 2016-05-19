/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.5
* @Date   : Found date 2014.9.7
*/
#include "MMA7361.h"
/**
*  @name   : MMA7361_Init
*  @author : ZX
*  @tips   : 
*/
void MMA7361_Init(void)
{

	
	ADC_QuickInit(MMA7361_X_Channel,kADC_SingleDIff16); //X
	ADC_QuickInit(MMA7361_Y_Channel,kADC_SingleDIff16); //Y
	ADC_QuickInit(MMA7361_Z_Channel,kADC_SingleDIff16); //Z
	

	
	
	
	
}

/**
*  @name   : Read_ACC_X
*  @author : ZX
*  @tips   : 
*/
uint16_t Read_ACC_X(void)
{
	uint16_t AD_Value;
	AD_Value = ADC_QuickReadValue(MMA7361_X_Channel);
	return AD_Value;
}

/**
*  @name   : Read_ACC_Y
*  @author : ZX
*  @tips   : 
*/
uint16_t Read_ACC_Y(void)
{
	uint16_t AD_Value;
	AD_Value = ADC_QuickReadValue(MMA7361_Y_Channel);
	return AD_Value;
}
/**
*  @name   : Read_ACC_Z
*  @author : ZX
*  @tips   : 
*/
uint16_t Read_ACC_Z(void)
{
	uint16_t AD_Value;
	AD_Value = ADC_QuickReadValue(MMA7361_Z_Channel);
	return AD_Value;
}
