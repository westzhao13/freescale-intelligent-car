/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _KEY_H_
#define _KEY_H_

#include "Data.h"

/*	Key Map	*/
#define Key1_Portx  	PTA				//confirm
#define Key1In 				PAin(15)
#define Key1_Pin			GPIO_Pin_15

#define Key2_Portx		PTA			//rightup
#define Key2In 				PAin(12) //12
#define Key2_Pin			GPIO_Pin_12

#define Key3_Portx		PTA				//leftup
#define Key3In 				PAin(13)
#define Key3_Pin			GPIO_Pin_13

#define Key4_Portx		PTE		//rightdown
#define Key4In 				PEin(26)
#define Key4_Pin			GPIO_Pin_26

#define Key5_Portx		PTA				//leftdown
#define Key5In 				PAin(14)
#define Key5_Pin			GPIO_Pin_14

#define KeyDelayTime	80


void KeyInit(void);
uint8_t GetKey(void);

#endif
