
/* Includes ------------------------------------------------------------------*/
#include "dac_audio.h"
#include "usbd_audio.h"
#include "usb_device.h"
#include "usbd_audio_spkr_if.h"
#include "main.h"
#include <math.h>
#include <stdio.h>

#include "audio.h"

extern TIM_HandleTypeDef htim6;
extern DAC_HandleTypeDef hdac1;
extern USBD_HandleTypeDef hUsbDevice;

//#define printf(...)  ((void) 0)
volatile AUDIO_DAC_ModeTypeDef dac_mode = AUDIO_DAC_MODE_USB;
static AUDIO_DAC_HandleTypeDef haudioDACinstance;
// должен быть не в DTCMRAM
/* https://community.st.com/s/article/FAQ-DMA-is-not-working-on-STM32H7-devices */
//ALIGN_32BYTES( volatile uint16_t dac_buff[DAC_BUFF_SIZE] );

static void DAC_DMA_ClearBuff(void);

static void DAC_DMA_ClearBuff(void)
{
  for(size_t i = 0; i < DAC_BUFF_SIZE; i++)
  {
    haudioDACinstance.dac_buff[i] = 2048;
  }
}
/**
  * @brief  Configures the audio peripherals.
  * @param  OutputDevice: OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       or OUTPUT_DEVICE_BOTH.
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @note   The I2S PLL input clock must be done in the user application.  
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t DAC_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{ 
  uint8_t ret = AUDIO_OK;
  haudioDACinstance.rd_ptr=0;
  haudioDACinstance.wr_ptr=0;
  haudioDACinstance.rd_enable=0;

  //HAL_TIM_Base_Start(&htim6); // задает 48000 Гц для Аудио ЦАП
  DAC_DMA_ClearBuff();
  printf("DAC_AUDIO_OUT_Init\n");
  return ret;
}

/**
  * @brief  Starts playing audio stream from a data buffer for a determined size. 
  * @param  pBuffer: Pointer to the buffer 
  * @param  Size: Number of audio data in BYTES unit.
  *         In memory, first element is for left channel, second element is for right channel
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t DAC_AUDIO_OUT_Play (uint8_t mode)
{
#if(0)
  /* Call the audio Codec Play function */
    // віключить MUTE
    /* Update the Media layer and enable it for play */  
    //HAL_SAI_Transmit_DMA(&haudio_out_sai, (uint8_t*) pBuffer, DMA_MAX(Size / AUDIODATA_SIZE));
  ind_buff=0;

  switch (mode)
  {
  case 0:
    haudioDACinstance.rd_ptr=0;
    haudioDACinstance.rd_enable=1;
  // ,e
    // for(uint32_t i = 0; i < DAC_BUFF_SIZE; i++)
    // {
    //   haudioDACinstance.dac_buff[i] = 2048;
    // }
    break;
  
  case 1:
    for (uint32_t i = 0; i < DAC_BUFF_SIZE; i++)
    {
      haudioDACinstance.dac_buff[i] = 2048;
      //haudioDACinstance.dac_buff[i] = 1000.0 * sin(angle)+2048;
      //angle += (2 * M_PI) / DAC_BUFF_SIZE*4; // 4 периода
    }
    break;
  default:
    break;
  }
    
    // for (uint32_t i = 0; i < DAC_BUFF_SIZE; i++)
    // {
    //   //if (i%16 == 0) printf("\n");

    //   printf("dac_buff[%i]=%i\n", i, dac_buff[i]);
    //   HAL_Delay(100);
    // }

  HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, (uint32_t*)haudioDACinstance.dac_buff, DAC_BUFF_SIZE, DAC_ALIGN_12B_R);
  printf("DAC_AUDIO_OUT_Play\n");
#endif
  return AUDIO_OK;
}

void DAC_AUDIO_OUT_StartIfNeed(void)
{
  if(haudioDACinstance.rd_enable == 0)
  {
    if(haudioDACinstance.wr_ptr >= (RAW_BUFFER_SIZE/2U+RAW_BUFFER_SIZE/4U))
    {
      haudioDACinstance.rd_enable = 1;
      haudioDACinstance.rd_ptr=0;
      printf("DAC_AUDIO_OUT_StartIfNeed\n");
      DAC_DMA_ClearBuff();
      HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, (uint32_t*)haudioDACinstance.dac_buff, DAC_BUFF_SIZE, DAC_ALIGN_12B_R);
    }
  }
}

