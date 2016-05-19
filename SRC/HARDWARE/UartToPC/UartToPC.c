/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @º¼Öİµç×Ó¿Æ¼¼´óÑ§ ĞÅÏ¢¹¤³ÌÑ§ÔºCCD¹âµç×é
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "UartToPC.h"

/**
*  @name   : Uart_Init
*  @author : ZX
*  @tips   : 
*/
void Uart_Init(void)
{
	UART_DebugPortInit(UART4_RX_E24_TX_E25,115200);
}

/**
*  @name   : SendHex
*  @author : ZX
*  @tips   : 
*/
void SendHex(unsigned char hex) 
{
  unsigned char temp;
  temp = hex >> 4;
  if(temp < 10) {
    UART_SendData(UART4,temp + '0');
  } else {
    UART_SendData(UART4,temp - 10 + 'A');
  }
  temp = hex & 0x0F;
  if(temp < 10) {
    UART_SendData(UART4,temp + '0');
  } else {
   UART_SendData(UART4,temp - 10 + 'A');
  }
}

/**
*  @name   : SendImageData
*  @author : ZX
*  @tips   : 
*/
void SendImageData(unsigned char * ImageData) 
{

    unsigned char i;
    unsigned char crc = 0;

    /* Send Data */
    UART_SendData(UART4,'*');
    UART_SendData(UART4,'L');
    UART_SendData(UART4,'D');

    SendHex(0);
    SendHex(0);
    SendHex(0);
    SendHex(0);

    for(i=0; i<128; i++) {
      SendHex(*ImageData++);
    }

    SendHex(crc);
    UART_SendData(UART4,'#');
}


//SerialChart
uint8_t sendbuf[4][15];      // 4¸öÍ¨µÀ£¬Ã¿¸öÍ¨µÀÊı¾İ³¤¶È×î³¤20¸öASCII×Ö·û

/**
*  @name   : Send_dat
*  @author : ZX
*  @tips   : 
*/
void Send_dat(uint8_t dat[],uint8_t ledgth);  // ·¢ËÍÒ»¸öÍ¨µÀµÄÊı¾İ,Êı×éµÄ³¤¶È length
/**
*  @name   : Serial_Send
*  @author : ZX
*  @tips   : 
*/
void Serial_Send(float dat0,float dat1,float dat2,float dat3)
{
  uint8_t CH0Len=0,CH1Len=0,CH2Len=0,CH3Len=0; // ¸÷¸öÍ¨µÀËùÒª·¢ËÍÊı¾İµÄÊµ¼Ê³¤¶È
 // ¸¡µãÊı×ª»»³ÉASCIIÊı×é
  CH0Len=ftostr(dat0,  sendbuf[0],AFTERDOT) ;
  CH1Len=ftostr(dat1,  sendbuf[1],AFTERDOT) ;
  CH2Len=ftostr(dat2,  sendbuf[2],AFTERDOT) ;
  CH3Len=ftostr(dat3,  sendbuf[3],AFTERDOT) ;
  
  Send_dat(sendbuf[0],CH0Len);          // serialchart Êı¾İ¸ñÊ½ £º ASCII + , + ASCII + ,+.....+\n
  Send_dat(sendbuf[1],CH1Len);
  Send_dat(sendbuf[2],CH2Len);
  Send_dat(sendbuf[3],CH3Len);
  UART_printf("\n");
}
/**
*  @name   : Send_dat
*  @author : ZX
*  @tips   : 
*/
void Send_dat(uint8_t dat[],uint8_t length )
{
  uint8_t i=0;
  for(i=0;i<length;i++)
  {
    UART_printf("%c",dat[i]);
  }
  UART_printf(",");             
}

