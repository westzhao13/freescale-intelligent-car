/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/

#ifndef _L3G4200D_H_
#define _L3G4200D_H_


#include "i2c.h"



#define L3G4200D_I2C_PIN_MAP  I2C0_SCL_PB0_SDA_PB1
#define L3G4200D_I2C_PORT     I2C0


#define L3G4200D_SCL   PBout(0)
#define L3G4200D_SDA   PBout(1)
#define L3G4200D_SDAIN PBin(1)

#define	L3G4200_Addr   0xD2	 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
//**********L3G4200D内部寄存器地址*********
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define REFERENCE 0x25
#define OUT_TEMP 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0xa8
#define OUT_X_H 0xa9
#define OUT_Y_L 0xaA
#define OUT_Y_H 0xaB
#define OUT_Z_L 0xaC
#define OUT_Z_H 0xaD
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define INT1_CFG 0x30
#define INT1_SRC 0x31
#define INT1_TSH_XH 0x32
#define INT1_TSH_XL 0x33
#define INT1_TSH_YH 0x34
#define INT1_TSH_YL 0x35
#define INT1_TSH_ZH 0x36
#define INT1_TSH_ZL 0x37
#define INT1_DURATION 0x38
//------------------------------------


void InitL3G4200D(void);
void Single_WriteL3G4200D(I2C_Type* I2Cx,uint8_t REG_Address,uint8_t REG_data);
uint8_t Single_ReadL3G4200D(I2C_Type* I2Cx,uint8_t REG_Address);

int Read_Gyro_X(void);
int Read_Gyro_Y(void);
int Read_Gyro_Z(void);

#endif
