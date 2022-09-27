/**
  ******************************************************************************
  * @file         extconfig.c
  * @author       contactus@clawskeyboard.com
  * @brief        
  ******************************************************************************
  * @attention
  *
  * Licensed 
  *
  ******************************************************************************
  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "audio.h"
#include "dac_audio.h"

#include "fileworker.h"

#if 0
 static uint32_t wavelen = 0;
 static char* WaveFileName ;
 static __IO uint32_t SpeechDataOffset = 0x00;
 __IO ErrorCode WaveFileStatus = Unvalid_RIFF_ID;
 UINT BytesRead;
 WAVE_FormatTypeDef WAVE_Format;
 uint16_t buffer1[_MAX_SS] ={0x00};
 uint16_t buffer2[_MAX_SS] ={0x00};
 uint8_t buffer_switch = 1;
 extern FATFS fatfs;
 extern FIL file;
 extern FIL fileR;
 extern DIR dir;
 extern FILINFO fno;
 extern uint16_t *CurrentPos;
 extern USB_OTG_CORE_HANDLE USB_OTG_Core;
 extern uint8_t WaveRecStatus


static ErrorCode WavePlayer_WaveParsing(uint32_t *FileLen);
#endif
volatile bool is_upd;

volatile uint32_t readBytes;
#define GAIN_DIV    1
volatile uint32_t gain =1;
#define DAC2_BUFF_SIZE 960
ALIGN_32BYTES( volatile uint16_t dac2_buff[DAC2_BUFF_SIZE] );
extern DAC_HandleTypeDef hdac1;

void play_welcom_song(void)
{

  // bluetooth сreate dir if they are not

  static AUDIO_DAC_ModeTypeDef old_dac_mode;
  old_dac_mode = dac_mode;
  dac_mode = AUDIO_DAC_MODE_FILE;
  uint32_t res, wavDataSize;
  uint32_t dataOffset;
  //printf("play_welcom_song=%p\n", 25); osDelay(2);

  dataOffset = 0;
  wavDataSize =0;
  res = f_open(&filRead, "0:/"DIR_AUDIO_LOCAL"/"FILE_AUDIO_WAV"", FA_READ);
  if(res != FR_OK) 
  {
    printf("NO welcom song\n");//assert_failed(__FILE__, __LINE__, res);
    goto errexit;
  }
  memset(readBuff, 0, sizeof(readBuff));

  res = f_read(&filRead, readBuff, sizeof(readBuff), &bytesRead);  
  if(res != FR_OK) 
  {
    assert_failed(__FILE__, __LINE__, res);
    goto errexit;
  }
  
  for (uint16_t i = 0; i < (sizeof(readBuff) - 3); i++)
  {
    if ((readBuff[i] == 'd') && (readBuff[i + 1] == 'a') &&
        (readBuff[i + 2] == 't') && (readBuff[i + 3] == 'a'))
    {
        dataOffset = i + 8;
        break;
    }
  }

  if(dataOffset == 0) 
  {
    assert_failed(__FILE__, __LINE__, res);
    goto errexit;
  }
restart:  
//printf("gain=%u\n",gain);
  res = f_lseek(&filRead, dataOffset);
  if(res != FR_OK) 
  {
    assert_failed(__FILE__, __LINE__, res);
    goto errexit;
  }
  wavDataSize = f_size(&filRead) - dataOffset;
  //printf("dataOffset=%u, wavDataSize=%u\n", dataOffset, wavDataSize); osDelay(2);
  //DAC_AUDIO_OUT_Init(0, Volume, AudioFreq);
  for(uint32_t i = 0; i < DAC2_BUFF_SIZE; i++)
  {
    dac2_buff[i] = 2048;
  }
  HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, (uint32_t*)dac2_buff, DAC2_BUFF_SIZE, DAC_ALIGN_12B_R); //DAC_AUDIO_OUT_Play (0);
  is_upd = true;
  uint32_t ws = wavDataSize;
  while( ws > 0 )
  {
    uint32_t chunk_size = ws > DAC2_BUFF_SIZE ? DAC2_BUFF_SIZE : ws;
    res = f_read(&filRead, readBuff, chunk_size, &readBytes);
    if(res != FR_OK) 
    {
      assert_failed(__FILE__, __LINE__, res);
      goto errexit;
    }

    ws -= chunk_size;

    while (is_upd == false) osDelay(1);
    is_upd = false;
    
  }
  for(uint32_t i = 0; i < DAC2_BUFF_SIZE; i++)
  {
    dac2_buff[i] = 2048;
  }
  DAC_AUDIO_OUT_Stop(0);
  is_upd = true;
  

  if(gain < GAIN_DIV)
  {
    gain *= 2;
    goto restart;
  }
errexit:  
    f_close(&filRead);
    dac_mode = old_dac_mode;
}

void file_AUDIO_DAC_Sync(AUDIO_DAC_OffsetTypeDef offset)
{
  uint32_t k =0;
  // memcpy(buff, (uint8_t*)readBuff, size);
  for(uint32_t i = 0; i < DAC2_BUFF_SIZE/2; i++)
  {
    //printf("s%02u=%i\n",i, pBuffer[i]);
    uint16_t dacData = (((readBuff[k + 1] << 8) | readBuff[k+0]) + 32767);
    k+=2;
    dacData /= 16;
    dacData /= gain;
    //printf("s=%i\n",dacData);
    switch (offset)
    {
    case AUDIO_DAC_OFFSET_HALF:
      dac2_buff[i] = dacData;
      break;
    case AUDIO_DAC_OFFSET_FULL:
      dac2_buff[i+DAC2_BUFF_SIZE/2] = dacData;
      break;
    default:
      break;
    }
  }
  is_upd = true;
}

#if 0
/**
  * @brief  Checks the format of the .WAV file and gets information about
  *   the audio format. This is done by reading the value of a
  *   number of parameters stored in the file header and comparing
  *   these to the values expected authenticates the format of a
  *   standard .WAV  file (44 bytes will be read). If  it is a valid
  *   .WAV file format, it continues reading the header to determine
  *   the  audio format such as the sample rate and the sampled data
  *   size. If the audio format is supported by this application,
  *   it retrieves the audio format in WAVE_Format structure and
  *   returns a zero value. Otherwise the function fails and the
  *   return value is nonzero.In this case, the return value specifies
  *   the cause of  the function fails. The error codes that can be
  *   returned by this function are declared in the header file.
  * @param  None
  * @retval Zero value if the function succeed, otherwise it return
  *         a nonzero value which specifies the error code.
  */
