/**
  ******************************************************************************
  * @file    QSPI/QSPI_ReadWrite/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use QuadSPI through
  *          the STM32F7xx HAL API.
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

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "main.h"
#include "mx25l25645g.h"
#include "debug_helper.h"

//#define printf(...) ((void)0)

/* Definition for QSPI clock resources */
#define QSPI_CLK_ENABLE()            __HAL_RCC_QSPI_CLK_ENABLE()
#define QSPI_CLK_DISABLE()           __HAL_RCC_QSPI_CLK_DISABLE()

#define QSPI_CS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define QSPI_CLK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define QSPI_IO0_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define QSPI_IO1_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define QSPI_IO2_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define QSPI_IO3_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()

#define QSPI_FORCE_RESET()           __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_RELEASE_RESET()         __HAL_RCC_QSPI_RELEASE_RESET()

#define QSPI_DUMMY_CYCLES_READ           8
#define QSPI_DUMMY_CYCLES_READ_QUAD_IO   8
#define QSPI_DUMMY_CYCLES_READ_QUAD_DTR  8

#define QSPI_BULK_ERASE_MAX_TIME         600000
#define QSPI_SECTOR_ERASE_MAX_TIME       2000
#define QSPI_SUBSECTOR_ERASE_MAX_TIME    400

#define mx25l25645g_malloc         pvPortMalloc
//#define u24lc512_free           free
#define mx25l25645g_free           vPortFree
/** 
  * @brief  QSPI Commands  
  */  
/* Reset Operations */
#define RESET_ENABLE_CMD                      0x66
#define RESET_EXECUTE_CMD                     0x99

/* Identification Operations */
#define READ_ID_CMD                          0x9F
#define MULTIPLE_IO_READ_ID_CMD              0xAF
#define READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x5A

/* Read Operations */
#define READ_CMD                             0x03
#define READ_4_BYTE_ADDR_CMD                 0x13

#define FAST_READ_CMD                        0x0B
#define FAST_READ_DTR_CMD                    0x0D
#define FAST_READ_4_BYTE_ADDR_CMD            0x0C

#define DUAL_OUT_FAST_READ_CMD               0x3B
#define DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD   0x3C

#define DUAL_INOUT_FAST_READ_CMD             0xBB
#define DUAL_INOUT_FAST_READ_DTR_CMD         0xBD
#define DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0xBC

#define QUAD_OUT_FAST_READ_CMD               0x6B
#define QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD   0x6C

#define QUAD_INOUT_FAST_READ_CMD             0xEB
#define QUAD_INOUT_FAST_READ_DTR_CMD         0xED  
#define QPI_READ_4_BYTE_ADDR_CMD             0xEC

/* Write Operations */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04

/* Register Operations */
#define READ_STATUS_REG_CMD                  0x05
#define READ_CFG_REG_CMD                     0x15   
#define WRITE_STATUS_CFG_REG_CMD             0x01

#define READ_LOCK_REG_CMD                    0x2D
#define WRITE_LOCK_REG_CMD                   0x2C

#define READ_EXT_ADDR_REG_CMD                0xC8
#define WRITE_EXT_ADDR_REG_CMD               0xC5

/* Program Operations */
#define PAGE_PROG_CMD                        0x02
#define PAGE_PROG_4_BYTE_ADDR_CMD            0x12
#define QPI_PAGE_PROG_4_BYTE_ADDR_CMD        0x12  ///?

#define QUAD_IN_FAST_PROG_CMD                0x38
#define EXT_QUAD_IN_FAST_PROG_CMD            0x38 
#define QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD    0x3E

/* Erase Operations */
#define SUBSECTOR_ERASE_CMD                  0x20
#define SUBSECTOR_ERASE_4_BYTE_ADDR_CMD      0x21
   
#define SECTOR_ERASE_CMD                     0xD8
#define SECTOR_ERASE_4_BYTE_ADDR_CMD         0xDC

#define BULK_ERASE_CMD                       0xC7

#define PROG_ERASE_RESUME_CMD                0x30
#define PROG_ERASE_SUSPEND_CMD               0xB0

/* 4-byte Address Mode Operations */
#define ENTER_4_BYTE_ADDR_MODE_CMD           0xB7
#define EXIT_4_BYTE_ADDR_MODE_CMD            0xE9

/* Quad Operations */
#define ENTER_QUAD_CMD                       0x35//enable QPI
#define EXIT_QUAD_CMD                        0xF5
   
/** 
  * @brief  QSPI Registers  
  */ 
