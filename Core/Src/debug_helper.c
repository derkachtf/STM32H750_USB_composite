
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "debug_helper.h"
#include "stm32h7xx_hal.h"

// DEBUG printf to USART1 1152000 8N1 (NRF52805)
// DEBUG printf to USART2 1152000 8N1 (ESP8266)
extern UART_HandleTypeDef huart8;
#define SIZE_BUFF_PRINT_SRT 32U  // 32*256 = 8192
#define SIZE_BUFF_PRINT_NUM 128U

static uint8_t buff_print[SIZE_BUFF_PRINT_NUM][SIZE_BUFF_PRINT_SRT] __attribute__((section(".section_ram_d2")));

static volatile uint8_t pri_out=0;
static volatile uint8_t pri_in=0;
static volatile bool is_pritx = false;

static bool isTimout(uint32_t ms)
{
  static uint32_t oldtik;
  uint32_t tik;
  tik = HAL_GetTick();
  if( ( tik-oldtik ) < ms) return false;
  else return true;
  
  oldtik=tik;
}

static volatile bool is_debug_task = false;
static volatile uint32_t oldtik;
static volatile bool is_need_print = false;

int _write(int fd, char* ptr, int len)
{
  (void)fd;

  if(len>(SIZE_BUFF_PRINT_SRT-2)) len=(SIZE_BUFF_PRINT_SRT-2);

  if( pri_in >= (SIZE_BUFF_PRINT_NUM-1)) pri_in=0;
  else pri_in++;
  buff_print[pri_in][0]=len;
  memcpy((uint8_t*) &buff_print[pri_in][1], ptr, len);
  
  if(is_debug_task == false)
  {
    if (is_pritx == false || ( HAL_GetTick()-oldtik ) > 1 )// 2 milisek timeout(no ISR)
    {
      is_pritx =true;
      if( pri_out >= (SIZE_BUFF_PRINT_NUM-1)) pri_out=0;
      else pri_out++; 
      __HAL_UART_CLEAR_OREFLAG(&huart8);
      __HAL_UART_CLEAR_TXFECF(&huart8);
      HAL_UART_Transmit_DMA(&huart8, (uint8_t *) &buff_print[pri_out][1], buff_print[pri_out][0]);
      oldtik = HAL_GetTick();
    }
  }
  else
  {
    // flag to debug task
    is_need_print = true;
  }

  //HAL_UART_Transmit(&huart8, (uint8_t *) ptr, len, 1);
  return len;
}

void InitOSprint(void)
{
  is_debug_task = true;
}

void OSprint(void)
{
  if(is_need_print == true)
  {
    is_need_print = false;
    if (is_pritx == false || ( HAL_GetTick()-oldtik ) > 1 )// 2 milisek timeout(no ISR)
    {
      is_pritx =true;
      if( pri_out >= (SIZE_BUFF_PRINT_NUM-1)) pri_out=0;
      else pri_out++; 
      __HAL_UART_CLEAR_OREFLAG(&huart8);
      __HAL_UART_CLEAR_TXFECF(&huart8);
      HAL_UART_Transmit_DMA(&huart8, (uint8_t *) &buff_print[pri_out][1], buff_print[pri_out][0]);
      oldtik = HAL_GetTick();
    }
  }
}

void IsrPrintDbug(void)
{
  if(pri_out != pri_in)
  {
    if( pri_out >= (SIZE_BUFF_PRINT_NUM-1) ) pri_out=0;
    else pri_out++;
    __HAL_UART_CLEAR_OREFLAG(&huart8);
    __HAL_UART_CLEAR_TXFECF(&huart8);
    HAL_UART_Transmit_DMA(&huart8, (uint8_t *) &buff_print[pri_out][1], buff_print[pri_out][0]); 
  }
  else
  {
    is_pritx = false;
  }
}

// Assumes little endian
//https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
void printBits(size_t const size, void const * const ptr)
{
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;

  printf("0b");

  for (i = size-1; i >= 0; i--)
  {
    for (j = 7; j >= 0; j--)
    {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }
  
  puts("");
}

void printMass8bit(const uint8_t *buff, uint32_t count)
{
  uint8_t* p;

  p = (uint8_t* )buff;
  printf("\n ={");
  for(uint32_t i = 0; i<count; i++)
  {
    if(i%16==0) printf("\\\n/*%04lu*/ ",i);
    HAL_Delay(1);
    printf("0x%02X, ", p[i]);
  }

  printf(" };\n");
}