void DAC_AUDIO_OUT_StopIfNeed(void)
{
  ;
}

// ewri milisecond 192 byte
//ALIGN_32BYTES( volatile uint16_t dac_prom_buff[DAC_BUFF_SIZE*32] );
void DAC_AUDIO_OUT_UpdateBuffer(uint8_t* pBuffer, uint32_t Size)
{
  if(Size%4) assert_failed(__FILE__, __LINE__, Size);
  for (size_t i = 0; i < Size; i++)
  {

    haudioDACinstance.raw_buffer[haudioDACinstance.wr_ptr] = pBuffer[i];
    
    haudioDACinstance.wr_ptr++;
    
    if(haudioDACinstance.wr_ptr >= RAW_BUFFER_SIZE)
    {
      haudioDACinstance.wr_ptr=0;
    } 
  }
  DAC_AUDIO_OUT_StartIfNeed();
#if(0)
  return;
  static uint32_t tik=0;
  uint32_t tmp;
  tmp =HAL_GetTick();
  printf("SZ=%lu, t=%lu\n", Size, tmp-tik );
  //tik = tmp;

  //ind_buff=0;
  //for(uint32_t i = 0; i < Size/2; i+=2) //FIXME: отдельно метод - корекция формат стерео USB
  if(Size%4) assert_failed(__FILE__, __LINE__, Size);

  for(uint32_t i = 0; i < Size; i+=4)
  {
    //printf("s%02u=%i\n",i, pBuffer[i]);

    uint32_t tl;
    uint32_t th;
    tl=pBuffer[i+2];
    th=pBuffer[i+3];
    tl=tl&0xFF;
    th = (th<<8)&0xFF00;
    int16_t sempl= (int16_t) (th | tl)/16;
    if(sempl>2047) sempl=2047;
    if(sempl<-2048) sempl=-2048;
    dac_buff[ind_buff] = sempl + 2048;
    ind_buff++;
    if(ind_buff >= DAC_BUFF_SIZE ) 
    {
      ind_buff=0;
    }
  }
  if(tik==1) HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, (uint32_t*)dac_buff, DAC_BUFF_SIZE, DAC_ALIGN_12B_R);
  tik++;
#endif
}

/**
  * @brief  Sends n-Bytes on the SAI interface.
  * @param  pData: pointer on data address 
  * @param  Size: number of data to be written
  * @retval None
  */
void DAC_AUDIO_OUT_ChangeBuffer(uint8_t *pBuffer, uint32_t Size)
{
#if(0)
  return;
  ind_buff=0;
  //sizDbuf = Size;
  printf("Cb=%lu\n",Size);
  if(Size%4) assert_failed(__FILE__, __LINE__, Size);
  if( Size > DAC_BUFF_SIZE)
  {
    assert_failed(__FILE__, __LINE__, Size);
    return;
  }
   //HAL_SAI_Transmit_DMA(&haudio_out_sai, (uint8_t*) pData, Size);
for(uint32_t i = 0; i < Size; i+=4) //FIXME: отдельно метод - корекция формат стерео USB
  {
    //printf("s%02u=%i\n",i, pBuffer[i]);

    uint32_t tl;
    uint32_t th;
    tl=pBuffer[i+2];
    th=pBuffer[i+3];
    tl=tl&0xFF;
    th = (th<<8)&0xFF00;
    int16_t sempl= (int16_t) (th | tl)/16;
    if(sempl>2047) sempl=2047;
    if(sempl<-2048) sempl=-2048;
    dac_buff[ind_buff] = sempl + 2048;
    ind_buff++;
    if(ind_buff >= DAC_BUFF_SIZE ) 
    {
      ind_buff=0;
    }
  }
//   HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_2);
  //HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, (uint32_t*)dac_buff, DAC_BUFF_SIZE, DAC_ALIGN_12B_R);
  HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, (uint32_t*)dac_buff, Size, DAC_ALIGN_12B_R);
