/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define LCD_Busy_Read(void)   HAL_GPIO_ReadPin(LCD_BUSY_GPIO_Port, LCD_BUSY_Pin)              //更新标记
	
#define Input_YeTi_Read(void) HAL_GPIO_ReadPin(Input_YeTi_GPIO_Port, Input_YeTi_Pin)          //Input YeTi Read
	
#define Control_ON(void)  HAL_GPIO_WritePin(Control_Guanduan_GPIO_Port, Control_Guanduan_Pin, GPIO_PIN_SET)	  //SET Control_Guanduan_Pin
#define Control_OFF(void) HAL_GPIO_WritePin(Control_Guanduan_GPIO_Port, Control_Guanduan_Pin, GPIO_PIN_RESET)	//RESET Control_Guanduan_Pin

#define Control_Paiyan_ON(void)     HAL_GPIO_WritePin(Control_Paiyan_GPIO_Port, Control_Paiyan_Pin, GPIO_PIN_SET)	  //SET Control_Paiyan_Pin
#define Control_Paiyan_OFF(void)    HAL_GPIO_WritePin(Control_Paiyan_GPIO_Port, Control_Paiyan_Pin, GPIO_PIN_RESET)	//RESET Control_Paiyan_Pin
#define Control_Paiyan_Toggle(void) HAL_GPIO_TogglePin(Control_Paiyan_GPIO_Port,Control_Paiyan_Pin)                 //Toggle Control_Paiyan_Pin

#define LED_ON(void)      HAL_GPIO_WritePin(LED_TEST_GPIO_Port, LED_TEST_Pin, GPIO_PIN_SET)	  //SET LED
#define LED_OFF(void)     HAL_GPIO_WritePin(LED_TEST_GPIO_Port, LED_TEST_Pin, GPIO_PIN_RESET)	//RESET LED
#define LED_Toggle(void)  HAL_GPIO_TogglePin(LED_TEST_GPIO_Port,LED_TEST_Pin)                 //Toggle LED

#define BUZ_ON(void)      HAL_GPIO_WritePin(BUZ_GPIO_Port, BUZ_Pin, GPIO_PIN_SET)	            //SET BUZ
#define BUZ_OFF(void)     HAL_GPIO_WritePin(BUZ_GPIO_Port, BUZ_Pin, GPIO_PIN_RESET)	          //RESET BUZ
#define BUZ_Toggle(void)  HAL_GPIO_TogglePin(BUZ_GPIO_Port,BUZ_Pin)                           //Toggle BUZ
	
#define Speak_PWM_On()		HAL_GPIO_WritePin(Speak_PWM_GPIO_Port, Speak_PWM_Pin, GPIO_PIN_RESET)//每个音符的播放控制
#define Speak_PWM_Off()	  HAL_GPIO_WritePin(Speak_PWM_GPIO_Port, Speak_PWM_Pin, GPIO_PIN_SET)

#define Speak_On()		    HAL_GPIO_WritePin(Speak_GPIO_Port, Speak_Pin, GPIO_PIN_SET)         //开启扬声器
#define Speak_Off() 	    HAL_GPIO_WritePin(Speak_GPIO_Port, Speak_Pin, GPIO_PIN_RESET)       //关闭扬声器
#define Input_Speak_Read(void) HAL_GPIO_ReadPin(Speak_GPIO_Port, Speak_Pin)                   //Input Speak Read
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

