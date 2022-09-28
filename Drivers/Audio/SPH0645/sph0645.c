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
#include "main.h"
#include "sph0645.h"
#include <stdio.h>
#include <math.h>
//#include "cmsis_os.h"
#include "i2s.h"
#include "dac_audio.h"
#include "usbd_audio_mic.h"

//#define AUDIO_MIC_CHANNELS     0x01
//#define AUDIO_MIC_SMPL_FREQ   48000
#define HAL_TIMEOUT     1000
#define MIC_MILISEK_SIZE 10U
#define MIC_BUFF_SIZE   (MIC_MILISEK_SIZE * AUDIO_MIC_CHANNELS * (AUDIO_MIC_SMPL_FREQ/1000U) * 2U) //256 //(AUDIO_MIC_OUT_PACKET*2) //256 //512
#define MIC_GAIN_DEF 8

extern USBD_HandleTypeDef hUsbDevice;

ALIGN_32BYTES (volatile uint32_t mic_buff[MIC_BUFF_SIZE]);
ALIGN_32BYTES (volatile int16_t pcm_buff[MIC_BUFF_SIZE]); 
static volatile uint8_t *pbuff_usb = NULL;   // указатель на выходной буффер
static volatile uint32_t size_usb_byte = 0;  // размер выходного буффера USB
static volatile uint32_t byte_cnt=0;
static uint32_t mic_gain = MIC_GAIN_DEF;
extern volatile uint32_t mic_buff_cnt;

volatile bool mic_on = false;

void sph0645_init(void)
{
  //mic_on = true;
  //sph0645_Record();
}

void sph0645_DeInit(void)
{

}
void sph0645_Record(void)
{
  HAL_StatusTypeDef hal_st;
  printf("sph0645_Record\n");
  hal_st = HAL_I2S_Receive_DMA(&hi2s2,
                        mic_buff,
                        MIC_BUFF_SIZE );
  if(hal_st!=HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, hal_st);
  }

  mic_on = true;
}

void sph0645_VolumeCtl(int16_t Volume)
{
  //printf("sph0645_VolumeCtl(%i)\n",Volume);
  // // reduce resolution from 1/256dB to 1/2dB

  // volume /= 128;

  // // ensure SVC library limits are respected

  // if (volume < -160) {
  //   volume = -160;
  // } else if (volume > 72) {
  //   volume = 72;
  // }

  // _volumeControl.setVolume(volume);
}
void sph0645_MuteCtl(uint8_t cmd)
{

}
void sph0645_Stop(void)
{
  HAL_StatusTypeDef hal_st;
  hal_st = HAL_I2S_DMAStop(&hi2s2);
  if(hal_st!=HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, hal_st);
  }
  mic_on = false;
}

void sph0645_Pause(void)
{
  HAL_StatusTypeDef hal_st;
  hal_st = HAL_I2S_DMAPause(&hi2s2);
  if(hal_st!=HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, hal_st);
  }
  mic_on = false;
}

void sph0645_Resume(void)
{
  HAL_StatusTypeDef hal_st;
  hal_st = HAL_I2S_DMAResume(&hi2s2);
  if(hal_st!=HAL_OK)
  {
    assert_failed(__FILE__, __LINE__, hal_st);
  }
  mic_on = true;
}

void sph0645_CommandMgr(uint8_t cmd)
{

}

void sph0645_buf(uint8_t *pout_buff, uint32_t siz)
{
  size_usb_byte = siz;
  pbuff_usb = pout_buff;
  printf("sph0645_buf siz=%i, pb=%i\n",size_usb_byte, pbuff_usb);
}

void sph0645_set_gain(uint32_t gain)
{
  mic_gain = gain;
  printf("sph0645 set gain =%lu\n", mic_gain);
}

static volatile bool is_mic_buf_centr = false;

void sph0645_buf_centr(void)
{
  is_mic_buf_centr = true;
}

