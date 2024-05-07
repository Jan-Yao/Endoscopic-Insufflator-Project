/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

#include "usart.h"

#include "Constant.h"
#include "Type.h"
#include "Prototype.h"
#include "Variable.h"

#define EEPROM_Write 0xA0
#define EEPROM_Read 0xA1
/* USER CODE END 0 */

I2C_HandleTypeDef hi2c2;

/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = EEPROM_SCL_Pin|EEPROM_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(EEPROM_SCL_GPIO_Port, EEPROM_SCL_Pin);

    HAL_GPIO_DeInit(EEPROM_SDA_GPIO_Port, EEPROM_SDA_Pin);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void EEPROM_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{
	HAL_I2C_Mem_Write(&hi2c2,EEPROM_Write,WriteAddr,I2C_MEMADD_SIZE_8BIT,&DataToWrite,1,10);      //3 Byte + for wait = 24 bit  time: 1/100000*24 =  0.24ms
}

void EEPROM_WriteFloat(uint16_t WriteAddr,float DataToWrite)
{
	uint16_t Data_Write = 0;
	uint8_t Data[2];
	
	Data_Write = DataToWrite*10;
	Data[0] = Data_Write >> 8;
	Data[1] = Data_Write;
	
	for(uint8_t i=0;i<2;i++)
	{
		HAL_I2C_Mem_Write(&hi2c2,EEPROM_Write,WriteAddr+i,I2C_MEMADD_SIZE_8BIT,&Data[i],1,10);
		DelayMS(100); 
	}
}

void EEPROM_Write_PWM(uint16_t WriteAddr)
{
	uint8_t Data[122];
	for(uint8_t i=0;i<61;i++)
	{
		if(i == 0)
		{
			Data[i*2] = 0;
		}
		else
		{
			Data[i*2] = PWM_buffer[i] >> 8;
		}
		HAL_I2C_Mem_Write(&hi2c2,EEPROM_Write,WriteAddr+(i*2),I2C_MEMADD_SIZE_8BIT,&Data[i*2],1,10); 
		DelayMS(100);
    if(i == 0)
		{
			Data[i*2+1] = 0;
		}
		else
		{
			Data[i*2+1] = PWM_buffer[i];
		}		
		HAL_I2C_Mem_Write(&hi2c2,EEPROM_Write,WriteAddr+(i*2+1),I2C_MEMADD_SIZE_8BIT,&Data[i*2+1],1,10); 
		DelayMS(100); 
	}     
}

void EEPROM_Write_PWM_Data(uint16_t WriteAddr,uint16_t DataToWrite,uint8_t size)
{
	uint8_t Data[2];
	Data[0] = DataToWrite >> 8;
	Data[1] = (uint8_t)DataToWrite;
	HAL_I2C_Mem_Write(&hi2c2,EEPROM_Write,WriteAddr+(size*2),I2C_MEMADD_SIZE_8BIT,&Data[0],1,10); 
	DelayMS(100); 
	HAL_I2C_Mem_Write(&hi2c2,EEPROM_Write,WriteAddr+(size*2+1),I2C_MEMADD_SIZE_8BIT,&Data[1],1,10); 
	DelayMS(100);    
}

uint8_t EEPROM_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t DataToRead = 0;
	
	if(HAL_I2C_Mem_Read(&hi2c2,EEPROM_Read,ReadAddr,I2C_MEMADD_SIZE_8BIT,&DataToRead,1,10) == HAL_OK)
	{
		return DataToRead;
	}
	else
	{
		return 0;
	}
}

float EEPROM_ReadFloat(uint16_t ReadAddr)
{
	uint8_t Data[2];
	uint16_t Data_Read = 0;
	float Data_Float = 0;
	
	
	for(uint8_t i=0;i<2;i++)
	{
		HAL_I2C_Mem_Read(&hi2c2,EEPROM_Read,ReadAddr+i,I2C_MEMADD_SIZE_8BIT,&Data[i],1,10);
	}
	Data_Read = Data[0];
	Data_Read = Data_Read << 8;
	Data_Read = Data_Read + Data[1];
	
	Data_Float = (float)Data_Read/10;
	
	return Data_Float;
}