static ErrorCode WavePlayer_WaveParsing(uint32_t *FileLen)
{
  uint32_t temp = 0x00;
  uint32_t extraformatbytes = 0;
  
  /* Read chunkID, must be 'RIFF' */
  temp = ReadUnit((uint8_t*)buffer1, 0, 4, BigEndian);
  if (temp != CHUNK_ID)
  {
    return(Unvalid_RIFF_ID);
  }
  
  /* Read the file length */
  WAVE_Format.RIFFchunksize = ReadUnit((uint8_t*)buffer1, 4, 4, LittleEndian);
  
  /* Read the file format, must be 'WAVE' */
  temp = ReadUnit((uint8_t*)buffer1, 8, 4, BigEndian);
  if (temp != FILE_FORMAT)
  {
    return(Unvalid_WAVE_Format);
  }
  
  /* Read the format chunk, must be'fmt ' */
  temp = ReadUnit((uint8_t*)buffer1, 12, 4, BigEndian);
  if (temp != FORMAT_ID)
  {
    return(Unvalid_FormatChunk_ID);
  }
  /* Read the length of the 'fmt' data, must be 0x10 -------------------------*/
  temp = ReadUnit((uint8_t*)buffer1, 16, 4, LittleEndian);
  if (temp != 0x10)
  {
    extraformatbytes = 1;
  }
  /* Read the audio format, must be 0x01 (PCM) */
  WAVE_Format.FormatTag = ReadUnit((uint8_t*)buffer1, 20, 2, LittleEndian);
  if (WAVE_Format.FormatTag != WAVE_FORMAT_PCM)
  {
    return(Unsupporetd_FormatTag);
  }
  
  /* Read the number of channels, must be 0x01 (Mono) or 0x02 (Stereo) */
  WAVE_Format.NumChannels = ReadUnit((uint8_t*)buffer1, 22, 2, LittleEndian);
  
  /* Read the Sample Rate */
  WAVE_Format.SampleRate = ReadUnit((uint8_t*)buffer1, 24, 4, LittleEndian);

  /* Read the Byte Rate */
  WAVE_Format.ByteRate = ReadUnit((uint8_t*)buffer1, 28, 4, LittleEndian);
  
  /* Read the block alignment */
  WAVE_Format.BlockAlign = ReadUnit((uint8_t*)buffer1, 32, 2, LittleEndian);
  
  /* Read the number of bits per sample */
  WAVE_Format.BitsPerSample = ReadUnit((uint8_t*)buffer1, 34, 2, LittleEndian);
  if (WAVE_Format.BitsPerSample != BITS_PER_SAMPLE_16) 
  {
    return(Unsupporetd_Bits_Per_Sample);
  }
  SpeechDataOffset = 36;
  /* If there is Extra format bytes, these bytes will be defined in "Fact Chunk" */
  if (extraformatbytes == 1)
  {
    /* Read th Extra format bytes, must be 0x00 */
    temp = ReadUnit((uint8_t*)buffer1, 36, 2, LittleEndian);
    if (temp != 0x00)
    {
      return(Unsupporetd_ExtraFormatBytes);
    }
    /* Read the Fact chunk, must be 'fact' */
    temp = ReadUnit((uint8_t*)buffer1, 38, 4, BigEndian);
    if (temp != FACT_ID)
    {
      return(Unvalid_FactChunk_ID);
    }
    /* Read Fact chunk data Size */
    temp = ReadUnit((uint8_t*)buffer1, 42, 4, LittleEndian);
    
    SpeechDataOffset += 10 + temp;
  }
  /* Read the Data chunk, must be 'data' */
  temp = ReadUnit((uint8_t*)buffer1, SpeechDataOffset, 4, BigEndian);
  SpeechDataOffset += 4;
  if (temp != DATA_ID)
  {
    return(Unvalid_DataChunk_ID);
  }
  
  /* Read the number of sample data */
  WAVE_Format.DataSize = ReadUnit((uint8_t*)buffer1, SpeechDataOffset, 4, LittleEndian);
  SpeechDataOffset += 4;
  WaveCounter =  SpeechDataOffset;
  return(Valid_WAVE_File);
}
#endif