static void put_buff (uint32_t *addr, uint32_t dsize)
{
  if(is_mic_buf_centr)
  {
    is_mic_buf_centr = false;
    byte_cnt=size_usb_byte/2;
  }
  for(uint32_t i=0; i < dsize; i++)
  {
    if( byte_cnt >= size_usb_byte )
    {
      byte_cnt=0;
    }
    
    uint32_t norm = addr[i];
    int32_t vol=(norm >> 16);
    if(vol&(1<<15)) vol|=0xFFFF0000; // знак -
    vol*=mic_gain;
    if( vol > INT16_MAX) vol = INT16_MAX;
    if( vol < INT16_MIN) vol = INT16_MIN;

    if(i&1) // первый  канал WAV RIGHT (MK1)
    {
    // waw_buf[WAV_DATA_OFSET+byte_cnt]=(norm >> 8) & 0xFF;
    // byte_cnt++;
    // waw_buf[WAV_DATA_OFSET+byte_cnt]=(norm) & 0xFF;
    // byte_cnt++;
      // pbuff_usb[byte_cnt] = ((uint32_t)vol >> 0) & 0xFF;
      // //waw_buf[WAV_DATA_OFSET+byte_cnt]=(norm >> 16) & 0xFF;
      // byte_cnt++;
      // pbuff_usb[byte_cnt] = ((uint32_t)vol >> 8) & 0xFF;
      // //waw_buf[WAV_DATA_OFSET+byte_cnt]=(norm >> 24) & 0xFF;
      // byte_cnt++;
    }
    else // нулевой канал WAV LEFT (MK2)
    {
      pbuff_usb[byte_cnt]= (vol >> 0) & 0xFF;
      //waw_buf[WAV_DATA_OFSET+byte_cnt]=(norm >> 16) & 0xFF;
      byte_cnt++;
      pbuff_usb[byte_cnt]= (vol >> 8) & 0xFF;
      // //waw_buf[WAV_DATA_OFSET+byte_cnt]=(norm >> 24) & 0xFF;
      byte_cnt++;
    }
    //mic_buff_cnt+=2;
    
  }
  mic_buff_cnt+=dsize;
  //printf("m");
}

void usb_put_buff( volatile int32_t *data_in, int16_t *data_out, size_t size)
{

     // transform the I2S samples from the 64 bit L/R (32 bits per side)

      int16_t *dest = data_out;

      for (uint16_t i = 0; i < size; i++)
      {
        // dither the LSB with a random bit

        // int16_t sample = (data_in[0] & 0xfffffffe) | (rand() & 1);

        // *dest++ = sample;     // left channel has data
        // *dest++ = sample;     // right channel is duplicated from the left
        *dest++ = data_in[0];     // left channel has data
        *dest++ = data_in[0];     // right channel is duplicated from the left
        data_in += 2;
      }

    // send the adjusted data to the host

    if (USBD_AUDIO_MIC_Data_Transfer(&hUsbDevice, data_out, size) != USBD_OK) 
    {
      printf("USBD_AUDIO_Data_Transfer ERRR%u\n",HAL_GetTick() );
      //Error_Handler();
    }

}


