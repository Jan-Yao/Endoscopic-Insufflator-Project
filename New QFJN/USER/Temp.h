#ifndef __TEMP_H
#define __TEMP_H

#include "main.h"

#define  DS18B20_DQ_OUT_HIGH  HAL_GPIO_WritePin(Temp_GPIO_Port, Temp_Pin, GPIO_PIN_SET)
#define  DS18B20_DQ_OUT_LOW   HAL_GPIO_WritePin(Temp_GPIO_Port, Temp_Pin, GPIO_PIN_RESET)
#define  DS18B20_DQ_IN        HAL_GPIO_ReadPin(Temp_GPIO_Port, Temp_Pin)

uint8_t DS18B20_Init(void);
short DS18B20_Get_Temperature(void);

#endif
