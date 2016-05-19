/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @杭州电子科技大学 信息工程学院CCD光电组
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.5 and 2.4
* @Date   : Found date 2014.9.7
*/
#include "CCD.h"

#if(CCD1_SWITCH==ON)
	ADC_Type * CCD1_ADCx=NULL;
#endif

#if(CCD2_SWITCH==ON)
	ADC_Type * CCD2_ADCx=NULL;
#endif


uint8_t Pixels1[128];
uint8_t Pixels2[128];
uint8_t Pixels3[128];
uint8_t Pixels4[128];

//-------------------CCD驱动函数--------------------
/**
*  @name   : CCD1_Init
*  @author : ZX
*  @tips   : 
*/
void CCD1_Init(void)
{
	uint8_t i;
	//-----------GPIO----------------
	GPIO_InitTypeDef CCD_GPIO_InitStructure1;
	//CCD1
	//SI
	CCD_GPIO_InitStructure1.GPIOx = CCD_PORTx;
	CCD_GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	CCD_GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	CCD_GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OPP;
	CCD_GPIO_InitStructure1.GPIO_Pin = CCD1_PIN_SI;
	GPIO_Init(&CCD_GPIO_InitStructure1);
	//CLK
	CCD_GPIO_InitStructure1.GPIOx = CCD_PORTx;
	CCD_GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	CCD_GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	CCD_GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OPP;
	CCD_GPIO_InitStructure1.GPIO_Pin = CCD1_PIN_CLK;
	GPIO_Init(&CCD_GPIO_InitStructure1);
	
	
	
	//CCD3
	//SI
	CCD_GPIO_InitStructure1.GPIOx = CCD_PORTx;
	CCD_GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	CCD_GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	CCD_GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OPP;
	CCD_GPIO_InitStructure1.GPIO_Pin = CCD3_PIN_SI;
	GPIO_Init(&CCD_GPIO_InitStructure1);
	//CLK
	CCD_GPIO_InitStructure1.GPIOx = CCD_PORTx;
	CCD_GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	CCD_GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	CCD_GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OPP;
	CCD_GPIO_InitStructure1.GPIO_Pin = CCD3_PIN_CLK;
	GPIO_Init(&CCD_GPIO_InitStructure1);
	
  //----------------ADC-----------------
	ADC_QuickInit(CCD1_ADC_Channel,kADC_SingleDiff12or13);
	ADC_QuickInit(CCD3_ADC_Channel,kADC_SingleDiff12or13);
	
	//------------Pixels Init---------------
	for(i=0;i<128;i++)
	{
		Pixels1[i] = 0;
		Pixels3[i] = 0;
	}
}


/**
*  @name   : hw_ad_mid(uint32_t MAP)
*  @author : ZX
*  @tips   : 中值滤波 软件方法
*/
uint16_t hw_ad_mid(uint32_t MAP)
{
	uint16_t i,j,k,tmp;
	
	i = ADC_QuickReadValue(MAP)>>4;
	j = ADC_QuickReadValue(MAP)>>4;
	k = ADC_QuickReadValue(MAP)>>4;

	if (i > j)
	{
		tmp = i; i = j; j = tmp;
	}
	if (k > j) 
	  tmp = j;
	else if(k > i) 
	  tmp = k; 
    else 
      tmp = i;
	return tmp;
}
/**
*  @name   : hw_ad_ave
*  @author : ZX
*  @tips   : 平均滤波
*/
uint16_t hw_ad_ave(uint32_t MAP,uint8_t N)
{
	uint32_t tmp = 0;
	uint8_t  i;
    for(i = 0; i < N; i++)
			tmp += hw_ad_mid( MAP);
	tmp = tmp / N; 
  return (uint16_t)tmp;
}

/**
*  @name   : SamplingDelay()
*  @author : ZX
*/
//100ns
void SamplingDelay(void)
{
	volatile uint8_t i ;
	for(i=0;i<1;i++) 
	{
		//10ns
		__asm("nop"); 
		__asm("nop");
	}
}

/**
*  @name   : CCDStartIntegration1
*  @author : ZX
*  @tips   : 
*/
void CCDStartIntegration1(void)
{
  //setup
  uint8_t i;
	
  //StartIntegration
  //all port line low
  SI_ClrVal(); 
  CLK_ClrVal(); 
  SamplingDelay();
	
  SI_SetVal();   											//start pin High
  SamplingDelay();                            
	CLK_SetVal();                       //clock pin High
  SamplingDelay();
  SI_ClrVal();                        //start pin Low
  SamplingDelay();         
  CLK_ClrVal();                       //clock pin Low 
	
  for(i=1;i<128;i++) 
  {
   SamplingDelay();                    
   CLK_SetVal();                       //clock pin High 
   SamplingDelay();        
   CLK_ClrVal();                       //clock pin Low                           
  }
  SamplingDelay();      
  CLK_SetVal();                      //第129个clk结束本次采集
  SamplingDelay();       
  CLK_ClrVal();             
}