HAL_StatusTypeDef sph0645_set_freq(uint32_t new_freq)
{
  I2S_HandleTypeDef *hi2s = &hi2s2;
  uint32_t i2sdiv;
  uint32_t i2sodd;
  uint32_t packetlength;
  uint32_t tmp;
  uint32_t i2sclk;
  uint32_t ispcm;
  
  printf("sph0645_set_freq(%u)\n", new_freq);
  
  if (hi2s == NULL)
  {
    return HAL_ERROR;
  }

  if (hi2s->Init.AudioFreq == new_freq)
  {
    return HAL_OK;
  }

  hi2s->Init.AudioFreq = new_freq;

  if (hi2s->Init.DataFormat == I2S_DATAFORMAT_16B)
  {
    packetlength = 1UL;
  }
  else
  {
    packetlength = 2UL;
  }

  /* Check if PCM standard is used */
  if ((hi2s->Init.Standard == I2S_STANDARD_PCM_SHORT) ||
      (hi2s->Init.Standard == I2S_STANDARD_PCM_LONG))
  {
    ispcm = 1UL;
  }
  else
  {
    ispcm = 0UL;
  }

  i2sclk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI123);

  /* Compute the Real divider depending on the MCLK output state, with a floating point */
  if (hi2s->Init.MCLKOutput == I2S_MCLKOUTPUT_ENABLE)
  {
    /* MCLK output is enabled */
    tmp = (uint32_t)((((i2sclk / (256UL >> ispcm)) * 10UL) / hi2s->Init.AudioFreq) + 5UL);
  }
  else
  {
    /* MCLK output is disabled */
    tmp = (uint32_t)((((i2sclk / ((32UL >> ispcm) * packetlength)) * 10UL) / hi2s->Init.AudioFreq) + 5UL);
  }

  /* Remove the flatting point */
  tmp = tmp / 10UL;

  /* Check the parity of the divider */
  i2sodd = (uint32_t)(tmp & (uint32_t)1UL);

  /* Compute the i2sdiv prescaler */
  i2sdiv = (uint32_t)((tmp - i2sodd) / 2UL);

  /* Test if the obtain values are forbidden or out of range */
  if (((i2sodd == 1UL) && (i2sdiv == 1UL)) || (i2sdiv > 0xFFUL))
  {
    /* Set the error code */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_PRESCALER);
    return  HAL_ERROR;
  }

  /* Force i2smod to 1 just to be sure that (2xi2sdiv + i2sodd) is always higher than 0 */
  if (i2sdiv == 0UL)
  {
    i2sodd = 1UL;
  }

  MODIFY_REG(hi2s->Instance->I2SCFGR, (SPI_I2SCFGR_I2SDIV                 | SPI_I2SCFGR_ODD),
             ((i2sdiv << SPI_I2SCFGR_I2SDIV_Pos) | (i2sodd << SPI_I2SCFGR_ODD_Pos)));

  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
  return HAL_OK;
}

//int16_t filtered_buff[MIC_BUFF_SIZE*4];
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);
//  dac_put_buff((uint32_t *) &mic_buff[0], MIC_BUFF_SIZE/2);
  //put_buff(&mic_buff[0], MIC_BUFF_SIZE/2);
  if(mic_on) usb_put_buff(&mic_buff[0], &pcm_buff[0], MIC_BUFF_SIZE/2);
  //HAL_GPIO_WritePin(REZEV_I2S_SCKIN_GPIO_Port, REZEV_I2S_SCKIN_Pin,GPIO_PIN_SET); //onLED
  //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
  //printf("Hi2s\n");
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);
  //dac_put_buff((uint32_t *) &mic_buff[MIC_BUFF_SIZE/2], MIC_BUFF_SIZE/2);
  //put_buff(&mic_buff[MIC_BUFF_SIZE/2], MIC_BUFF_SIZE/2);
  if(mic_on) usb_put_buff(&mic_buff[MIC_BUFF_SIZE/2], &pcm_buff[MIC_BUFF_SIZE/2], MIC_BUFF_SIZE/2);
  //HAL_GPIO_WritePin(REZEV_I2S_SCKIN_GPIO_Port, REZEV_I2S_SCKIN_Pin,GPIO_PIN_RESET); //onLED
  //printf("Fi2s\n");
}