#endif
}

/**
  * @brief  This function Pauses the audio file stream. In case
  *         of using DMA, the DMA Pause feature is used.
  * @note When calling DAC_AUDIO_OUT_Pause() function for pause, only
  *          DAC_AUDIO_OUT_Resume() function should be called for resume (use of DAC_AUDIO_OUT_Play() 
  *          function for resume could lead to unexpected behaviour).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t DAC_AUDIO_OUT_Pause(void)
{    

    /* Call the Media layer pause function */
    //HAL_SAI_DMAPause(&haudio_out_sai);
  //    ind_buff=0;
  // for(uint32_t i = 0; i < DAC_BUFF_SIZE; i++)
  // {
  //   dac_buff[i] = 2048;
  // }

    HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_2);
    
    /* Return AUDIO_OK when all operations are correctly done */
  printf("DAC_AUDIO_OUT_Pause\n");
  return AUDIO_OK;

}

/**
  * @brief  This function  Resumes the audio file stream.  
  * @note When calling DAC_AUDIO_OUT_Pause() function for pause, only
  *          DAC_AUDIO_OUT_Resume() function should be called for resume (use of DAC_AUDIO_OUT_Play() 
  *          function for resume could lead to unexpected behaviour).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t DAC_AUDIO_OUT_Resume(void)
{    

  /* Call the Media layer pause/resume function */
  //HAL_SAI_DMAResume(&haudio_out_sai);
  DAC_DMA_ClearBuff();
  HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, (uint32_t*)haudioDACinstance.dac_buff, DAC_BUFF_SIZE, DAC_ALIGN_12B_R);
  
  printf("DAC_AUDIO_OUT_Resume\n");
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
  * @brief  Stops audio playing and Power down the Audio Codec. 
  * @param  Option: could be one of the following parameters 
  *           - CODEC_PDWN_SW: for software power off (by writing registers). 
  *                            Then no need to reconfigure the Codec after power on.
  *           - CODEC_PDWN_HW: completely shut down the codec (physically). 
  *                            Then need to reconfigure the Codec after power on.  
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t DAC_AUDIO_OUT_Stop(uint32_t Option)
{

  HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_2);
  
  printf("DAC_AUDIO_OUT_Stop\n");
  return AUDIO_OK;
}

/**
  * @brief  Controls the current audio volume level. 
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t DAC_AUDIO_OUT_SetVolume(uint8_t Volume)
{
  /* Call the codec volume control function with converted volume value */
  
  printf("DAC_AUDIO_OUT_SetVolume\n");
  return AUDIO_OK;
}

/**
  * @brief  Enables or disables the MUTE mode by software 
  * @param  Cmd: Could be AUDIO_MUTE_ON to mute sound or AUDIO_MUTE_OFF to 
  *         unmute the codec and restore previous volume level.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t DAC_AUDIO_OUT_SetMute(uint32_t Cmd)
{ 

//  DAC_DMA_ClearBuff();
//  HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_2);
  printf("DAC_AUDIO_OUT_SetMute\n");
  
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
  * @brief  Switch dynamically (while audio file is played) the output target 
  *         (speaker or headphone).
  * @param  Output: The audio output target: OUTPUT_DEVICE_SPEAKER,
  *         OUTPUT_DEVICE_HEADPHONE or OUTPUT_DEVICE_BOTH
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t DAC_AUDIO_OUT_SetOutputMode(uint8_t Output)
{

  printf("DAC_AUDIO_OUT_SetOutputMode\n");
  return AUDIO_OK;
}

/**
  * @brief  Updates the audio frequency.
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @note   This API should be called after the DAC_AUDIO_OUT_Init() to adjust the
  *         audio frequency.
  * @retval None
  */
void DAC_AUDIO_OUT_SetFrequency(uint32_t AudioFreq)
{ 
  printf("DAC_AUDIO_OUT_SetFrequency\n");
  // HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_2);;
}


/**
  * @brief  Deinit the audio peripherals.
  * @retval None
  */