/* Status Register */
#define QSPI_SR_WIP                      ((uint8_t)0x01)    /*!< Write in progress */
#define QSPI_SR_WREN                     ((uint8_t)0x02)    /*!< Write enable latch */
#define QSPI_SR_BLOCKPR                  ((uint8_t)0x5C)    /*!< Block protected against program and erase operations */
#define QSPI_SR_PRBOTTOM                 ((uint8_t)0x20)    /*!< Protected memory area defined by BLOCKPR starts from top or bottom */
#define QSPI_SR_QUADEN                   ((uint8_t)0x40)    /*!< Quad IO mode enabled if =1 */
#define QSPI_SR_SRWREN                   ((uint8_t)0x80)    /*!< Status register write enable/disable */

/* Configuration Register */
#define QSPI_CR_ODS                      ((uint8_t)0x07)    /*!< Output driver strength */
#define QSPI_CR_ODS_30                   ((uint8_t)0x07)    /*!< Output driver strength 30 ohms (default)*/
#define QSPI_CR_ODS_15                   ((uint8_t)0x06)    /*!< Output driver strength 15 ohms */
#define QSPI_CR_ODS_20                   ((uint8_t)0x05)    /*!< Output driver strength 20 ohms */
#define QSPI_CR_ODS_45                   ((uint8_t)0x03)    /*!< Output driver strength 45 ohms */
#define QSPI_CR_ODS_60                   ((uint8_t)0x02)    /*!< Output driver strength 60 ohms */
#define QSPI_CR_ODS_90                   ((uint8_t)0x01)    /*!< Output driver strength 90 ohms */
#define QSPI_CR_TB                       ((uint8_t)0x08)    /*!< Top/Bottom bit used to configure the block protect area */
#define QSPI_CR_PBE                      ((uint8_t)0x10)    /*!< Preamble Bit Enable */
#define QSPI_CR_4BYTE                    ((uint8_t)0x20)    /*!< 3-bytes or 4-bytes addressing */
#define QSPI_CR_NB_DUMMY                 ((uint8_t)0xC0)    /*!< Number of dummy clock cycles */

#define QSPI_MANUFACTURER_ID               ((uint8_t)0xC2)
#define QSPI_DEVICE_ID_MEM_TYPE            ((uint8_t)0x20)
#define QSPI_DEVICE_ID_MEM_CAPACITY        ((uint8_t)0x1A)
#define QSPI_UNIQUE_ID_DATA_LENGTH         ((uint8_t)0x10)  /*JCC: not checked */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WRITE_READ_ADDR     ((uint32_t)0x0000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
QSPI_HandleTypeDef hqspi;
MDMA_HandleTypeDef hmdma_quadspi_fifo_th;


/* Private function prototypes -----------------------------------------------*/
static void Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);

static uint8_t QSPI_WriteEnable(void);
static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout);
static uint8_t QSPI_EnterFourBytesAddress(void);
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

inline static void QSPI_Delay(uint32_t ms)
{
  HAL_Delay(ms);
}

/**
  * @brief QSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for QSPI interrupts
  * @param hqspi: QSPI handle pointer
  * @retval None
  */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
  //PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;//240MHz
  PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_PLL; //87MHz
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the QuadSPI memory interface clock */
  QSPI_CLK_ENABLE();

  /* Reset the QuadSPI memory interface */
  QSPI_FORCE_RESET();
  QSPI_RELEASE_RESET();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**QUADSPI GPIO Configuration
    PB2     ------> QUADSPI_CLK
    PE7     ------> QUADSPI_BK2_IO0
    PE8     ------> QUADSPI_BK2_IO1
    PE9     ------> QUADSPI_BK2_IO2
    PE10     ------> QUADSPI_BK2_IO3
    PC11     ------> QUADSPI_BK2_NCS
    */
    GPIO_InitStruct.Pin = QSPI_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(QSPI_CLK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_IO0_Pin|QSPI_IO1_Pin|QSPI_IO2_Pin|QSPI_IO3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(QSPI_CS_GPIO_Port, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for QSPI #########################################*/
  /* NVIC configuration for QSPI interrupt */
  //HAL_NVIC_SetPriority(QUADSPI_IRQn, 0x05, 0);
  //HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
}

/**
  * @brief QSPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param hqspi: QSPI handle pointer
  * @retval None
  */
void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi)
{
  /*##-1- Disable the NVIC for QSPI ###########################################*/
  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-Configure QSPI pins */
    HAL_GPIO_DeInit(QSPI_CLK_GPIO_Port, QSPI_CLK_Pin);

    HAL_GPIO_DeInit(GPIOE, QSPI_IO0_Pin|QSPI_IO1_Pin|QSPI_IO2_Pin|QSPI_IO3_Pin);

    HAL_GPIO_DeInit(QSPI_CS_GPIO_Port, QSPI_CS_Pin);

  /*##-3- Reset peripherals ##################################################*/
  /* Reset the QuadSPI memory interface */
  QSPI_FORCE_RESET();
  QSPI_RELEASE_RESET();

  /* Disable the QuadSPI memory interface clock */
  QSPI_CLK_DISABLE();
}

