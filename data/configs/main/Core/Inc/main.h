/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define IGNITOR_EN_Pin GPIO_PIN_13
#define IGNITOR_EN_GPIO_Port GPIOC
#define SHORT_DETECT_Pin GPIO_PIN_3
#define SHORT_DETECT_GPIO_Port GPIOA
#define SENSE_4_Pin GPIO_PIN_12
#define SENSE_4_GPIO_Port GPIOB
#define FIRE_4_Pin GPIO_PIN_13
#define FIRE_4_GPIO_Port GPIOB
#define FIRE_3_Pin GPIO_PIN_14
#define FIRE_3_GPIO_Port GPIOB
#define SENSE_3_Pin GPIO_PIN_15
#define SENSE_3_GPIO_Port GPIOB
#define SENSE_2_Pin GPIO_PIN_8
#define SENSE_2_GPIO_Port GPIOA
#define FIRE_2_Pin GPIO_PIN_9
#define FIRE_2_GPIO_Port GPIOA
#define FIRE_1_Pin GPIO_PIN_10
#define FIRE_1_GPIO_Port GPIOA
#define SENSE_1_Pin GPIO_PIN_11
#define SENSE_1_GPIO_Port GPIOA
#define IGNITOR_CTRL_Pin GPIO_PIN_12
#define IGNITOR_CTRL_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