// float ÀàĞÍÊı¾İ×ª»»³ÉASCIIÀàĞÍstringµÄº¯Êı
// º¯ÊıÃû£ºftostr
// ÊäÈë²ÎÊı£º´ı×ª»»Êı×Ö£¬´æ·Å×Ö·û´®£¬¸¡µãĞ¡Êıºó±ßÎ»Êı
// ÃèÊö£º½«¸¡µãĞÍÊı¾İ×ª»»Îª×Ö·û´®
// ·µ»ØÖµ:×Ö·û´®´óĞ¡
//============================================================================
/**
*  @name   : ftostr
*  @author : ZX
*  @tips   : 
*/
uint8_t ftostr(float fNum,  uint8_t str[],uint8_t dotsize)
{
	// ¶¨Òå±äÁ¿
	 uint8_t iSize=0;//¼ÇÂ¼×Ö·û´®³¤¶ÈµÄÊı
	 uint8_t n=0;//ÓÃ×÷Ñ­»·µÄÁÙÊ±±äÁ¿
	 uint8_t *p=str;//×ö»»ÏòÊ±ÓÃµÄÖ¸Õë
	 uint8_t *s=str;//×ö»»ÏòÊ±ÓÃµÄÖ¸Õë
	 uint8_t isnegative=0;//¸ºÊı±êÖ¾
	 uint32_t i_predot;//Ğ¡ÊıµãÇ°µÄÊı
	 uint32_t i_afterdot;//Ğ¡ÊıµãºóµÄÊı
	 float f_afterdot;//ÊµÊıĞÍµÄĞ¡Êı²¿·Ö
               
	//ÅĞ¶ÏÊÇ·ñÎª¸ºÊı
	if(fNum<0)
	{
		isnegative=1;//ÉèÖÃ¸ºÊı±êÖ¾
		fNum=-fNum;//½«¸ºÊı±äÎªÕıÊı
	}

	i_predot=(unsigned long int)fNum;//½«Ğ¡ÊıµãÖ®Ç°µÄÊı±äÎªÕûÊı
	f_afterdot=fNum-i_predot;//µ¥¶ÀÈ¡³öĞ¡ÊıµãÖ®ºóµÄÊı
	//¸ù¾İÉè¶¨µÄÒª±£´æµÄĞ¡ÊıµãºóµÄÎ»Êı£¬½«Ğ¡ÊıµãºóÏàÓ¦µÄÎ»Êı±äµ½Ğ¡ÊıµãÖ®Ç°
	for(n=dotsize;n>0;n--)
	{
		f_afterdot=f_afterdot*10;
	}
	i_afterdot=(unsigned long int)f_afterdot;//½«Ğ¡ÊıµãºóÏàÓ¦Î»ÊıµÄÊı×Ö±äÎªÕûÊı

	//ÏÈ½«Ğ¡ÊıµãºóµÄÊı×ª»»Îª×Ö·û´®
	n=dotsize;
	while(i_afterdot>0|n>0)
	{
		n--;
	 	str[iSize++]=i_afterdot%10+'0';//¶Ô10È¡Óà²¢±äÎªASCIIÂë
	 	i_afterdot=i_afterdot/10;//¶Ô10È¡ÉÌ
	}
	str[iSize++] ='.';//¼ÓÉÏĞ¡Êıµã
    //´¦ÀíĞ¡ÊıµãÇ°Îª0µÄÇé¿ö¡£
	if(i_predot==0)
		str[iSize++]='0';
	//ÔÙ½«Ğ¡ÊıµãÇ°µÄÊı×ª»»Îª×Ö·û´®
	while(i_predot>0)
	{
	 	str[iSize++]=i_predot%10+'0';//¶Ô10È¡Óà²¢±äÎªASCIIÂë
	 	i_predot=i_predot/10;//¶Ô10È¡ÉÌ
	}

	if(isnegative==1)
		str[iSize++]='-';//Èç¹ûÊÇ¸ºÊı£¬ÔòÔÚ×îºó¼ÓÉÏ¸ººÅ
       	

	p=str+iSize-1;//½«PÖ¸ÕëÖ¸Ïò×Ö·û´®½áÊø±êÖ¾Ö®Ç°

	for(;p-s>0;p--,s++)//½«×Ö·û´®ÖĞ´æ´¢µÄÊıµ÷Í·
	{
		*s^=*p;
		*p^=*s;
		*s^=*p;
	}
      //  *s='\0';     // ×Ö·û´®½áÊø±êÖ¾
	// ·µ»ØÖ¸Õë×Ö·û´®´óĞ¡
	return iSize;
}
/**
*  @name   : strtofloat
*  @author : ZX
*  @tips   : 
*/
float strtofloat(uint8_t str[],uint8_t len) //½«´ø·ûºÅÊı×ÖµÄ¸¡µã×Ö·û´®×ª»»³É¸¡µãÊı¾İ
{
  int8_t i=0,dot=0;
  uint8_t isnegative=0;//¸ºÊı±êÖ¾
  float tempint=0,tempf=0.0,value=0.0;
  
  for(i=0;i<len;i++)       // ¼ìË÷Ğ¡ÊıµãºÍÊı×Ö½áÊø±êÖ¾
  {
    if(str[i]=='.')
    {
      dot=i;
      break;
    }
  }
  if(dot==0) // Ã»ÓĞĞ¡Êıµã
  {
    dot=len;
  }
  
  if(str[0]=='-')        // ¸ºÊı
  {
    isnegative=1;
  }
  for(i=isnegative;i<dot;i++)        // ÕûÊı²¿·Ö
  {
      str[i]-='0';;   // ×Ö·û×ªÕûÊı
      tempint=tempint*10+str[i];
  }
  for(i=len;i>dot;i--)         // Ğ¡Êı²¿·Ö
  {
    if(str[i]>=0x30&&str[i]<=0x39)
    {
      str[i]-=0x30;
      tempf=tempf*0.1+str[i]*0.1;
    }
  }
  value=tempint+tempf;
  if(isnegative)
  {
    value=-value;
  }
  return(value);
  
}