uint8_t QSPI_ResetChip(void)
{
  QSPI_CommandTypeDef sCommand;
  uint32_t temp = 0;
  
  /* Erasing Sequence -------------------------------------------------- */
  sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction = RESET_ENABLE_CMD;
  sCommand.AddressMode = QSPI_ADDRESS_NONE;
  sCommand.Address = 0;
  sCommand.DataMode = QSPI_DATA_NONE;
  sCommand.DummyCycles = 0;

  if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) 
  {
    return HAL_ERROR;
  }
  for (temp = 0; temp < 0x2f; temp++) 
  {  //TODO:
    __NOP();
  }

  sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction = RESET_EXECUTE_CMD;
  sCommand.AddressMode = QSPI_ADDRESS_NONE;
  sCommand.Address = 0;
  sCommand.DataMode = QSPI_DATA_NONE;
  sCommand.DummyCycles = 0;

  if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) 
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

void test1_mx25l25645g(void)
{
  uint8_t qspi_aTxBuffer[QSPI_SUBSECTOR_SIZE];
  uint8_t qspi_aRxBuffer[QSPI_SUBSECTOR_SIZE];
  uint8_t rez;
  // uint32_t addr;
  // addr = (0) << 12;
  // uint8_t *p;
  
	// p = (uint8_t *)qspi_aTxBuffer;

  /*##-2- Erase QSPI memory ##################################################*/
  rez = QSPI_Erase_Block(WRITE_READ_ADDR);
  printf("QSPI_Erase_Block=%u\n",rez);

  /*##-3- QSPI memory read/write access  #####################################*/
  /* Fill the buffer to write */
  Fill_Buffer(qspi_aTxBuffer, QSPI_SUBSECTOR_SIZE, 0);

  /* Write data to the QSPI memory */
  rez = QSPI_Write(qspi_aTxBuffer, WRITE_READ_ADDR, QSPI_SUBSECTOR_SIZE);
  //rez = QSPI_Write(fat, WRITE_READ_ADDR, QSPI_SUBSECTOR_SIZE);
  printf("QSPI_Write=%u\n",rez);

  /* Read back data from the QSPI memory */
  rez = QSPI_Read(qspi_aRxBuffer, WRITE_READ_ADDR, QSPI_SUBSECTOR_SIZE);
  printf("QSPI_Read=%u\n",rez);

  /*##-4- Check read data integrity ##########################################*/
  if(Buffercmp(qspi_aTxBuffer, qspi_aRxBuffer, QSPI_SUBSECTOR_SIZE) > 0)
  //if(Buffercmp(fat, qspi_aRxBuffer, QSPI_SUBSECTOR_SIZE) > 0)
  {
     printf("Comparison failed\n");
  }
  else
  {
     printf("Comparison successfull\n");
  }

  /* Infinte loop */
  printf("STOPin mx25l25645g test\n");
  while (1)
  {
  }
}

void test2_mx25l25645g(void)
{
  //uint8_t qspi_aTxBuffer[QSPI_SUBSECTOR_SIZE];
  uint8_t qspi_aRxBuffer[QSPI_SUBSECTOR_SIZE];
  uint8_t rez;
  //uint32_t addr;
  //addr = (0) << 12;
  // uint8_t *p;
  
	// p = (uint8_t *)qspi_aTxBuffer;

  /* Read back data from the QSPI memory */
  rez = QSPI_Read(qspi_aRxBuffer, WRITE_READ_ADDR, 256);
  printf("QSPI_Read=%u\n",rez);

  printMass8bit(qspi_aRxBuffer, 256);
  /* Infinte loop */
  printf("STOP in test_mx25l25645g_r1\n");
  while (1)
  {
  }
}