void DAC_AUDIO_OUT_DeInit(void)
{
  HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_2);//
  DAC_DMA_ClearBuff();
  haudioDACinstance.rd_enable =0;
  haudioDACinstance.rd_ptr =0;
  haudioDACinstance.wr_ptr =0;
  printf("DAC_AUDIO_OUT_DeInit\n");
}


void DAC_DMA_CallBack(AUDIO_OffsetTypeDef offset)
{
  uint16_t dacData;
  int32_t delta;
  int8_t shift = 0;
  size_t i;

  if (haudioDACinstance.rd_enable == 1U)
  {
    if (haudioDACinstance.rd_ptr > haudioDACinstance.wr_ptr)// |--------wp-----rp----|
    {
      delta = (haudioDACinstance.rd_ptr - haudioDACinstance.wr_ptr);
      if ( delta < DAC_BUFF_SIZE*2)
      {
        shift = +4; //printf("S+\n");
      }
      else
      {
        if ( delta > (RAW_BUFFER_SIZE - DAC_BUFF_SIZE*2))
        {
          shift = -4; //printf("S-\n");
        }
      }
    }
    else //(haudio->rd_ptr < haudio->wr_ptr)//// |---rp---wp---------|
    {
      delta = (haudioDACinstance.wr_ptr - haudioDACinstance.rd_ptr);
      //printf("D%li\n", delta);
      if (delta < DAC_BUFF_SIZE*2)
      {
        shift = -4; //printf("Ss-%li\n", delta);
      }
      else
      {
        if (delta > (RAW_BUFFER_SIZE - DAC_BUFF_SIZE*2))
        {
          shift = +4; //printf("Ss+\n");
        }
      }
    }

    //haudioDACinstance.rd_ptr += (uint16_t) AUDIO_OUT_PACKET; //AUDIO_DAC_OUT_PACKET/2=DAC_BUFF_SIZE - stero/2=mono
    haudioDACinstance.rd_ptr += shift;
    
    //if (shift |=0 )  printf("Ds%u, w%u, r%u\n", haudio->offset, haudio->wr_ptr, haudio->rd_ptr);

    // pBuffer = &haudio->buffer[haudio->rd_ptr];
  ////////////
  // uint32_t k =0;
    //uint8_t *pBuffer;

    //pBuffer = &haudioDACinstance.raw_buffer[haudioDACinstance.rd_ptr];
    //if(haudioDACinstance.rd_enable==1)
    // memcpy(buff, (uint8_t*)readBuff, size);
    for( i = 0; i < DAC_BUFF_SIZE/2; i++)
    {
      //printf("s%02u=%i\n",i, pBuffer[i]);
      //dacData = (((haudioDACinstance.raw_buffer[haudioDACinstance.rd_ptr + 1] << 8) | haudioDACinstance.raw_buffer[haudioDACinstance.rd_ptr+0]) + 32767);
      dacData = (((haudioDACinstance.raw_buffer[haudioDACinstance.rd_ptr + 3] << 8) | haudioDACinstance.raw_buffer[haudioDACinstance.rd_ptr+2]) + 32767);
      //k+=2;
      //haudioDACinstance.rd_ptr+=2; // mono
      haudioDACinstance.rd_ptr+=4;
      if(haudioDACinstance.rd_ptr>=RAW_BUFFER_SIZE) // rol bak
      {
        haudioDACinstance.rd_ptr=0;
      }
      
      dacData /= 16;
      //dacData /= gain;
      //printf("s=%i\n",dacData);
      switch (offset)
      {
      case AUDIO_DAC_OFFSET_HALF:
        haudioDACinstance.dac_buff[i] = dacData;
        break;
      case AUDIO_DAC_OFFSET_FULL:
        haudioDACinstance.dac_buff[i+DAC_BUFF_SIZE/2] = dacData;
        break;
      default:
        break;
      }
    }
    //is_upd = true;
  ////////////////
    // if ( offset == AUDIO_OFFSET_HALF)
    // {
    //   ind_buff=0;
    //   for(uint32_t i = 0; i < (AUDIO_OUT_PACKET); i+=4)
    //   {
    //     uint32_t tl;
    //     uint32_t th;
    //     tl=pBuffer[i+2];
    //     th=pBuffer[i+3];
    //     tl=tl&0xFF;
    //     th = (th<<8)&0xFF00;
    //     int16_t sempl= (int16_t) (th | tl)/16;
    //     if(sempl>2047) sempl=2047;
    //     if(sempl<-2048) sempl=-2048;
    //     dac_buff[ind_buff] = sempl + 2048;
    //     ind_buff++;
    //   }
    // }
    // else if ( offset == AUDIO_OFFSET_FULL)
    // {
    //   ind_buff=DAC_BUFF_SIZE/2;
    //   for(uint32_t i = 0; i < (AUDIO_OUT_PACKET); i+=4)
    //   {
    //     uint32_t tl;
    //     uint32_t th;
    //     tl=pBuffer[i+2];
    //     th=pBuffer[i+3];
    //     tl=tl&0xFF;
    //     th = (th<<8)&0xFF00;
    //     int16_t sempl= (int16_t) (th | tl)/16;
    //     if(sempl>2047) sempl=2047;
    //     if(sempl<-2048) sempl=-2048;
    //     dac_buff[ind_buff] = sempl + 2048;
    //     ind_buff++;
    //   }
    // }
  }
  else //haudioDACinstance.rd_enable == 0U)
  {
    for(i = 0; i < DAC_BUFF_SIZE/2; i++)
    {
      switch (offset)
      {
      case AUDIO_DAC_OFFSET_HALF:
        haudioDACinstance.dac_buff[i] = 2048;
        break;
      case AUDIO_DAC_OFFSET_FULL:
        haudioDACinstance.dac_buff[i+DAC_BUFF_SIZE/2] = 2048;
        break;
      default:
        break;
      }
    }
  }
}
/**
  * @brief  Conversion complete callback in non-blocking mode for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef *hdac)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);
  switch (dac_mode)
  {
  case AUDIO_DAC_MODE_USB:
    //printf("du\n");
    //TransferComplete_SPKR_CallBack(); //USBD_AUDIO_DAC_Sync(&hUsbDevice, AUDIO_DAC_OFFSET_FULL);
    DAC_DMA_CallBack(AUDIO_DAC_OFFSET_FULL);
    break;
  case AUDIO_DAC_MODE_FILE:
    //printf("df\n");
   // file_AUDIO_DAC_Sync(AUDIO_DAC_OFFSET_FULL);
    break;
  default:
    break;
  }

  //HAL_GPIO_TogglePin(ON5VP1_GPIO_Port, ON5VP1_Pin);
  //printf("dC\n");
}

/**
  * @brief  Conversion half DMA transfer callback in non-blocking mode for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef *hdac)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);
  switch (dac_mode)
  {
  case AUDIO_DAC_MODE_USB:
    //HalfTransfer_SPKR_CallBack();
    //USBD_AUDIO_DAC_Sync(&hUsbDevice, AUDIO_DAC_OFFSET_HALF);
    DAC_DMA_CallBack(AUDIO_DAC_OFFSET_HALF);
    break;
  case AUDIO_DAC_MODE_FILE:
    //file_AUDIO_DAC_Sync(AUDIO_DAC_OFFSET_HALF);
    break;
  default:
    break;
  }
//  printf("dH\n");
//  HAL_GPIO_TogglePin(ON5VP1_GPIO_Port, ON5VP1_Pin);
  //HAL_GPIO_WritePin(ON5VP1_GPIO_Port, ON5VP1_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Error DAC callback for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DACEx_ErrorCallbackCh2(DAC_HandleTypeDef *hdac)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);
  assert_failed(__FILE__, __LINE__, 0);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DACEx_ErrorCallbackCh2 could be implemented in the user file
   */
}

/**
  * @brief  DMA underrun DAC callback for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DACEx_DMAUnderrunCallbackCh2(DAC_HandleTypeDef *hdac)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);
  assert_failed(__FILE__, __LINE__, 0);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DACEx_DMAUnderrunCallbackCh2 could be implemented in the user file
   */
}