//ÄäÃûÉÏÎ»»ú
/**
*  @name   : Data_Send_Begins
*  @author : ZX
*  @tips   : 
*/
uint16_t data_to_send[128];
//·¢ËÍËÄ¸öccdµÄÉ¨ÃèÆğÊ¼µãÖµ
void Data_Send_Begins(uint8_t x1,uint8_t x2,uint8_t x3,uint8_t x4)
{
	uint8_t _cnt=0;
	uint16_t sum = 0;
	uint8_t i;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xF1;
	data_to_send[_cnt++]=0;

	data_to_send[_cnt++] = x1;
	data_to_send[_cnt++] = x2;
	data_to_send[_cnt++] = x3;
	data_to_send[_cnt++] = x4;
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	for(i=0;i<_cnt;i++)
		UART_SendData(UART4,data_to_send[i]);
}
//send endlong data
/**
*  @name   : Data_Send_Senser
*  @author : ZX
*  @tips   : 
*/
void Data_Send_Senser(int16_t x,int16_t y,int16_t z,int16_t a,int16_t b,int16_t c,int16_t d)
{
	uint8_t _cnt=0;
	uint32_t sum = 0;
	uint8_t i;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xF2;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE1(x);  //?8?
	data_to_send[_cnt++]=BYTE0(x);  //?8?
	data_to_send[_cnt++]=BYTE1(y);
	data_to_send[_cnt++]=BYTE0(y);
	data_to_send[_cnt++]=BYTE1(z);
	data_to_send[_cnt++]=BYTE0(z);
  data_to_send[_cnt++]=BYTE1(a);  //?8?
	data_to_send[_cnt++]=BYTE0(a);  //?8?
	data_to_send[_cnt++]=BYTE1(b);
	data_to_send[_cnt++]=BYTE0(b);
	data_to_send[_cnt++]=BYTE1(c);
	data_to_send[_cnt++]=BYTE0(c);
	data_to_send[_cnt++]=BYTE1(d);
	data_to_send[_cnt++]=BYTE0(d);
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	for(i=0;i<_cnt;i++)
		UART_SendData(UART4,data_to_send[i]);
}