void test3_mx25l25645g(void)
{
  uint8_t qspi_aTxBuffer[QSPI_SUBSECTOR_SIZE];
  uint8_t qspi_aRxBuffer[QSPI_SUBSECTOR_SIZE];

  uint8_t rez;
  uint32_t addr;
  addr = (0) << 12;
  // uint8_t *p;
	// p = (uint8_t *)qspi_aTxBuffer;
  uint32_t blockerr =0;
  for (uint32_t i =0; i < QSPI_NUM_SUBSECTOR ; i++ )
  //for (uint32_t i =0; i < 16 ; i++ )
  {
    addr = (i) << 12;
    /*##-2- Erase QSPI memory ##################################################*/
    rez = QSPI_Erase_Block(addr);
    //printf("QSPI_Erase_Block=%u\n",rez);

    /*##-3- QSPI memory read/write access  #####################################*/
    /* Fill the buffer to write */
    Fill_Buffer(qspi_aTxBuffer, QSPI_SUBSECTOR_SIZE, 0);

    /* Write data to the QSPI memory */
    rez = QSPI_Write(qspi_aTxBuffer, addr, QSPI_SUBSECTOR_SIZE);
    //printf("QSPI_Write=%u\n",rez);

    /* Read back data from the QSPI memory */
    rez = QSPI_Read(qspi_aRxBuffer, addr, QSPI_SUBSECTOR_SIZE);
    //printf("QSPI_Read=%u\n",rez);

    /*##-4- Check read data integrity ##########################################*/
    if(Buffercmp(qspi_aTxBuffer, qspi_aRxBuffer, QSPI_SUBSECTOR_SIZE) > 0)
    {
      printf("Comparison failed subsek=%lu\n",i);
      blockerr++;
    }
    else
    {
      printf("Comparison successfull subsek=%lu\n",i);
    }
  }
  printf("Comparison subsek err=%lu\n", blockerr);
  /* Infinte loop */
  printf("STOPin mx25l25645g test3\n");
  while (1);
}

