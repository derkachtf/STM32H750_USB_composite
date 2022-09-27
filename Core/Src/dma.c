/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
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

/* Includes ------------------------------------------------------------------*/
#include "dma.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, IRQ_PP_DMA1_ST0, IRQ_SP_DMA1_ST0);
  //HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);//преривания DMA1_Stream0_IRQn ws2812 ненужно
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, IRQ_PP_DMA1_ST1, IRQ_SP_DMA1_ST1);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn); // DAC
  /* DMA1_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, IRQ_PP_DMA1_ST2, IRQ_SP_DMA1_ST2);
  //HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn); // ADC3
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, IRQ_PP_DMA1_ST3, IRQ_SP_DMA1_ST3);
  //HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn); // ADC2
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, IRQ_PP_DMA1_ST4, IRQ_SP_DMA1_ST4);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);  // USART1 nRF52805
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, IRQ_PP_DMA1_ST5, IRQ_SP_DMA1_ST5);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);  // USART2 TX ESP8266
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, IRQ_PP_DMA1_ST6, IRQ_SP_DMA1_ST6);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn); // I2C1_TX
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, IRQ_PP_DMA1_ST7, IRQ_SP_DMA1_ST7);
  //HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn); // ADC1
      /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, IRQ_PP_DMA2_ST0, IRQ_SP_DMA2_ST0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn); // USART2 RX ESP8266
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, IRQ_PP_DMA2_ST1, IRQ_SP_DMA2_ST1);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);  // прерывание MIC
    // HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, IRQ_PP_DMA2_ST2, IRQ_SP_DMA2_ST2);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn); // usart8 debug
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, IRQ_PP_DMA2_ST3, IRQ_SP_DMA2_ST3);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn); // SPI3 TX to display
  /* DMA2_Stream6_IRQn interrupt configuration */
  // HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, IRQ_PP_DMA2_ST6, IRQ_SP_DMA2_ST6);
  // HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  // HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, IRQ_PP_DMA2_ST7, IRQ_SP_DMA2_ST7);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

