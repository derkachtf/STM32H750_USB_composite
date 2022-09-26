/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

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
#define VP5_ON_Pin GPIO_PIN_13
#define VP5_ON_GPIO_Port GPIOC
#define MOT2_Pin GPIO_PIN_11
#define MOT2_GPIO_Port GPIOE
#define MOT3_Pin GPIO_PIN_13
#define MOT3_GPIO_Port GPIOE
#define MOT1_Pin GPIO_PIN_14
#define MOT1_GPIO_Port GPIOE
#define ON5VP2_Pin GPIO_PIN_8
#define ON5VP2_GPIO_Port GPIOD
#define DUFP2_Pin GPIO_PIN_9
#define DUFP2_GPIO_Port GPIOD
#define DUFP1_Pin GPIO_PIN_9
#define DUFP1_GPIO_Port GPIOC
#define ON5VP1_Pin GPIO_PIN_8
#define ON5VP1_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