void EEPROM_Read_PWM(uint16_t ReadAddr)
{
	uint8_t Data[122];
	for(uint8_t i=0;i<61;i++)
	{
		HAL_I2C_Mem_Read(&hi2c2,EEPROM_Read,ReadAddr+(i*2),I2C_MEMADD_SIZE_8BIT,&Data[i*2],1,10);  
		PWM_buffer[i] = Data[i*2];
		HAL_I2C_Mem_Read(&hi2c2,EEPROM_Read,ReadAddr+(i*2+1),I2C_MEMADD_SIZE_8BIT,&Data[i*2+1],1,10);  
		PWM_buffer[i] = PWM_buffer[i] << 8;
		PWM_buffer[i] = PWM_buffer[i] + Data[i*2+1];
	}	
}

void EEPROM_Data_Read(void)
{
	if(EEPROM_ReadOneByte(0x00) == 0xAA)                                            //Read EEPROM
	{
		uPre_Value.SetH = EEPROM_ReadOneByte(0x10);                                         //Read Pre
		uFlo_Value.SetF = EEPROM_ReadFloat(0x20);                                           //Read Flo
		EEPROM_Read_PWM(0x30);
	}
	else
	{
		EEPROM_WriteOneByte(0x00,0xAA);
		DelayMS(100);                                                                 //wait 5ms to write       can use    RCC_Delay(5)  HAL_Delay(4)
		uPre_Value.SetH = 0x05;
		EEPROM_WriteOneByte(0x10,uPre_Value.SetH);                                          //Pre Default value = 5
		DelayMS(100);                                                                 //wait 5ms to write       can use    RCC_Delay(5)  HAL_Delay(4)
		uFlo_Value.SetF = 0x01;
		EEPROM_WriteFloat(0x20,uFlo_Value.SetF);                                            //Flo Default value = 1
//	  PWM_buffer_Init();
		EEPROM_Write_PWM(0x30);
	}
}

void EEPROM_Read_PWM_Debug(uint8_t Flag)
{
	uint8_t Data[128];
	int16_t P_Data,F_Data;
	int16_t Buffer[61];
	
	if(Flag == 0)
	{
		P_Data = 0 - EEPROM_ReadOneByte(0x10);                                          //Read Pre
	  F_Data = 0 - EEPROM_ReadOneByte(0x20);                                          //Read Flo
	}
	else
	{
		P_Data = EEPROM_ReadOneByte(0x10);                                          //Read Pre
	  F_Data = EEPROM_ReadOneByte(0x20);                                          //Read Flo
	}
	
	for(uint8_t i=0;i<61;i++)                                                       //Read
	{
		HAL_I2C_Mem_Read(&hi2c2,EEPROM_Read,0x30+(i*2),I2C_MEMADD_SIZE_8BIT,&Data[i*2],1,10);  
		Buffer[i] = Data[i*2];
		HAL_I2C_Mem_Read(&hi2c2,EEPROM_Read,0x30+(i*2+1),I2C_MEMADD_SIZE_8BIT,&Data[i*2+1],1,10);  
		Buffer[i] = Buffer[i] << 8;
		Buffer[i] = Buffer[i] + Data[i*2+1];
		if(Flag == 0)
		{
			Buffer[i] = 0 - Buffer[i];
		}
	}
	
  for(uint8_t i=0;i<61;i++)                                                       //·¢ËÍ >0 Green <0 White
	{
		Data[i*2]   = Buffer[i] >> 8;
		Data[i*2+1] = Buffer[i];
	}
	Data[122] = P_Data >> 8;
	Data[123] = P_Data;
  Data[124] = F_Data >> 8;	
	Data[125] = F_Data;
	Data[126] = 0x03;
	Data[127] = 0xE9;                                                         // 1001 finsh send
	SendBuffer(Data,128);
}

/* USER CODE END 1 */
