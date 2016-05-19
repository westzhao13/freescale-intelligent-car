/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _UARTTOPC_H_
#define _UARTTOPC_H_

#include "uart.h"

//将“int”类型的数据分成两个单字节的数据
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
	

/*函数声明*/
//void Uart_Init(void);
void SendHex(unsigned char hex);
void SendImageData(unsigned char * ImageData);

#define AFTERDOT  5  // 小数点后精度

//向pc端发送4个带符号浮点数（16位长），默认数据格式 .xxxxx	保留到小数点后5位
void Serial_Send(float dat0,float dat1,float dat2,float dat3);

uint8_t ftostr(float fNum,  uint8_t str[],uint8_t dotsize); //将浮点数据转化为字符串
float strtofloat(uint8_t str[],uint8_t len); //将纯数字的浮点字符串转化成字符数据


void Data_Send_Begins(uint8_t x1,uint8_t x2,uint8_t x3,uint8_t x4);
void Data_Send_Senser(int16_t x,int16_t y,int16_t z,int16_t a,int16_t b,int16_t c,int16_t d);
void Uart_Init(void);

#endif