/*
void Data_Send_Status(void)
{
	uint8_t _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	uint16_t _temp;
	_temp = (int)(Att_Angle.rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Att_Angle.pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Att_Angle.yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	vs32 _temp2 = Alt;
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
		
	if(Rc_C.ARMED==0)		data_to_send[_cnt++]=0xA0;	
	else if(Rc_C.ARMED==1)		data_to_send[_cnt++]=0xA1;
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;

	Uart1_Put_Buf(data_to_send,_cnt);
}

void Data_Send_Senser(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(Acc.X);  //?8?
	data_to_send[_cnt++]=BYTE0(Acc.X);  //?8?
	data_to_send[_cnt++]=BYTE1(Acc.Y);
	data_to_send[_cnt++]=BYTE0(Acc.Y);
	data_to_send[_cnt++]=BYTE1(Acc.Z);
	data_to_send[_cnt++]=BYTE0(Acc.Z);
	data_to_send[_cnt++]=BYTE1(Gyr.X);
	data_to_send[_cnt++]=BYTE0(Gyr.X);
	data_to_send[_cnt++]=BYTE1(Gyr.Y);
	data_to_send[_cnt++]=BYTE0(Gyr.Y);
	data_to_send[_cnt++]=BYTE1(Gyr.Z);
	data_to_send[_cnt++]=BYTE0(Gyr.Z);
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;

	Uart1_Put_Buf(data_to_send,_cnt);
}

¦3:??????
void Data_Send_RCData(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(Rc_D.THROTTLE);
	data_to_send[_cnt++]=BYTE0(Rc_D.THROTTLE);
	data_to_send[_cnt++]=BYTE1(Rc_D.YAW);
	data_to_send[_cnt++]=BYTE0(Rc_D.YAW);
	data_to_send[_cnt++]=BYTE1(Rc_D.ROLL);
	data_to_send[_cnt++]=BYTE0(Rc_D.ROLL);
	data_to_send[_cnt++]=BYTE1(Rc_D.PITCH);
	data_to_send[_cnt++]=BYTE0(Rc_D.PITCH);
	data_to_send[_cnt++]=BYTE1(Rc_D.AUX1);
	data_to_send[_cnt++]=BYTE0(Rc_D.AUX1);
	data_to_send[_cnt++]=BYTE1(Rc_D.AUX2);
	data_to_send[_cnt++]=BYTE0(Rc_D.AUX2);
	data_to_send[_cnt++]=BYTE1(Rc_D.AUX3);
	data_to_send[_cnt++]=BYTE0(Rc_D.AUX3);
	data_to_send[_cnt++]=BYTE1(Rc_D.AUX4);
	data_to_send[_cnt++]=BYTE0(Rc_D.AUX4);
	data_to_send[_cnt++]=BYTE1(Rc_D.AUX5);
	data_to_send[_cnt++]=BYTE0(Rc_D.AUX5);
	data_to_send[_cnt++]=BYTE1(Rc_D.AUX6);
	data_to_send[_cnt++]=BYTE0(Rc_D.AUX6);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	Uart1_Put_Buf(data_to_send,_cnt);
}

¦4:??PWM?
void Data_Send_MotoPWM(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(Moto_PWM_1);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_1);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_2);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_2);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_3);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_3);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_4);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_4);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_5);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_5);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_6);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_6);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_7);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_7);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_8);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_8);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart1_Put_Buf(data_to_send,_cnt);
}

¦5:???????
void Data_Send_OFFSET(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x16;
	data_to_send[_cnt++]=0;
	vs16 _temp = AngleOffset_Rol*1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = AngleOffset_Pit*1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart1_Put_Buf(data_to_send,_cnt);
}

¦6:??PID1
void Data_Send_PID1(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = PID_ROL.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_ROL.I * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_ROL.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PIT.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PIT.I * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PIT.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_YAW.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_YAW.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_YAW.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart1_Put_Buf(data_to_send,_cnt);
}

¦7:??PID2
void Data_Send_PID2(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x11;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = PID_ALT.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_ALT.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_ALT.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_POS.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_POS.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_POS.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_1.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_1.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_1.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart1_Put_Buf(data_to_send,_cnt);
}

¦8:??PID3
void Data_Send_PID3(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x12;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = PID_PID_2.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_2.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_2.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_3.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_3.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_3.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_4.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_4.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_4.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart1_Put_Buf(data_to_send,_cnt);
}

¦8:??PID4
void Data_Send_PID4(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x13;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = PID_PID_5.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_5.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_5.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_6.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_6.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_6.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_7.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_7.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_7.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart1_Put_Buf(data_to_send,_cnt);
}

¦8:??PID5
void Data_Send_PID5(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x14;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = PID_PID_8.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_8.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_8.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_9.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_9.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_9.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_10.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_10.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_10.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart1_Put_Buf(data_to_send,_cnt);
}

¦8:??PID6
void Data_Send_PID6(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x15;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = PID_PID_11.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_11.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_11.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_12.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_12.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_12.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);

	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart1_Put_Buf(data_to_send,_cnt);
}

¦9:??????
void Data_Send_Check(u16 check)
{
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xF0;
	data_to_send[3]=3;
	data_to_send[4]=0xBA;
	
	data_to_send[5]=BYTE1(check);
	data_to_send[6]=BYTE0(check);
	
	u8 sum = 0;
	for(u8 i=0;i<7;i++)
		sum += data_to_send[i];
	
	data_to_send[7]=sum;

	Uart1_Put_Buf(data_to_send,8);
}

¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
¦¦??????????????????

¦void Data_Receive_Anl(u8 *data_buf,u8 num)
{
	vs16 rc_value_temp;
	u8 sum = 0;
	
	for(u8 i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//??sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//????
/////////////////////////////////////////////////////////////////////////////////////
	if(*(data_buf+2)==0X01)
	{
		if(*(data_buf+4)==0X01)
			MPU6050_CalOff_Acc();
		if(*(data_buf+4)==0X02)
			MPU6050_CalOff_Gyr();
		if(*(data_buf+4)==0X03)
		{MPU6050_CalOff_Acc();MPU6050_CalOff_Gyr();}
//		if(*(data_buf+4)==0X04)
//			Cal_Compass();
//		if(*(data_buf+4)==0X05)
//			MS5611_CalOffset();
	}
	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{
			Send_PID1 = 1;
			Send_PID2 = 1;
			Send_PID3 = 1;
			Send_PID4 = 1;
			Send_PID5 = 1;
			Send_PID6 = 1;
		}
		if(*(data_buf+4)==0X02)
			Send_Offset = 1;
	}
#ifndef CONTROL_USE_RC
	if(*(data_buf+2)==0X03)
	{
		Rc_D.THROTTLE = (vs16)(*(data_buf+4)<<8)|*(data_buf+5);
		Rc_D.YAW = (vs16)(*(data_buf+6)<<8)|*(data_buf+7);
		Rc_D.ROLL = (vs16)(*(data_buf+8)<<8)|*(data_buf+9);
		Rc_D.PITCH = (vs16)(*(data_buf+10)<<8)|*(data_buf+11);
		Rc_D.AUX1 = (vs16)(*(data_buf+12)<<8)|*(data_buf+13);
		Rc_D.AUX2 = (vs16)(*(data_buf+14)<<8)|*(data_buf+15);
		Rc_D.AUX3 = (vs16)(*(data_buf+16)<<8)|*(data_buf+17);
		Rc_D.AUX4 = (vs16)(*(data_buf+18)<<8)|*(data_buf+19);
		Rc_D.AUX5 = (vs16)(*(data_buf+20)<<8)|*(data_buf+21);
		Rc_D.AUX6 = (vs16)(*(data_buf+21)<<8)|*(data_buf+22);
		Rc_Fun(&Rc_D,&Rc_C);
	}
#endif
	if(*(data_buf+2)==0X10)								//PID1
	{
			PID_ROL.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_ROL.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/1000;
			PID_ROL.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			PID_PIT.P = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11))/100;
			PID_PIT.I = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13))/1000;
			PID_PIT.D = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15))/100;
			PID_YAW.P = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17))/100;
			PID_YAW.I = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19))/100;
			PID_YAW.D = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(sum);
	}
	if(*(data_buf+2)==0X11)								//PID2
	{
			PID_ALT.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_ALT.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/100;
			PID_ALT.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			PID_POS.P = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11))/100;
			PID_POS.I = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13))/100;
			PID_POS.D = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15))/100;
			PID_PID_1.P = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17))/100;
			PID_PID_1.I = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19))/100;
			PID_PID_1.D = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(sum);
	}
	if(*(data_buf+2)==0X12)								//PID3
	{
			PID_PID_2.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_PID_2.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/100;
			PID_PID_2.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			PID_PID_3.P = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11))/100;
			PID_PID_3.I = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13))/100;
			PID_PID_3.D = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15))/100;
			PID_PID_4.P = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17))/100;
			PID_PID_4.I = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19))/100;
			PID_PID_4.D = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(sum);
	}
	if(*(data_buf+2)==0X13)								//PID4
	{
			PID_PID_5.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_PID_5.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/100;
			PID_PID_5.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			PID_PID_6.P = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11))/100;
			PID_PID_6.I = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13))/100;
			PID_PID_6.D = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15))/100;
			PID_PID_7.P = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17))/100;
			PID_PID_7.I = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19))/100;
			PID_PID_7.D = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(sum);
	}
	if(*(data_buf+2)==0X14)								//PID5
	{
			PID_PID_8.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_PID_8.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/100;
			PID_PID_8.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			PID_PID_9.P = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11))/100;
			PID_PID_9.I = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13))/100;
			PID_PID_9.D = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15))/100;
			PID_PID_10.P = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17))/100;
			PID_PID_10.I = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19))/100;
			PID_PID_10.D = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(sum);
	}
	if(*(data_buf+2)==0X15)								//PID6
	{
			PID_PID_11.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_PID_11.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/100;
			PID_PID_11.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			PID_PID_12.P = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11))/100;
			PID_PID_12.I = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13))/100;
			PID_PID_12.D = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15))/100;
			Data_Send_Check(sum);
			EE_SAVE_PID();
	}
	if(*(data_buf+2)==0X16)								//OFFSET
	{
			AngleOffset_Rol = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/1000;
			AngleOffset_Pit = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/1000;
	}
}
*/


