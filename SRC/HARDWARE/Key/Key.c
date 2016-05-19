/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "Key.h"

void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIOx = Key1_Portx;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = Key1_Pin;
	GPIO_Init(&GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIOx = Key2_Portx;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = Key2_Pin;
	GPIO_Init(&GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIOx = Key3_Portx;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = Key3_Pin;
	GPIO_Init(&GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIOx = Key4_Portx;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = Key4_Pin;
	GPIO_Init(&GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIOx = Key5_Portx;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = Key5_Pin;
	GPIO_Init(&GPIO_InitStructure1);
}

void KeyDelay(uint16_t x)
{
	DelayMs(x);
}

uint8_t GetKey(void)
{
	uint8_t Key=0;
	
	if(Key1In==0)
	{
		KeyDelay(KeyDelayTime);
		if(Key1In==0)
		{
			Key = 1;
		}
	}
	
	if(Key2In==0)
	{
		KeyDelay(KeyDelayTime);
		if(Key2In==0)
		{
			Key = 2;
		}
	}
	
	if(Key3In==0)
	{
		KeyDelay(KeyDelayTime);
		if(Key3In==0)
		{
			Key = 3;
		}
	}
	
	if(Key4In==0)
	{
		KeyDelay(KeyDelayTime);
		if(Key4In==0)
		{
			Key = 4;
		}
	}
	
	if(Key5In==0)
	{
		KeyDelay(KeyDelayTime);
		if(Key5In==0)
		{
			Key = 5;
		}
	}
	
	return Key;
}