/**
*  @name   : ImageCapture
*  @author : ZX
*  @tips   : ImageData为传递的数组形参
* 			     N表示第几个CCD
*/
void ImageCapture(uint8_t * ImageData,uint8_t N) //uint8_t (*ImageData)[128]) 
{

    uint8_t i;
		if(N == 1)
		{
				SI_SetVal();            /* SI  = 1 */
				SamplingDelay();
				CLK_SetVal();           /* CLK = 1 */
				SamplingDelay();
				SI_ClrVal();            /* SI  = 0 */
				SamplingDelay();
				
				for(i = 0; i < 150; i++) 
			  {                    //??250,?CCD??????????,
					SamplingDelay() ;  //200ns                  //???????????????????
				}
				//Sampling Pixel 1
				//*(*(ImageData+0)+0) = (hw_ad_mid(ADC0_SE1_DP1)>>4);
				//*ImageData = (hw_ad_mid(ADC0_SE1_DP1)>>4);
				*ImageData = ADC_QuickReadValue(CCD1_ADC_Channel)>>4;//for the k60 developed borad
				ImageData ++ ;
				
				CLK_ClrVal();           /* CLK = 0 */

				for(i=1; i<128; i++) 
				{
						SamplingDelay();
						//SamplingDelay();
						CLK_SetVal();       /* CLK = 1 */
						SamplingDelay();
						//SamplingDelay();
						//Sampling Pixel 2~128

					  //*(*(ImageData+0)+i) = (hw_ad_mid(ADC0_SE1_DP1)>>4);
						*ImageData = ADC_QuickReadValue(CCD1_ADC_Channel)>>4;//for the k60 developed borad
						ImageData ++ ;
					
						CLK_ClrVal();       /* CLK = 0 */
				}
				SamplingDelay();
				//SamplingDelay();
				CLK_SetVal();           /* CLK = 1 */
				SamplingDelay();
				//SamplingDelay();
				CLK_ClrVal();           /* CLK = 0 */
				SamplingDelay();
		}
		if(N == 2)
		{
			
		}
}
/**
*  @name   : RD_TSL1
*  @author : ZX
*  @tips   : 
*/
void RD_TSL1(uint8_t *ADV,uint32_t ADC_MAP) 
{
		uint8_t i=0,tslp=0;
		
		CLK_SetVal();
		SI_ClrVal(); 
		SamplingDelay();
				
		SI_SetVal();
		CLK_ClrVal();
		SamplingDelay();
				
		CLK_SetVal();
		SI_ClrVal();
		SamplingDelay();
		
		for(i=0;i<128;i++)
		{ 
			CLK_ClrVal();
			
			SamplingDelay();   
			//Pixels1[tslp]= hw_ad_ave(ADC0_SE1_DP1,3); 
			//Pixels1[tslp] = ADC_QuickReadValue(ADC0_SE1_DP1)>>4;
			ADV[tslp] = ADC_QuickReadValue(ADC_MAP)>>4;
			++tslp;
			
			CLK_SetVal(); 
			SamplingDelay();    
		}  
}

/**
*  @name   : RD_TSL2
*  @author : ZX
*  @tips   : 
*/
/*
void RD_TSL2(uint8_t *ADV,uint32_t ADC_MAP) 
{
		uint8_t i=0,tslp=0;
		
		CLK_SetVal2();
		SI_ClrVal2(); 
		SamplingDelay();
				
		SI_SetVal2();
		CLK_ClrVal2();
		SamplingDelay();
				
		CLK_SetVal2();
		SI_ClrVal2();
		SamplingDelay();
		
		for(i=0;i<128;i++)
		{ 
			CLK_ClrVal2();
			
			SamplingDelay();   
			//Pixels1[tslp]= hw_ad_ave(ADC0_SE1_DP1,3);  //ADC0-12CH??,PORTB2,10bit,5???
			//Pixels1[tslp] = ADC_QuickReadValue(ADC0_SE1_DP1)>>4;
			ADV[tslp] = ADC_QuickReadValue(ADC_MAP)>>4;
			++tslp;
			
			CLK_SetVal2(); 
			SamplingDelay();    
		}  
}
*/
/**
*  @name   : RD_TSL2
*  @author : ZX
*  @tips   : 
*/
void RD_TSL3(uint8_t *ADV,uint32_t ADC_MAP) 
{
		uint8_t i=0,tslp=0;
		
		CLK_SetVal3();
		SI_ClrVal3(); 
		SamplingDelay();
				
		SI_SetVal3();
		CLK_ClrVal3();
		SamplingDelay();
				
		CLK_SetVal3();
		SI_ClrVal3();
		SamplingDelay();
		
		for(i=0;i<128;i++)
		{ 
			CLK_ClrVal3();
			
			SamplingDelay();   
			//Pixels1[tslp]= hw_ad_ave(ADC0_SE1_DP1,3);  //ADC0-12CH??,PORTB2,10bit,5???
			//Pixels1[tslp] = ADC_QuickReadValue(ADC0_SE1_DP1)>>4;
			ADV[tslp] = ADC_QuickReadValue(ADC_MAP)>>4;
			++tslp;
			
			CLK_SetVal3(); 
			SamplingDelay();    
		}  
}

/**
*  @name   : Read_CCD1
*  @author : ZX
*  @tips   : 
*/
void Read_CCD1(void)
{
	RD_TSL1(Pixels1,CCD1_ADC_Channel);
}
/**
*  @name   : Read_CCD2
*  @author : ZX
*  @tips   : 
*/
/* 
void Read_CCD2(void)
{
	RD_TSL2(Pixels2,CCD2_ADC_Channel);
}*/

/**
*  @name   : Read_CCD3
*  @author : ZX
*  @tips   : 
*/
void Read_CCD3(void)
{
	RD_TSL3(Pixels3,CCD3_ADC_Channel);
}
