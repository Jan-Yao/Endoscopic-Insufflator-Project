/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_TEST_Pin GPIO_PIN_13
#define LED_TEST_GPIO_Port GPIOC
#define Speak_PWM_Pin GPIO_PIN_14
#define Speak_PWM_GPIO_Port GPIOC
#define Speak_Pin GPIO_PIN_15
#define Speak_GPIO_Port GPIOC
#define ADC_Pre_Pin GPIO_PIN_1
#define ADC_Pre_GPIO_Port GPIOC
#define ADC_Flo_Pin GPIO_PIN_3
#define ADC_Flo_GPIO_Port GPIOC
#define Heart_PWM_Pin GPIO_PIN_0
#define Heart_PWM_GPIO_Port GPIOA
#define Control_Guanduan_Pin GPIO_PIN_1
#define Control_Guanduan_GPIO_Port GPIOA
#define LCD_TX_Pin GPIO_PIN_2
#define LCD_TX_GPIO_Port GPIOA
#define LCD_RX_Pin GPIO_PIN_3
#define LCD_RX_GPIO_Port GPIOA
#define ADC_Cur_Pin GPIO_PIN_4
#define ADC_Cur_GPIO_Port GPIOA
#define BILI_IN_PWM_Pin GPIO_PIN_6
#define BILI_IN_PWM_GPIO_Port GPIOA
#define ADC_Temp_1_Pin GPIO_PIN_4
#define ADC_Temp_1_GPIO_Port GPIOC
#define ADC_Temp_2_Pin GPIO_PIN_5
#define ADC_Temp_2_GPIO_Port GPIOC
#define EEPROM_SCL_Pin GPIO_PIN_10
#define EEPROM_SCL_GPIO_Port GPIOB
#define EEPROM_SDA_Pin GPIO_PIN_11
#define EEPROM_SDA_GPIO_Port GPIOB
#define LCD_BUSY_Pin GPIO_PIN_14
#define LCD_BUSY_GPIO_Port GPIOB
#define BILI_OUT_PWM_Pin GPIO_PIN_6
#define BILI_OUT_PWM_GPIO_Port GPIOC
#define Control_Paiyan_Pin GPIO_PIN_7
#define Control_Paiyan_GPIO_Port GPIOC
#define Input_YeTi_Pin GPIO_PIN_8
#define Input_YeTi_GPIO_Port GPIOA
#define QIBENG_PWM_Pin GPIO_PIN_9
#define QIBENG_PWM_GPIO_Port GPIOA
#define Debug_TX_Pin GPIO_PIN_10
#define Debug_TX_GPIO_Port GPIOC
#define Debug_RX_Pin GPIO_PIN_11
#define Debug_RX_GPIO_Port GPIOC
#define BUZ_Pin GPIO_PIN_5
#define BUZ_GPIO_Port GPIOB
#define Music_PWM_Pin GPIO_PIN_6
#define Music_PWM_GPIO_Port GPIOB
#define Temp_Pin GPIO_PIN_7
#define Temp_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