uint32_t QSPI_ReadID(void)	
{
	QSPI_CommandTypeDef s_command;// QSPI transport configuration
  QSPI_AutoPollingTypeDef s_config;
	uint8_t	QSPI_ReceiveBuff[3];// Store data read by QSPI
	uint32_t Device_ID;// Device ID
  HAL_StatusTypeDef status;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    	// One line command mode
	//s_command.AddressSize       = QSPI_ADDRESS_24_BITS;     	// 24 bit address
  s_command.AddressSize       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  	// No alternate bytes 
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;      	// Disable DDR mode
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  	// Data delay in DDR mode is not used here
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 	// Every time the data is transmitted, an instruction is sent
	s_command.AddressMode		= QSPI_ADDRESS_NONE;   			// No address mode
	s_command.DataMode			= QSPI_DATA_1_LINE;       	 	// 1-line data mode
	s_command.DummyCycles 		= 0;                   			// Number of empty periods
	s_command.NbData 			= 3;                       		// Length of transmitted data
	s_command.Instruction 		= READ_ID_CMD;         	// Execute read device ID command

	// Send command
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) 
		return HAL_ERROR;// If the sending fails, an error message is returned
	// receive data 
	if (HAL_QSPI_Receive(&hqspi, QSPI_ReceiveBuff, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) 
		return HAL_ERROR;// If the reception fails, an error message is returned
    
	// Combine the obtained data into ID
	Device_ID = (QSPI_ReceiveBuff[0] << 16) | (QSPI_ReceiveBuff[1] << 8 ) | QSPI_ReceiveBuff[2];
  printf("FLASH Device_ID =0x%lX\n", Device_ID);
  if (Device_ID != 0xC22019)
  {
    assert_failed(__FILE__, __LINE__, Device_ID);
    //printf("FLASH 25L25645 ERROR\n");
    return HAL_ERROR;
  }

  uint8_t conf_reg;
  uint8_t status_reg;
  
  /* Initialize the reading of configuration register */
  s_command.Instruction       = READ_CFG_REG_CMD;
  s_command.NbData            = 1;
    /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &conf_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf ("conf_reg= 0x%x \n", conf_reg);
  
  /* Initialize the reading of status register */
  s_command.Instruction       = READ_STATUS_REG_CMD;
  s_command.NbData            = 1;
    /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &status_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf (status_reg= 0x%x \n", status_reg);

  /* Enable write operations */
  s_command.Instruction       = WRITE_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.NbData            = 0;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  status = HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    //printf("QSPI_WriteEnable HAL_QSPI_Command ERROR\n");
    return HAL_ERROR;
  }

  /* Configure automatic polling mode to wait the WEL bit=1 and WIP bit=0 */
  s_config.Match           = QSPI_SR_WREN;
  s_config.Mask            = QSPI_SR_WREN|QSPI_SR_WIP;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.Instruction    = READ_STATUS_REG_CMD;
  s_command.DataMode       = QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Update the configuration register with new dummy cycles */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = WRITE_STATUS_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 2;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* QSPI_DUMMY_CYCLES_READ_QUAD = 3 for 10 cycles in QPI mode */
  uint8_t reg[2];
  reg[0] = 0x43; //status register 0b01000011
  reg[1] = 0x80; //configuration register 0b10000000 Dummy 8 bit & 30Om
 
  /* Configure the write volatile configuration register command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(&hqspi, &(reg[0]), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* 40ms  Write Status/Configuration Register Cycle Time */
  QSPI_Delay( 40 );



  /* Initialize the reading of configuration register */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    	// One line command mode
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  	// No alternate bytes 
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;      	// Disable DDR mode
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  	// Data delay in DDR mode is not used here
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 	// Every time the data is transmitted, an instruction is sent
	s_command.AddressMode		= QSPI_ADDRESS_NONE;   			// No address mode
	s_command.DataMode			= QSPI_DATA_1_LINE;       	 	// 1-line data mode
	s_command.DummyCycles 		= 0;                   			// Number of empty periods
  s_command.Instruction       = READ_CFG_REG_CMD;
  s_command.NbData            = 1;// Length of transmitted data
  /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &conf_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf ("conf_reg= 0x%x \n", conf_reg);
  
  /* Initialize the reading of status register */
  s_command.Instruction       = READ_STATUS_REG_CMD;
  /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &status_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf (status_reg= 0x%x \n", status_reg);
  //////////////////////////////////////////////////////////////////////
  /* Initialize the reading of configuration register */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    	// One line command mode
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  	// No alternate bytes 
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;      	// Disable DDR mode
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  	// Data delay in DDR mode is not used here
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 	// Every time the data is transmitted, an instruction is sent
	s_command.AddressMode		= QSPI_ADDRESS_NONE;   			// No address mode
	s_command.DataMode			= QSPI_DATA_1_LINE;       	 	// 1-line data mode
	s_command.DummyCycles 		= 0;                   			// Number of empty periods
  s_command.Instruction       = READ_CFG_REG_CMD;
  s_command.NbData            = 1;// Length of transmitted data
  /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &conf_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf ("conf_reg= 0x%x \n", conf_reg);
  
  /* Initialize the reading of status register */
  s_command.Instruction       = READ_STATUS_REG_CMD;
  /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &status_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf (status_reg= 0x%x \n", status_reg);
////////////////////////////////////////////////////////////////////////




	return Device_ID;// Return ID
}

/**
  * @brief  QSPI Init
  * @param  None
  * @retval HAL_ERROR or HAL_OK
  */
uint8_t QSPI_Init(void)
{
  QSPI_CommandTypeDef s_command;// QSPI transport configuration
  QSPI_AutoPollingTypeDef s_config;
	uint8_t	QSPI_ReceiveBuff[3];// Store data read by QSPI
	uint32_t Device_ID;// Device ID
  uint8_t conf_reg;
  uint8_t status_reg;
  HAL_StatusTypeDef status;

  hqspi.Instance = QUADSPI;

  /* Call the DeInit function to reset the driver */
  if (HAL_QSPI_DeInit(&hqspi) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* System level initialization */
  HAL_QSPI_MspInit(&hqspi);

  /* QSPI initialization */
  //hqspi.Init.ClockPrescaler     = 4;   /* QSPI freq = 240 MHz/(1+4) = 48 Mhz */
  hqspi.Init.ClockPrescaler     = 0;   /* QSPI freq = 87 MHz/(1+0)= 87 Mhz */
  hqspi.Init.FifoThreshold      = 64;
  hqspi.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_NONE; //QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize          = POSITION_VAL(QSPI_FLASH_SIZE) - 1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode          = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID            = QSPI_FLASH_ID_2;
  hqspi.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;

  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (QSPI_ResetChip() != HAL_OK)
  {
    return HAL_ERROR;
  }

  QSPI_Delay(1);

  //READ ID
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    	// One line command mode
  s_command.AddressSize       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  	// No alternate bytes 
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;      	// Disable DDR mode
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  	// Data delay in DDR mode is not used here
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 	// Every time the data is transmitted, an instruction is sent
	s_command.AddressMode		    = QSPI_ADDRESS_NONE;   			// No address mode
	s_command.DataMode			    = QSPI_DATA_1_LINE;       	 	// 1-line data mode
	s_command.DummyCycles 	   	= 0;                   			// Number of empty periods
	s_command.NbData 			      = 3;                       		// Length of transmitted data
	s_command.Instruction 	  	= READ_ID_CMD;         	// Execute read device ID command

	// Send command
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) 
		return HAL_ERROR;// If the sending fails, an error message is returned
	// receive data 
	if (HAL_QSPI_Receive(&hqspi, QSPI_ReceiveBuff, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) 
		return HAL_ERROR;// If the reception fails, an error message is returned
    
	// Combine the obtained data into ID
	Device_ID = (QSPI_ReceiveBuff[0] << 16) | (QSPI_ReceiveBuff[1] << 8 ) | QSPI_ReceiveBuff[2];
  printf("FLASH Device_ID =0x%lX\n", Device_ID);
  if (Device_ID != 0xC22019)
  {
    assert_failed(__FILE__, __LINE__, Device_ID);
    printf("FLASH 25L25645 ERROR\n");
    return HAL_ERROR;
  }
  
  /* Initialize the reading of configuration register */
  s_command.Instruction       = READ_CFG_REG_CMD;
  s_command.NbData            = 1;
    /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &conf_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf ("conf_reg= 0x%x \n", conf_reg);
  
  /* Initialize the reading of status register */
  s_command.Instruction       = READ_STATUS_REG_CMD;
  s_command.NbData            = 1;
    /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &status_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf (status_reg= 0x%x \n", status_reg);

  /* Enable write operations */
  s_command.Instruction       = WRITE_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.NbData            = 0;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  status = HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    //printf("QSPI_WriteEnable HAL_QSPI_Command ERROR\n");
    return HAL_ERROR;
  }

  /* Configure automatic polling mode to wait the WEL bit=1 and WIP bit=0 */
  s_config.Match           = QSPI_SR_WREN;
  s_config.Mask            = QSPI_SR_WREN|QSPI_SR_WIP;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.Instruction    = READ_STATUS_REG_CMD;
  s_command.DataMode       = QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Update the configuration register with new dummy cycles */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = WRITE_STATUS_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 2;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* QSPI_DUMMY_CYCLES_READ_QUAD = 3 for 10 cycles in QPI mode */
  uint8_t reg[2];
  reg[0] = 0x43; //status register 0b01000011
  reg[1] = 0x80; //configuration register 0b10000000 Dummy 8 bit & 30Om
 
  /* Configure the write volatile configuration register command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(&hqspi, &(reg[0]), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* 40ms  Write Status/Configuration Register Cycle Time */
  QSPI_Delay( 40 );

  /* Set the QSPI memory in 4-bytes address mode */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = ENTER_4_BYTE_ADDR_MODE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Enable write operations */
  if (QSPI_WriteEnable() != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Send the command */
  status = HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    return HAL_ERROR;
  }

  /* Configure automatic polling mode to wait the memory is ready */
  status = QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    return HAL_ERROR;
  }
  
  /* Initialize the reading of configuration register */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    	// One line command mode
	//s_command.AddressSize       = QSPI_ADDRESS_24_BITS;     	// 24 bit address
  s_command.AddressSize       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  	// No alternate bytes 
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;      	// Disable DDR mode
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  	// Data delay in DDR mode is not used here
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 	// Every time the data is transmitted, an instruction is sent
	s_command.AddressMode		= QSPI_ADDRESS_NONE;   			// No address mode
	s_command.DataMode			= QSPI_DATA_1_LINE;       	 	// 1-line data mode
	s_command.DummyCycles 		= 0;                   			// Number of empty periods
	s_command.NbData 			= 1;                       		// Length of transmitted data
  s_command.Instruction       = READ_CFG_REG_CMD;
    /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &conf_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf ("conf_reg= 0x%x \n", conf_reg);
  
  /* Initialize the reading of status register */
  s_command.Instruction       = READ_STATUS_REG_CMD;
  s_command.NbData            = 1;
    /* Configure the command */
  if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, &status_reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  //printf (status_reg= 0x%x \n", status_reg);

  return HAL_OK;
}

/**
  * @brief  Erases the specified block of the QSPI memory.
  * @param  BlockAddress: Block address to erase
  * @retval QSPI memory status
  */
uint8_t QSPI_Erase_Block(uint32_t BlockAddress)
{
  QSPI_CommandTypeDef s_command;
  HAL_StatusTypeDef status;
  //printf("QSPIerase blockadd =0x%lX\n",BlockAddress);
  /* Initialize the erase command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = SUBSECTOR_ERASE_4_BYTE_ADDR_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
  s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
  s_command.Address           = BlockAddress;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Enable write operations */
  if (QSPI_WriteEnable() != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Send the command */
  status = HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    return HAL_ERROR;
  }

  /* Configure automatic polling mode to wait for end of erase */
  if (QSPI_AutoPollingMemReady(QSPI_SUBSECTOR_ERASE_MAX_TIME) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Writes an amount of data to the QSPI memory.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write
  * @retval QSPI memory status
  */
uint8_t QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
  QSPI_CommandTypeDef s_command;
  uint32_t end_addr, current_size, current_addr;
  HAL_StatusTypeDef status;
  //printf("QSPIwrite add =0x%lX, sz=0x%lX\n",WriteAddr, Size);
  /* Calculation of the size between the write address and the end of the page */
  current_addr = 0;

  while (current_addr <= WriteAddr)
  {
    current_addr += QSPI_PAGE_SIZE;
  }
  current_size = current_addr - WriteAddr;

  /* Check if the size of the data is less than the remaining place in the page */
  if (current_size > Size)
  {
    current_size = Size;
  }

  /* Initialize the address variables */
  current_addr = WriteAddr;
  end_addr = WriteAddr + Size;

  /* Initialize the program command */
  s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction        = PAGE_PROG_4_BYTE_ADDR_CMD;
  s_command.AddressMode        = QSPI_ADDRESS_1_LINE;
  s_command.AddressSize        = QSPI_ADDRESS_32_BITS;
  s_command.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
  //s_command.AlternateByteMode   = QSPI_ALTERNATE_BYTES_1_LINE;  //Add alternate bytes to 1 line 
  //s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS; //1 bytes of alternateByte is solved the problem
  s_command.DataMode           = QSPI_DATA_1_LINE;
  s_command.DummyCycles        = 0;
  s_command.DdrMode            = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle   = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;
  
  /* Perform the write page by page */
  do
  {
    s_command.Address = current_addr;
    s_command.NbData  = current_size;
    //printf("current_addr=0x%X, current_size=0x%x\n", current_addr, current_size);
    /* Enable write operations */
    status = QSPI_WriteEnable();
    if (status != HAL_OK)
    {
      assert_failed(__FILE__, __LINE__, status);
      return HAL_ERROR;
    }

    /* Configure the command */
    status = HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
    if (status != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Transmission of the data */
    status = HAL_QSPI_Transmit(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
    if (status != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Configure automatic polling mode to wait for end of program */
    status = QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
    if (status != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Update the address and size variables for next page programming */
    current_addr += current_size;
    pData += current_size;
    current_size = ((current_addr + QSPI_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : QSPI_PAGE_SIZE;
  } while (current_addr < end_addr);

  return HAL_OK;
}

/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval QSPI memory status
  */
uint8_t QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  QSPI_CommandTypeDef s_command;
  HAL_StatusTypeDef status;
  //printf("QSPIread add =0x%lX, sz=0x%lX\n",ReadAddr, Size);
  /* Initialize the read command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = READ_4_BYTE_ADDR_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
  s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
  s_command.Address           = ReadAddr;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  //s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_1_LINE;  //Add alternate bytes to 1 line 
  //s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS; //1 bytes of alternateByte is solved the problem
  //s_command.DummyCycles = QSPI_DUMMY_CYCLES_READ;
  s_command.DummyCycles = 0;
  s_command.NbData            = Size;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  status = HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    return HAL_ERROR;
  }

  /* Reception of the data */
  status = HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status  != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_WriteEnable(void)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;
  HAL_StatusTypeDef status;

  /* Enable write operations */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = WRITE_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  status = HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
  if (status != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    return HAL_ERROR;
  }

  /* Configure automatic polling mode to wait the WEL bit=1 and WIP bit=0 */
  s_config.Match           = QSPI_SR_WREN;
  s_config.Mask            = QSPI_SR_WREN|QSPI_SR_WIP;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.Instruction    = READ_STATUS_REG_CMD;
  s_command.DataMode       = QSPI_DATA_1_LINE;
  
  status = HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
  if(status != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hqspi: QSPI handle
  * @param  Timeout
  * @retval None
  */
static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;
  HAL_StatusTypeDef status;

  /* Configure automatic polling mode to wait for memory ready */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = READ_STATUS_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  s_config.Match           = 0;
  s_config.Mask            = QSPI_SR_WIP;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  status = HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, Timeout);
  if (status != HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, status);
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  This function set the QSPI memory in 4-byte address mode
  * @param  hqspi: QSPI handle
  * @retval None
  */
// static uint8_t QSPI_EnterFourBytesAddress(void)
// {
//   QSPI_CommandTypeDef s_command;
//   HAL_StatusTypeDef status;

//   /* Initialize the command */
//   s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
//   s_command.Instruction       = ENTER_4_BYTE_ADDR_MODE_CMD;
//   s_command.AddressMode       = QSPI_ADDRESS_NONE;
//   s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//   s_command.DataMode          = QSPI_DATA_NONE;
//   s_command.DummyCycles       = 0;
//   s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
//   s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
//   s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

//   /* Enable write operations */
//   if (QSPI_WriteEnable() != HAL_OK)
//   {
//     return HAL_ERROR;
//   }

//   /* Send the command */
//   status = HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
//   if (status != HAL_OK)
//   {
//     assert_failed(__FILE__, __LINE__, status);
//     return HAL_ERROR;
//   }

//   /* Configure automatic polling mode to wait the memory is ready */
//   status = QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
//   if (status != HAL_OK)
//   {
//     assert_failed(__FILE__, __LINE__, status);
//     return HAL_ERROR;
//   }

//   return HAL_OK;
// }


/**
* @brief  Fills buffer with user predefined data.
* @param  pBuffer: pointer on the buffer to fill
* @param  uwBufferLenght: size of the buffer to fill
* @param  uwOffset: first value to fill on the buffer
* @retval None
*/
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    //pBuffer[tmpIndex] = (tmpIndex + uwOffset)&0xFF;
    pBuffer[tmpIndex] = rand()%255;
  }
}

/**
* @brief  Compares two buffers.
* @param  pBuffer1, pBuffer2: buffers to be compared.
* @param  BufferLength: buffer's length
* @retval 1: pBuffer identical to pBuffer1
*         0: pBuffer differs from pBuffer1
*/
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  uint8_t ret =0;
  //uint32_t ind =0;
  for (uint32_t ind =0; ind<BufferLength; ind++)
  {
    //printf("bl=%04lu, Tx=0x%02X Rx0x%02X \n", ind, *pBuffer1, *pBuffer2);
    //QSPI_Delay(1);
    if (*pBuffer1 != *pBuffer2)
    {
      ret = 1;
      printf("bl=%lu, Tx=0x%02X |= Rx0x%02X \n", ind, *pBuffer2, *pBuffer1);
      QSPI_Delay(1);
    }

    pBuffer1++;
    pBuffer2++;
  }

  return ret;
}

uint32_t erase_mx25l25645g(void)
{
  uint8_t rez;
  uint32_t addr;
  uint32_t blockerr = 0;

  uint8_t* qspi_aTxBuffer;
  uint8_t* qspi_aRxBuffer;

  qspi_aTxBuffer = mx25l25645g_malloc(QSPI_SUBSECTOR_SIZE);
  qspi_aRxBuffer = mx25l25645g_malloc(QSPI_SUBSECTOR_SIZE);
  if (qspi_aTxBuffer == NULL || qspi_aRxBuffer == NULL)
  {
    printf("NO RAM!\n");
    blockerr=0xFFFFFFFF;
    goto out;
  }

  addr = (0) << 12;
  // uint8_t *p;
	// p = (uint8_t *)qspi_aTxBuffer;
  for (uint32_t i =0; i < QSPI_NUM_SUBSECTOR ; i++ )
  //for (uint32_t i =0; i < 16 ; i++ )
  {
    addr = (i) << 12;
    /*##-2- Erase QSPI memory ##################################################*/
    rez = QSPI_Erase_Block(addr);
    //printf("QSPI_Erase_Block=%u\n",rez);

    /*##-3- QSPI memory read/write access  #####################################*/
    /* Fill the buffer to write */
    Fill_Buffer(qspi_aTxBuffer, QSPI_SUBSECTOR_SIZE, 0);

    /* Write data to the QSPI memory */
    rez = QSPI_Write(qspi_aTxBuffer, addr, QSPI_SUBSECTOR_SIZE);
    //printf("QSPI_Write=%u\n",rez);

    /* Read back data from the QSPI memory */
    rez = QSPI_Read(qspi_aRxBuffer, addr, QSPI_SUBSECTOR_SIZE);
    //printf("QSPI_Read=%u\n",rez);

    /*##-4- Check read data integrity ##########################################*/
    if(Buffercmp(qspi_aTxBuffer, qspi_aRxBuffer, QSPI_SUBSECTOR_SIZE) > 0)
    {
      printf("Comparison failed subsek=%lu\n",i);
      blockerr++;
    }
    else
    {
      printf("Comparison successfull subsek=%lu\n",i);
    }
  }
  printf("Comparison subsek err=%lu\n", blockerr);
  /* Infinte loop */
out:
  mx25l25645g_free(qspi_aTxBuffer);
  mx25l25645g_free(qspi_aRxBuffer);
  return blockerr;
}
