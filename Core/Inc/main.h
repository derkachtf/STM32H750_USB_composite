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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

#define HW_VER "v1.5"
#define SW_VER_MAIN "v1.0"
#define SW_VER_LOADER "v1.0"
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
extern void assert_failed(char *file, uint32_t line, uint32_t error_code);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NFC_IRQ_Pin GPIO_PIN_2
#define NFC_IRQ_GPIO_Port GPIOE
#define NFC_IRQ_EXTI_IRQn EXTI2_IRQn
#define RM1_Pin GPIO_PIN_3
#define RM1_GPIO_Port GPIOE
#define RM2_Pin GPIO_PIN_4
#define RM2_GPIO_Port GPIOE
#define RM3_Pin GPIO_PIN_5
#define RM3_GPIO_Port GPIOE
#define CPU_WS2812_Pin GPIO_PIN_6
#define CPU_WS2812_GPIO_Port GPIOE
#define VP5_ON_Pin GPIO_PIN_13
#define VP5_ON_GPIO_Port GPIOC
#define RM4_Pin GPIO_PIN_0
#define RM4_GPIO_Port GPIOC
#define I2S_DOUT_Pin GPIO_PIN_1
#define I2S_DOUT_GPIO_Port GPIOC
#define JOY_ID_Pin GPIO_PIN_3
#define JOY_ID_GPIO_Port GPIOC
#define VADC_GAS4_Pin GPIO_PIN_0
#define VADC_GAS4_GPIO_Port GPIOA
#define VADC_GAS5_Pin GPIO_PIN_1
#define VADC_GAS5_GPIO_Port GPIOA
#define VADC_GAS6_Pin GPIO_PIN_2
#define VADC_GAS6_GPIO_Port GPIOA
#define VADC_GAS8_Pin GPIO_PIN_3
#define VADC_GAS8_GPIO_Port GPIOA
#define AUD_ON_Pin GPIO_PIN_4
#define AUD_ON_GPIO_Port GPIOA
#define AUD_DAC_Pin GPIO_PIN_5
#define AUD_DAC_GPIO_Port GPIOA
#define VADC_P1_Pin GPIO_PIN_6
#define VADC_P1_GPIO_Port GPIOA
#define VADC_P2_Pin GPIO_PIN_7
#define VADC_P2_GPIO_Port GPIOA
#define HG_LED_Pin GPIO_PIN_4
#define HG_LED_GPIO_Port GPIOC
#define HG_INM_Pin GPIO_PIN_5
#define HG_INM_GPIO_Port GPIOC
#define VADC_JX_Pin GPIO_PIN_0
#define VADC_JX_GPIO_Port GPIOB
#define VADC_JY_Pin GPIO_PIN_1
#define VADC_JY_GPIO_Port GPIOB
#define QSPI_CLK_Pin GPIO_PIN_2
#define QSPI_CLK_GPIO_Port GPIOB
#define QSPI_IO0_Pin GPIO_PIN_7
#define QSPI_IO0_GPIO_Port GPIOE
#define QSPI_IO1_Pin GPIO_PIN_8
#define QSPI_IO1_GPIO_Port GPIOE
#define QSPI_IO2_Pin GPIO_PIN_9
#define QSPI_IO2_GPIO_Port GPIOE
#define QSPI_IO3_Pin GPIO_PIN_10
#define QSPI_IO3_GPIO_Port GPIOE
#define MOT2_Pin GPIO_PIN_11
#define MOT2_GPIO_Port GPIOE
#define CM7_Pin GPIO_PIN_12
#define CM7_GPIO_Port GPIOE
#define MOT3_Pin GPIO_PIN_13
#define MOT3_GPIO_Port GPIOE
#define MOT1_Pin GPIO_PIN_14
#define MOT1_GPIO_Port GPIOE
#define CM8_Pin GPIO_PIN_15
#define CM8_GPIO_Port GPIOE
#define I2S_CLK_Pin GPIO_PIN_10
#define I2S_CLK_GPIO_Port GPIOB
#define CM9_Pin GPIO_PIN_11
#define CM9_GPIO_Port GPIOB
#define I2S_WS_Pin GPIO_PIN_12
#define I2S_WS_GPIO_Port GPIOB
#define CM10_Pin GPIO_PIN_13
#define CM10_GPIO_Port GPIOB
#define ON5VP2_Pin GPIO_PIN_8
#define ON5VP2_GPIO_Port GPIOD
#define DUFP2_Pin GPIO_PIN_9
#define DUFP2_GPIO_Port GPIOD
#define RM6_Pin GPIO_PIN_10
#define RM6_GPIO_Port GPIOD
#define EXT_IO_INT_Pin GPIO_PIN_11
#define EXT_IO_INT_GPIO_Port GPIOD
#define EXT_IO_INT_EXTI_IRQn EXTI15_10_IRQn
#define NRF_SWDCLK_Pin GPIO_PIN_14
#define NRF_SWDCLK_GPIO_Port GPIOD
#define NRF_SWDIO_Pin GPIO_PIN_15
#define NRF_SWDIO_GPIO_Port GPIOD
#define E2A_Pin GPIO_PIN_6
#define E2A_GPIO_Port GPIOC
#define E2B_Pin GPIO_PIN_7
#define E2B_GPIO_Port GPIOC
#define CM1_Pin GPIO_PIN_8
#define CM1_GPIO_Port GPIOC
#define DUFP1_Pin GPIO_PIN_9
#define DUFP1_GPIO_Port GPIOC
#define ON5VP1_Pin GPIO_PIN_8
#define ON5VP1_GPIO_Port GPIOA
#define NRF_RX_Pin GPIO_PIN_9
#define NRF_RX_GPIO_Port GPIOA
#define NRF_TX_Pin GPIO_PIN_10
#define NRF_TX_GPIO_Port GPIOA
#define NFC_NSS_Pin GPIO_PIN_15
#define NFC_NSS_GPIO_Port GPIOA
#define HG_SCL_Pin GPIO_PIN_10
#define HG_SCL_GPIO_Port GPIOC
#define QSPI_CS_Pin GPIO_PIN_11
#define QSPI_CS_GPIO_Port GPIOC
#define HG_SDA_Pin GPIO_PIN_12
#define HG_SDA_GPIO_Port GPIOC
#define HG_DC_Pin GPIO_PIN_0
#define HG_DC_GPIO_Port GPIOD
#define CM4_Pin GPIO_PIN_1
#define CM4_GPIO_Port GPIOD
#define CM3_Pin GPIO_PIN_2
#define CM3_GPIO_Port GPIOD
#define CM5_Pin GPIO_PIN_3
#define CM5_GPIO_Port GPIOD
#define CM6_Pin GPIO_PIN_4
#define CM6_GPIO_Port GPIOD
#define ESP_RX_Pin GPIO_PIN_5
#define ESP_RX_GPIO_Port GPIOD
#define ESP_TX_Pin GPIO_PIN_6
#define ESP_TX_GPIO_Port GPIOD
#define NFC_MOSI_Pin GPIO_PIN_7
#define NFC_MOSI_GPIO_Port GPIOD
#define NFC_SCK_Pin GPIO_PIN_3
#define NFC_SCK_GPIO_Port GPIOB
#define NFC_MISO_Pin GPIO_PIN_4
#define NFC_MISO_GPIO_Port GPIOB
#define CM2_Pin GPIO_PIN_5
#define CM2_GPIO_Port GPIOB
#define E1A_Pin GPIO_PIN_6
#define E1A_GPIO_Port GPIOB
#define E1B_Pin GPIO_PIN_7
#define E1B_GPIO_Port GPIOB
#define RM5_Pin GPIO_PIN_0
#define RM5_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

