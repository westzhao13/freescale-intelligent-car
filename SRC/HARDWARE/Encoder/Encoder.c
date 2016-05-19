/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.5
* @Date   : Found date 2014.9.7
*/
#include "Encoder.h"

uint8_t dummy1, dummy2;

static const uint32_t DMA_PORT_TriggerSourceTable[] = 
{
    PORTA_DMAREQ,
    PORTB_DMAREQ,
    PORTC_DMAREQ,
    PORTD_DMAREQ,
    PORTE_DMAREQ,
};

/**
 * @brief  DMA 用作脉冲计数初始化     
 * @param  dmaChl :DMA通道号
 * @param  instance :端口号 比如HW_GPIOA
 * @param  pinIndex :引脚号
 * @retval None
 */
static void DMA_PulseCountInit(uint32_t dmaChl, GPIO_Type *GPIOx, uint32_t instance, uint16_t pinIndex)
{
		GPIO_InitTypeDef GPIO_InitStructure1;
		DMA_InitTypeDef DMA_InitStruct1 = {0};
    /* 开启2路引脚 配置为DMA触发 */
		GPIO_InitStructure1.GPIOx = GPIOx;
		GPIO_InitStructure1.GPIO_Pin = pinIndex;
		GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DMA_RISING;
		GPIO_Init(&GPIO_InitStructure1);
    //GPIO_QuickInit(instance, pinIndex, kGPIO_Mode_IFT);
    /* 配置为DMA上升沿触发 */
    //GPIO_ITDMAConfig(instance, pinIndex, kGPIO_DMA_RisingEdge, true);
    /* 配置DMA */
    DMA_InitStruct1.chl = dmaChl;  
    DMA_InitStruct1.chlTriggerSource = DMA_PORT_TriggerSourceTable[instance];
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal; 
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = DMA_CITER_ELINKNO_CITER_MASK; /* 最大值 */
    
    DMA_InitStruct1.sAddr = (uint32_t)&dummy1;
    DMA_InitStruct1.sLastAddrAdj = 0; 
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = (uint32_t)&dummy2; 
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 0; 
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_Init(&DMA_InitStruct1);
    /* 启动传输 */
    DMA_EnableRequest(dmaChl);
}
/**
*  @name   : Encoder_Init
*  @author : ZX
*  @tips   : 
*/
void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;
	
	GPIO_InitStructure1.GPIOx = Left_DDR;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = Left_DRR_pin;
	GPIO_Init(&GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIOx = right_DDR;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = right_DDR_Pin;
	GPIO_Init(&GPIO_InitStructure1);
	
	DMA_PulseCountInit(HW_DMA_CH0,PTC,HW_GPIOC,5);
	DMA_PulseCountInit(HW_DMA_CH1,PTA,HW_GPIOA,5);
}
/**
*  @name   : Read_Left_CNT
*  @author : ZX
*  @tips   : 
*/
uint32_t Read_Left_CNT(void)
{
		uint32_t value;
    /* CH0 */
    value = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH0);
    /* 清零计数 */
    DMA_CancelTransfer();
    DMA_SetMajorLoopCounter(HW_DMA_CH0, DMA_CITER_ELINKNO_CITER_MASK);
    /* 开始下一次传输 */
    DMA_EnableRequest(HW_DMA_CH0);
		return value;
}
/**
*  @name   : Read_Right_CNT
*  @author : ZX
*  @tips   : 
*/
uint32_t Read_Right_CNT(void)
{
		uint32_t value;
    /* CH1 */
    value = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH1);
    /* 清零计数 */
    DMA_CancelTransfer();
    DMA_SetMajorLoopCounter(HW_DMA_CH1, DMA_CITER_ELINKNO_CITER_MASK);
    /* 开始下一次传输 */
    DMA_EnableRequest(HW_DMA_CH1);
		return value;
}

/**
*  @name   : Read_CNT
*  @author : ZX
*  @tips   : 
*/
void Read_CNT(void)
{
	  /* 由于DMA 是倒计数的 所需需要用最大值减一下 */
    /* CH0 */
    LeftSpeedNow = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH0);
    /* CH1 */
    RightSpeedNow = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH1);
    /* 清零计数 */
    DMA_CancelTransfer();
    DMA_SetMajorLoopCounter(HW_DMA_CH0, DMA_CITER_ELINKNO_CITER_MASK);
    DMA_SetMajorLoopCounter(HW_DMA_CH1, DMA_CITER_ELINKNO_CITER_MASK);
    /* 开始下一次传输 */
    DMA_EnableRequest(HW_DMA_CH0);
    DMA_EnableRequest(HW_DMA_CH1);
}
