/**
  ******************************************************************************
  * @file         sph0645.h
  * @author       contactus@clawskeyboard.com
  * @brief        драйвер sph0645 (i2s)
  ******************************************************************************
  * @attention
  *
  * Licensed 
  * reff
  ******************************************************************************
  */

#ifndef __SPH0645_H
#define __SPH0645_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

void sph0645_test(I2S_HandleTypeDef *hi2s);

void sph0645_buf(uint8_t *pout_buff, uint32_t siz);
void sph0645_buf_centr(void);
void sph0645_init(void);
void sph0645_DeInit(void);
void sph0645_Record(void);
void sph0645_VolumeCtl(int16_t Volume);
void sph0645_MuteCtl(uint8_t cmd);
void sph0645_Stop(void);
void sph0645_Pause(void);
void sph0645_Resume(void);
void sph0645_CommandMgr(uint8_t cmd);
void microphoneTask(void);
void sph0645_set_gain(uint32_t gain);
HAL_StatusTypeDef sph0645_set_freq(uint32_t new_freq);

#ifdef __cplusplus
}
#endif
#endif  // __SPH0645_H
