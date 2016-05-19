/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _MENU_H_
#define _MENU_H_

#include "Data.h"

#define Confirm     1

#define RightUp 		5
#define RightDown 	3

#define LeftUp			2
#define LeftDown		4


void KeyScan2(void);
void menu(void);

void MenuDisplay(void);
void KeyScan(void);


#endif
