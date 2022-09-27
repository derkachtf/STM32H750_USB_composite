/**
  ******************************************************************************
  * @file    stm32746g_discovery_audio.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32746g_discovery_audio.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DAC_AUDIO_H
#define __DAC_AUDIO_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Include audio component Driver */
//#include "../Components/wm8994/wm8994.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//#include "usbd_audio_dac.h"

#define DAC_BUFF_SIZE     96U // 96 = 2msek on 48kHz (AUDIO_DAC_TOTAL_BUF_SIZE/2) //512
#define DAC_BUFF_CNT      32U
#define RAW_BUFFER_SIZE      (DAC_BUFF_SIZE*DAC_BUFF_CNT)
//extern volatile uint16_t dac_buff[DAC_BUFF_SIZE];
//extern volatile uint32_t ind_buff;

typedef enum
{
  AUDIO_DAC_MODE_USB = 0,
  AUDIO_DAC_MODE_FILE,
  AUDIO_DAC_MODE_UNKNOWN,
} AUDIO_DAC_ModeTypeDef;
typedef enum
{
  AUDIO_DAC_OFFSET_NONE = 0,
  AUDIO_DAC_OFFSET_HALF,
  AUDIO_DAC_OFFSET_FULL,
  AUDIO_DAC_OFFSET_UNKNOWN,
} AUDIO_DAC_OffsetTypeDef;

typedef struct
{
  //ALIGN_32BYTES( volatile uint16_t buffer[DAC_BUFF_SIZE*DAC_BUFF_CNT]; );
  volatile uint16_t dac_buff[DAC_BUFF_SIZE];
  volatile uint8_t raw_buffer[RAW_BUFFER_SIZE];
  uint32_t alt_setting;
  AUDIO_DAC_OffsetTypeDef offset;
  uint8_t rd_enable;
  uint16_t rd_ptr;
  uint16_t wr_ptr;
  //USBD_AUDIO_ControlTypeDef control;
} AUDIO_DAC_HandleTypeDef;

extern volatile AUDIO_DAC_ModeTypeDef dac_mode;
/*------------------------------------------------------------------------------
             CONFIGURATION: Audio Driver Configuration parameters
------------------------------------------------------------------------------*/

#define AUDIODATA_SIZE                      ((uint16_t)2)   /* 16-bits audio data size */

/* Audio status definition */     
#define AUDIO_OK                            ((uint8_t)0)
#define AUDIO_ERROR                         ((uint8_t)1)
#define AUDIO_TIMEOUT                       ((uint8_t)2)

uint8_t DAC_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
uint8_t DAC_AUDIO_OUT_Play (uint8_t mode);
void    DAC_AUDIO_OUT_ChangeBuffer(uint8_t *pBuffer, uint32_t Size);
void    DAC_AUDIO_OUT_UpdateBuffer(uint8_t* pBuffer, uint32_t Size);
uint8_t DAC_AUDIO_OUT_Pause(void);
uint8_t DAC_AUDIO_OUT_Resume(void);
uint8_t DAC_AUDIO_OUT_Stop(uint32_t Option);
uint8_t DAC_AUDIO_OUT_SetVolume(uint8_t Volume);
void    DAC_AUDIO_OUT_SetFrequency(uint32_t AudioFreq);
void    DAC_AUDIO_OUT_SetAudioFrameSlot(uint32_t AudioFrameSlot);
uint8_t DAC_AUDIO_OUT_SetMute(uint32_t Cmd);
uint8_t DAC_AUDIO_OUT_SetOutputMode(uint8_t Output);
void    DAC_AUDIO_OUT_DeInit(void);

/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function is called when the requested data has been completely transferred.*/
void    DAC_AUDIO_OUT_TransferComplete_CallBack(void);

/* This function is called when half of the requested buffer has been transferred. */
void    DAC_AUDIO_OUT_HalfTransfer_CallBack(void);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void    DAC_AUDIO_OUT_Error_CallBack(void);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void    DAC_AUDIO_IN_Error_CallBack(void);


#ifdef __cplusplus
}
#endif

#endif /* __DAC_AUDIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
