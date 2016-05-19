/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "Data.h"

/* GPIO端口定义 */
#define HW_GPIOA  (0x00U)   //PTA引脚端口定义
#define HW_GPIOB  (0x01U)   //PTB引脚端口定义
#define HW_GPIOC  (0x02U)   //PTC引脚端口定义
#define HW_GPIOD  (0x03U)   //PTD引脚端口定义
#define HW_GPIOE  (0x04U)   //PTE引脚端口定义
#define HW_GPIOF  (0x05U)   //PTF引脚端口定义

#define Left_Instance    PTC
#define Right_Instance	 PTA

#define Left_DDR     	   PTC
#define Left_DRR_pin   	 GPIO_Pin_13
#define Left_in 				 PCin(13)

#define right_DDR        PTC
#define right_DDR_Pin    GPIO_Pin_9
#define Right_in 				 PCin(9)

#define Left_Pinx        GPIO_Pin_5
#define Right_Pinx       GPIO_Pin_5


#define Left_DDRPinx     GPIO_Pin_13
#define Right_DDRPinx    GPIO_Pin_7



void Encoder_Init(void);
uint32_t Read_Left_CNT(void);
uint32_t Read_Right_CNT(void);
void Read_CNT(void);

#endif
