/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "Led.h"
/**
*  @name   : LED_Init
*  @author : ZX
*  @tips   : 
*/
void LED_Init(void)
{
	
}
/**
*  @name   : LED_Test_Init
*  @author : ZX
*  @tips   : 
*/
void LED_Test_Init(void)
{
	//test
	GPIO_InitTypeDef GPIO_InitStructure1;
	
	//black
	GPIO_InitStructure1.GPIOx = PTB;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(&GPIO_InitStructure1);
	
	
	GPIO_InitStructure1.GPIOx = PTB;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(&GPIO_InitStructure1);
	
	
	
	//beep
	GPIO_InitStructure1.GPIOx = PTB;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_21;
	GPIO_Init(&GPIO_InitStructure1);
	
	//infrared
	
	//ADC_QuickInit(Infrared_Black_Map,kADC_SingleDIff16);
	ADC_QuickInit(Infrared_Podao_Map,kADC_SingleDiff12or13);
	
	//起跑线
	GPIO_InitStructure1.GPIOx = PTB;
	GPIO_InitStructure1.GPIO_InitState  = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(&GPIO_InitStructure1);
	
}