//HAL - TIM2 IRQ pp priority = 0

// IRQ used (I2S rx MIC)
#define IRQ_PP_DMA2_ST1       5
#define IRQ_SP_DMA2_ST1       0

// IRQ used (DAC)
#define IRQ_PP_DMA1_ST1       6
#define IRQ_SP_DMA1_ST1       0

// IRQ used (USB UP)
#define IRQ_PP_OTGHS          7
#define IRQ_SP_OTGHS          0

// IRQ used (USB Side)
#define IRQ_PP_OTGFS          8
#define IRQ_SP_OTGFS          0

// IRQ used (ESP RX)//TODO: оптимизировать (при выкачке больших файлов из ESP важен MAX приоритет иначе ошибки)
#define IRQ_PP_USART2         5
#define IRQ_SP_USART2         0

// IRQ used (Bluetouch USART1_TX)
#define IRQ_PP_DMA1_ST4       9
#define IRQ_SP_DMA1_ST4       0

// IRQ used (WIFI USART2_TX in loader)
#define IRQ_PP_DMA1_ST5       5
#define IRQ_SP_DMA1_ST5       0

// IRQ used (NFC)
#define IRQ_PP_EXTI2          9
#define IRQ_SP_EXTI2          0

// IRQ used (MCP2307)
#define IRQ_PP_EXTI1510       9
#define IRQ_SP_EXTI1510       0

// IRQ used (BLE)
#define IRQ_PP_USART1         9
#define IRQ_SP_USART1         0

// IRQ used (matrix)
#define IRQ_PP_TIM3           9
#define IRQ_SP_TIM3           0

// IRQ used (I2C1_TX)
#define IRQ_PP_DMA1_ST6       9
#define IRQ_SP_DMA1_ST6       0

// IRQ used (I2C4_TX)
#define IRQ_PP_BDMA_CH1       9
#define IRQ_SP_BDMA_CH1       0

// IRQ used (USART2 RX ESP8266)
#define IRQ_PP_DMA2_ST0       5
#define IRQ_SP_DMA2_ST0       0

// IRQ used (DMA for SPI3 display)
#define IRQ_PP_DMA2_ST3      5
#define IRQ_SP_DMA2_ST3      0

// IRQ used (SPI3 display)
#define IRQ_PP_SPI3           5
#define IRQ_SP_SPI3           0

// IRQ used (RTC)
#define IRQ_PP_RTC            10
#define IRQ_SP_RTC            0

// IRQ used (DEBUG)
#define IRQ_PP_UART8          14
#define IRQ_SP_UART8          0

// IRQ used (DEBUG)
#define IRQ_PP_DMA2_ST2       14
#define IRQ_SP_DMA2_ST2       0

// IRQ not used (ws2812 SPI4)
#define IRQ_PP_SPI4           15
#define IRQ_SP_SPI4           0

// IRQ not used (ws2812 SPI4)
#define IRQ_PP_DMA1_ST0      15
#define IRQ_SP_DMA1_ST0      0

// IRQ not used (ADC3)
#define IRQ_PP_DMA1_ST2      15
#define IRQ_SP_DMA1_ST2      0

// IRQ not used (ADC2)
#define IRQ_PP_DMA1_ST3      15
#define IRQ_SP_DMA1_ST3      0

// IRQ not used (ADC1)
#define IRQ_PP_DMA1_ST7      15
#define IRQ_SP_DMA1_ST7      0

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
