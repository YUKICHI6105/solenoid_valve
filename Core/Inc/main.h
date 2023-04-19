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
void main_cpp(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EMS_Pin GPIO_PIN_13
#define EMS_GPIO_Port GPIOC
#define EMS_EXTI_IRQn EXTI15_10_IRQn
#define CAN_LED_Pin GPIO_PIN_14
#define CAN_LED_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_6
#define LED_GREEN_GPIO_Port GPIOA
#define LED_YELLOW_Pin GPIO_PIN_7
#define LED_YELLOW_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_0
#define LED_RED_GPIO_Port GPIOB
#define V0_Pin GPIO_PIN_10
#define V0_GPIO_Port GPIOB
#define V1_Pin GPIO_PIN_11
#define V1_GPIO_Port GPIOB
#define V2_Pin GPIO_PIN_12
#define V2_GPIO_Port GPIOB
#define V3_Pin GPIO_PIN_13
#define V3_GPIO_Port GPIOB
#define V4_Pin GPIO_PIN_14
#define V4_GPIO_Port GPIOB
#define V5_Pin GPIO_PIN_15
#define V5_GPIO_Port GPIOB
#define V6_Pin GPIO_PIN_8
#define V6_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
