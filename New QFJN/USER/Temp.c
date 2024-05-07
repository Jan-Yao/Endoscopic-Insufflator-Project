#include "Temp.h"

/****************************************************************************
delay_us
****************************************************************************/
void delay_us(uint32_t time)
{
  time *= 10;
	while(time)
		time--;
}

/****************************************************************************
DS18B20_IO_IN
****************************************************************************/
void DS18B20_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = Temp_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(Temp_GPIO_Port,&GPIO_InitStructure);
}


/****************************************************************************
DS18B20_IO_OUT
****************************************************************************/
void DS18B20_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = Temp_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(Temp_GPIO_Port,&GPIO_InitStructure);
}


/***************************************************************************
DS18B20_Rst
***************************************************************************/
void DS18B20_Rst(void)
{
	DS18B20_IO_OUT();//SET PB7 OUTPUT
	
	////拉低DQ 750us
	DS18B20_DQ_OUT_LOW;
	delay_us(750);     
	
	//wait 15~60us
	DS18B20_DQ_OUT_HIGH;
	delay_us(15);
}
	

/***************************************************************************
DS18B20_Check
2:Fail
1:未检测到DS18B20_CH1的存在
0:存在
***************************************************************************/
uint8_t DS18B20_Check(void)
{
	uint8_t retry = 0;

	DS18B20_IO_IN();
	while(DS18B20_DQ_IN && retry < 200)
	{
		retry++;
		delay_us(1);
	}
	
	if(retry >= 200)
		return 1;
	else
		retry = 0;
	
	while(!DS18B20_DQ_IN && retry < 240)
	{
		retry++;
		delay_us(1);
	}
	
	if(retry >= 240)
		return 2;
	
	return 0;
}

/***************************************************************************
DS18B20_Write_Byte
***************************************************************************/
void DS18B20_Write_Byte(uint8_t data)
{
	uint8_t j;
	uint8_t databit;
	DS18B20_IO_OUT();
	for(j=1;j<=8;j++)
	{
		databit=data&0x01;
		data=data>>1;     
		if(databit)
		{      
			DS18B20_DQ_OUT_LOW;
			delay_us(2);
			DS18B20_DQ_OUT_HIGH;
			delay_us(60);
		}
		else
		{          
			DS18B20_DQ_OUT_LOW;
			delay_us(60);
			DS18B20_DQ_OUT_HIGH;
			delay_us(2);
		}
	}
}

/***************************************************************************
DS18B20_Read_Bit
***************************************************************************/
uint8_t DS18B20_Read_Bit(void)
{
	uint8_t data;
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT_LOW;
	delay_us(2);
	DS18B20_DQ_OUT_HIGH;
	DS18B20_IO_IN();
	delay_us(12);
	
	if(DS18B20_DQ_IN)
		data = 1;
	else
		data = 0;
	
	delay_us(50);
	return data;
}


/***************************************************************************
DS18B20_Read_Byte
***************************************************************************/
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i,j,data;
	data = 0;
	for(i=1;i<=8;i++)
  {
		j = DS18B20_Read_Bit();
		data = (j<<7)|(data>>1);
	}
	return data;
}

/***************************************************************************
DS18B20_Start
***************************************************************************/
void DS18B20_Start(void)
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);//Skip ROM
	DS18B20_Write_Byte(0x44);
}


/***************************************************************************
DS18B20_Init
***************************************************************************/
uint8_t DS18B20_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.Pin = Temp_Pin;
//	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStructure.Pull = GPIO_PULLUP;
//	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(Temp_GPIO_Port,&GPIO_InitStructure);
	
	DS18B20_Rst();
	return DS18B20_Check();
}

/***************************************************************************
DS18B20_Read_Temperature
***************************************************************************/
short DS18B20_Get_Temperature(void)
{
	uint8_t temp;
	uint8_t TL,TH;
	short temperature;
	
	DS18B20_Start();
	DS18B20_Rst();
	DS18B20_Check();
  DS18B20_Write_Byte(0xcc);//Skip ROM
	DS18B20_Write_Byte(0xbe);
	TL = DS18B20_Read_Byte();//LSB
	TH = DS18B20_Read_Byte();//MSB
	
	//温度为负
	if(TH > 0x07)
	{
		TH = ~TH;
		TL = ~TL;
		temp = 0;
	}
	else
	{
		temp = 1;
	}
	
	temperature = TH;
	temperature <<= 8;
	temperature += TL;
	temperature = (float)temperature*0.625;
	if(temp)
		return temperature;
	else
		return -temperature;
}
