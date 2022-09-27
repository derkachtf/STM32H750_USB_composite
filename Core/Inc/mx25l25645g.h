/**
  ******************************************************************************
  * @file    QSPI/QSPI_ReadWrite/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MX25L25645G_H
#define __MX25L25645G_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* MX25L51245G Macronix memory */
#define QSPI_FLASH_SIZE                  0x2000000 /* 256 MBits => 32MBytes */
#define QSPI_SECTOR_SIZE                 0x10000   /* 512 sectors of 64KBytes */
#define QSPI_SUBSECTOR_SIZE              0x1000    /* 8192 subsectors of 4kBytes */
#define QSPI_PAGE_SIZE                   0x100     /* 131072 pages of 256 bytes */
#define QSPI_NUM_SUBSECTOR               8192      // 8192 subsectors 32MBytes

void test1_mx25l25645g(void);
void test2_mx25l25645g(void);
void test3_mx25l25645g(void);
uint32_t erase_mx25l25645g(void);

uint8_t QSPI_Init(void);
uint8_t QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
uint8_t QSPI_Erase_Block(uint32_t BlockAddress);

#endif /* __MX25L25645G_H */