void sph0645_test(I2S_HandleTypeDef *hi2s)
{
  printf("sph0645_test\n");
  //uint32_t i_usb=0;
  HAL_StatusTypeDef hal_st;
    // hal_st = HAL_I2S_Receive_DMA(hi2s,
    //                       mic_buff,
    //                       (MIC_BUFF_SIZE*1) );
  //__HAL_DBGMCU_UNFREEZE_IWDG();
  osDelay(3000);
  while(1)
  {
    if(size_usb_byte==0 || pbuff_usb == NULL)
    {
      osDelay(1);
      continue;
    }
    // for(uint32_t i=0; i<MIC_BUFF_SIZE; i++)
    // {
    //   mic_buff[i] = 0xAA;
    // }
    hal_st = HAL_I2S_Receive(hi2s,
                            mic_buff,
                            (MIC_BUFF_SIZE*1),
                            HAL_TIMEOUT);
    if(hal_st != HAL_OK)
    {
      printf("HAL_I2S hal_st=0x%i\n",hal_st);
      osDelay(1000);//osDelay(5000);
    }
    put_buff (mic_buff, MIC_BUFF_SIZE);
    // continue;

    // for(uint16_t j = 0; j < MIC_BUFF_SIZE; j+=2)
		// {
		// 	filtered_buff[j] = mic_buff[j]>>16;
    //   //filtered_buff[j] = (((float)mic_buff[j] - (float)16777215.0) / (float)16777215.0);
    //   filtered_buff[j+1] = filtered_buff[j];
		// }
    
    // // for(uint16_t j = 1; j < MIC_BUFF_SIZE+1; j+=2)
		// // {
		// // 	filtered_buff[j-1] = mic_buff[j]>>16;
    // //   filtered_buff[j] = filtered_buff[j-1];
		// // }

		// for(uint16_t j = 1; j < MIC_BUFF_SIZE; j+=2)
		// {
    //   pbuff_usb[i_usb] = filtered_buff[j];
    //   i_usb++;
    //   if(i_usb==size_usb_byte/2) i_usb=0;
		// }
  }
//    //memset(waw_buf, 0, sizeof(waw_buf));
// //return;
//   /* Write chunkID, must be 'RIFF'  ------------------------------------------*/
//   waw_buf[0] = 'R';
//   waw_buf[1] = 'I';
//   waw_buf[2] = 'F';
//   waw_buf[3] = 'F';

//   /* Write the file length ----------------------------------------------------*/
// /* The sampling time 8000 Hz To recorde 10s we need 8000 x 10 x 2 (16-Bit data) */
//   /* The sampling time: this value will be be written back at the end of the 
//   recording opearation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
//   waw_buf[4] = 0x00;
//   waw_buf[5] = 0x00;
//   waw_buf[6] = 0x00;
//   waw_buf[7] = 0x00;

//   /* Write the file format, must be 'WAVE' -----------------------------------*/
//   waw_buf[8]  = 'W';
//   waw_buf[9]  = 'A';
//   waw_buf[10] = 'V';
//   waw_buf[11] = 'E';

//   /* Write the format chunk, must be'fmt ' -----------------------------------*/
//   waw_buf[12]  = 'f';
//   waw_buf[13]  = 'm';
//   waw_buf[14]  = 't';
//   waw_buf[15]  = ' ';

//   /* Write the length of the 'fmt' data, must be 0x10 ------------------------*/
//   waw_buf[16]  = 0x10;
//   waw_buf[17]  = 0x00;
//   waw_buf[18]  = 0x00;
//   waw_buf[19]  = 0x00;

//   /* Write the audio format, must be 0x01 (PCM) ------------------------------*/
//   waw_buf[20]  = 0x01;
//   waw_buf[21]  = 0x00;

//   /* Write the number of channels, ie. 0x01 (Mono) ---------------------------*/
//   waw_buf[22]  = NBR_CHANL; /* Number of channels: 1:Mono or 2:Stereo */
//   waw_buf[23]  = 0x00;

//   /* Write the Sample Rate in Hz ---------------------------------------------*/
//   /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
//   waw_buf[24]  = (uint8_t)((SAMPL_RATE & 0xFF));
//   waw_buf[25]  = (uint8_t)((SAMPL_RATE >> 8) & 0xFF);
//   waw_buf[26]  = (uint8_t)((SAMPL_RATE >> 16) & 0xFF);
//   waw_buf[27]  = (uint8_t)((SAMPL_RATE >> 24) & 0xFF);

//   /* Write the Byte Rate -----------------------------------------------------*/
//   waw_buf[28]  = (uint8_t)(( (SAMPL_RATE*(BIT_PER_SAMPL/8U)*NBR_CHANL)  & 0xFF));
//   waw_buf[29]  = (uint8_t)(( (SAMPL_RATE*(BIT_PER_SAMPL/8U)*NBR_CHANL)  >> 8) & 0xFF);
//   waw_buf[30]  = (uint8_t)(( (SAMPL_RATE*(BIT_PER_SAMPL/8U)*NBR_CHANL)  >> 16) & 0xFF);
//   waw_buf[31]  = (uint8_t)(( (SAMPL_RATE*(BIT_PER_SAMPL/8U)*NBR_CHANL)  >> 24) & 0xFF);

//   /* Write the block alignment -----------------------------------------------*/
//   waw_buf[32]  = (uint8_t) (NBR_CHANL*(BIT_PER_SAMPL/8U));
//   waw_buf[33]  = 0x00;

//   /* Write the number of bits per sample -------------------------------------*/
//   waw_buf[34]  = 16U; /* Number of bits per sample (16, 24 or 32) */
//   waw_buf[35]  = 0x00;

//   /* Write the Data chunk, must be 'data' ------------------------------------*/
//   waw_buf[36]  = 'd';
//   waw_buf[37]  = 'a';
//   waw_buf[38]  = 't';
//   waw_buf[39]  = 'a';

//   /* Write the number of sample data -----------------------------------------*/
//   /* This variable will be written back at the end of the recording operation */
//   waw_buf[40]  = 0x00;
//   waw_buf[41]  = 0x00;
//   waw_buf[42]  = 0x00;
//   waw_buf[43]  = 0x00;

//   uint32_t oldtik = HAL_GetTick();
//   uint32_t sek =0;
//   printf("Sekee =%lu\n", sek);
//   while (byte_cnt != sizeof(waw_buf)) // жду пока наполнится буффер
//   {
//     if(HAL_GetTick() - oldtik >1000)
//     {
//       printf("Sek =%lu\n", sek++);
//       oldtik = HAL_GetTick();
//     }
//   }
  
//   /* Update the data length in the header of the recorded wave */ 
//   waw_buf[4] = (uint8_t)((byte_cnt-4) & 0xFF) ;
//   waw_buf[5] = (uint8_t)(((byte_cnt-4)  >> 8) & 0xFF);
//   waw_buf[6] = (uint8_t)(((byte_cnt-4)  >> 16) & 0xFF);
//   waw_buf[7] = (uint8_t)(((byte_cnt-4)  >> 24) & 0xFF);
  
//   waw_buf[40] = (uint8_t)((byte_cnt-44) & 0xFF);
//   waw_buf[41] = (uint8_t)(((byte_cnt-44) >> 8) & 0xFF);
//   waw_buf[42] = (uint8_t)(((byte_cnt-44) >> 16) & 0xFF);
//   waw_buf[43] = (uint8_t)(((byte_cnt-44) >> 24) & 0xFF);

//   // записываю в фаил
//   // if( file_write(WAVE_FILE, waw_buf , sizeof(waw_buf)) == 0)
//   // {
//   //   printf("rec.wav write ERROR\n");
//   // }
//   // printf("rec.wav write ok byte_cnt=0X%lX, %lu  \n", byte_cnt, byte_cnt);
//   //while(1);
}

void microphoneTask(void)
{
  
  //sph0645_test(&hi2s2);
  osThreadExit(); //while(1) osDelay(1);
}