//NRF24L01
//Exmaple:


//CH Kinetis¹Ì¼ş¿â V2.50 °æ±¾
// ĞŞ¸ÄÖ÷Æµ ÇëĞŞ¸Ä CMSIS±ê×¼ÎÄ¼ş system_MKxxxx.c ÖĞµÄ CLOCK_SETUP ºê 
 

/*     ÊµÑéÃû³Æ£ºNRF2401ÊµÑé
     ÊµÑéÆ½Ì¨£º¶ÉÑ»¿ª·¢°å
     °åÔØĞ¾Æ¬£ºMK60DN512ZVQ10
 ÊµÑéĞ§¹û£ºÍ¨¹ı´®¿ÚÊµÏÖÎŞÏßÊı¾İµÄÊÕ·¢¹¦ÄÜ£¬
      ÎŞÏßÍ¨ĞÅÖ§³ÖµÄÊÇNRF24L01Ä£¿é
    ×¢Òâ£ºµ×²ãÇı¶¯µÄ±àĞ´Õë¶Ô2.4GµÄusbÎŞÏßÄ£¿é±àĞ´£¬ĞèÒªÅäºÏUSB-2.4GÄ£¿éÊ¹ÓÃ 
      TX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01}; //·¢ËÍµØÖ·
      RX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01}; //½ÓÊÕµØÖ·    
	*/		


