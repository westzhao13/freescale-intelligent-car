/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _INCLUDES_H_
#define _INCLUDES_H_

//库函数头文件
//--------LIB------------
#include "sys.h"
#include "gpio.h"
#include "delay.h"
#include "wdog.h"
#include "pit.h"
#include "dma.h"
#include "ftm.h"
#include "lptm.h"
#include "adc.h"
#include "tsi.h"
#include "dac.h"
#include "spi.h"
#include "i2c.h"
#include "pdb.h"
#include "sd.h"
#include "flash.h"
#include "stdio.h"
#include "uart.h"

//外设自己定义的头文件
//------HARDWARE---------
#include "Data.h" //此头文件包含了相关硬件头文件 需要用到的外部全局变量

#define  Inline  __forceinline //强制内联


//定义CPU
//默认为512
#define MDK60DN512

//若不为512则为256
#ifndef MDK60DN512
	#define MDK60DN256
#endif

#define TESTTIME OFF

#define ON       1
#define OFF      0 

#define TRUE     1
#define FALSE    0

//function
#define abs(x)      ( (x)>0?(x):-(x) )
#define max(a,b)    ( (a)>(b)?(a):(b))
#define min(a,b)    ( (a)<(b)?(b):(a))

#define MA 3

#endif