/*extern int kinetis_spi_bus_init(struct spi_bus* bus, uint32_t instance);
static uint8_t NRF2401RXBuffer[32] = "HelloWorld\r\n";//ÎŞÏß½ÓÊÕÊı¾İ
static uint8_t* gpRevChar;

// ´®¿Ú½ÓÊÕÖĞ¶Ï 
void UART_ISR(uint16_t ch)
{
    static uint8_t rev_ch;
    rev_ch = ch;
    gpRevChar = (uint8_t*)&rev_ch;
}

int main(void)
{
    uint32_t i;
    uint32_t len;
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    UART_CallbackRxInstall(HW_UART0, UART_ISR);
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);
    
    printf("NRF24L01 test\r\n");
    // ³õÊ¼»¯ NRF2401Ä£¿é µÄSPI½Ó¿Ú¼°Æ¬Ñ¡ 
    PORT_PinMuxConfig(HW_GPIOE, 1, kPinAlt2); 
    PORT_PinMuxConfig(HW_GPIOE, 2, kPinAlt2); 
    PORT_PinMuxConfig(HW_GPIOE, 3, kPinAlt2); 
    PORT_PinMuxConfig(HW_GPIOE, 4, kPinAlt2);
    //³õÊ¼»¯2401ËùĞèµÄCEÒı½Å 
    GPIO_QuickInit(HW_GPIOE, 0 , kGPIO_Mode_OPP);
    //³õÊ¼»¯2401Ä£¿é
    static struct spi_bus bus;
    kinetis_spi_bus_init(&bus, HW_SPI1);
    nrf24l01_init(&bus, 0);
    //¼ì²âÊÇ·ñ´æÔÚÎŞÏßÉè±¸£¬²¢ÅäÖÃ½ÓÊÕºÍ·¢ËÍµØÖ·
    if(nrf24l01_probe())
    {
        printf("no nrf24l01 device found!\r\n");
    }
    // ½øÈëRxÄ£Ê½ 
    nrf24l01_set_rx_mode();
    while(1)
    {
        // Èç¹ûÊÕµ½´®¿ÚÊı¾İÔò·¢ËÍ 
        if(gpRevChar != NULL)
        {
            nrf24l01_set_tx_mode();
            nrf24l01_write_packet(gpRevChar, 1);
            nrf24l01_set_rx_mode();
            gpRevChar = NULL;
        }
        // Èç¹ûÊÕµ½2401 µÄÊı¾İ Ôò´«Êäµ½´®¿Ú 
        if(!nrf24l01_read_packet(NRF2401RXBuffer, &len))
        {
            i = 0;
            while(len--)
            {
                UART_WriteByte(HW_UART0, NRF2401RXBuffer[i++]);
            }
        }
    }
}
*/
