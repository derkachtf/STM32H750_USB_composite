
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "debug_helper.h"
#include "stm32h7xx_hal.h"

//#include "cmsis_os.h"
//#include "sph0645.h"
//#include "st7789.h"
//#include "bmp280.h"
//#include "hdc1080.h"
//#include "ccs811.h"
//#include "mpu9250.h"
//#include "apds9960.h"
//#include "u24lc512.h"
// #include "image_on_head.h"

//#include "n25q128.h"
//#include "w25q64.h"
//#include "i2c.h"
//#include "spi.h"
//#include "tim.h"
//#include "usart.h"
//#include "encoder.h"
//#include "layer.h"
//#include "keymap.h"

//uint8_t bbuff[12288];
// static uint8_t buff[64];

// static const char *const Build_Date = __DATE__;
// static const char *const Build_Time = __TIME__;


// void _4DebugThread(void)
// {
//   //int32_t cl1;
//   //int32_t cl2;
//   encoder_init();
//   while (1)
//   {
//     osDelay(100);
//     // cl1 = encoder_get_clicks(ENC1);
//     // cl2 = encoder_get_clicks(ENC2);
//     // printf("ENC1=%li, ENC2=%li\n",cl1 ,cl2);
//   }
// }

// void _3DebugThread(void)
// {
//   uint16_t prew_cnt = 0;
//   uint16_t cnt_clk = 0;
//   HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
//   HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
//   while (1)
//   {
//     osDelay(100);
//     uint32_t tec_cnt = __HAL_TIM_GET_COUNTER(&htim3);
//     uint32_t is_up = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
//     //uint32_t tec_cnt = __HAL_TIM_GET_COUNTER(&htim8);
//     //uint32_t is_up = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim8);
//     uint16_t delta_up = prew_cnt - tec_cnt;
//     if(delta_up < 4) continue;

//     uint16_t delta_dw = tec_cnt - prew_cnt;
//     if(delta_dw < 4) continue;

//     uint32_t is_upm;
//     int32_t n_cliks;
//     if (delta_dw<delta_up)
//     {
//       is_upm=0;
//       n_cliks = delta_dw/4;
//       prew_cnt+= (uint16_t) n_cliks*4;
//     }
//     else 
//     {
//       is_upm=1;
//       n_cliks = delta_up/4;
//       prew_cnt-= (uint16_t) n_cliks*4;
//     }
    
//     printf("tec_cnt=%lu, prew_cnt=%i is_up=%lu\n", tec_cnt, prew_cnt, is_up);
//     printf("delta_up=%i, delta_dw=%i, is_upm=%lu\n", delta_up, delta_dw, is_upm);
//     do
//     {
//       cnt_clk++;
//       printf("%u-n_cliks=%lu\n",cnt_clk , n_cliks);
//       n_cliks--;
//     } while (n_cliks>0);
//     if(is_up!=is_upm) printf("ERRRRRRR\n");
//     printf("--\n");
//   }

// }

// void _2DebugThread(void)
// {
//   //uint32_t cnt =0;
//    osDelay(200);
//   // ST7789_Init();
//   // ST7789_Fill_Color(BLACK);
//   led_ok_init();
//   int rez=QSPI_Init();
//   if(!rez)
//   {    
//     printf("W25Qxx_Init %i",rez);
//   }
//   else
//   {
//     printf("QSPI_W25Qxx_Init %i",rez);
//   }
	
//   uint32_t Device_ID;
//   Device_ID = QSPI_ReadID();//Read ID
//   printf("Device_ID0x%lX",Device_ID);
  
//   rez=QSPI_MemoryMappedMode();
//   if(!rez)
//   {    
//     printf("MemMer%i",rez);
//   }
//   else
//   {
//     printf("W25Qxx_Me %i",rez);
//   }
//   uint8_t ledok_pwr=40;
//   while(1)
//   {
//     //printf("cnt=%lu \n",cnt++);
//     //printBits(sizeof(cnt), &cnt);
//     uint16_t color = rand() % WHITE+1;
//     ST7789_DrawRectangle(0, 0, 239, 239, color);
//     //ST7789_DrawRectangle(1, 1, 238, 238, color);
//     ledok_pwr > 80 ? ledok_pwr=0 : (ledok_pwr+=4);
//     led_ok_toggle(ledok_pwr);
//     osDelay(1000);
//   }
// }
// #define DEB_DELAY (100U)
// void _1DebugThread(void)
// {
//   uint32_t cnt =0;
//   uint32_t sk=0;
//   uint32_t mn=0;
//   uint32_t hr=0;
//   uint32_t day=0;
//   osDelay(200);
//   ST7789_Init();
//   ST7789_Fill_Color(BLACK);
//   led_ok_init();
//   uint8_t ledok_pwr=100;
//   while(1)
//   {
//     ledok_pwr > 80 ? ledok_pwr=0 : (ledok_pwr+=4);
//     led_ok_toggle(ledok_pwr);
//     osDelay(DEB_DELAY);//TODO: меняю задержку на 210 и неработает
//     cnt++;
//     if (cnt>=(1000/DEB_DELAY))
//     {
//       cnt=0;
//       sk++;
//       if (sk>=60)
//       {
//         sk=0;
//         mn++;
//         if (mn>=60)
//         {
//           mn=0;
//           hr++;
//           if (hr>=24)
//           {
//             hr=0;
//             day++;
//           }
//         }
//       }
//     }

//     printf("dbg_cnt=%lu\n",cnt);
//     //if (sk == 10) osDelay(1000); //TODO:стабильно виснет дисплей если ничего невыводить > 200милисекунд. Поток при этом не висит
//     //ST7789_Init();
//     uint16_t color = rand() % WHITE+1;
//     ST7789_DrawRectangle(0, 0, 239, 239, color);
//     ST7789_DrawRectangle(1, 1, 238, 238, color);
//     sprintf((char *)buff,"On %lidays, %02li.%02li.%02li.%li", day, hr, mn, sk, cnt);
//     ST7789_WriteString(3, 3, (const char *)buff, Font_11x18, WHITE, BLACK);
    
//     sprintf((char *)buff,"Build - %s", Build_Date);
//     ST7789_WriteString(3, 23, (const char *)buff, Font_11x18, WHITE, BLACK);
    
//     sprintf((char *)buff,"Build - %s", Build_Time);
//     ST7789_WriteString(3, 43, (const char *)buff, Font_11x18, WHITE, BLACK);
// 	}
//   /* USER CODE END StartDebugTask */
// }

// static uint8_t buff[256];
// void DebugThread(void)
// {
//   uint32_t cntt=0;


// //BMP280 start
//   BMP280_HandleTypedef bmp280;
  
//   bmp280.params.mode = BMP280_MODE_NORMAL;
// 	bmp280.params.filter = BMP280_FILTER_16;
// 	bmp280.params.oversampling_pressure = BMP280_ULTRA_HIGH_RES;
// 	bmp280.params.oversampling_temperature = BMP280_ULTRA_HIGH_RES;
// 	bmp280.params.oversampling_humidity = BMP280_STANDARD;
// 	bmp280.params.standby = BMP280_STANDBY_250;
// 	bmp280.addr = BMP280_I2C_ADDRESS_0;
// 	bmp280.i2c = &hi2c4;
//   while (!bmp280_init(&bmp280, &bmp280.params)) 
//   {
// 		printf("BMP280 initialization failed\n");
// 		HAL_Delay(2000);
// 	}
//   bool bme280p = bmp280.id == BME280_CHIP_ID;
// 	printf("BMP280: found %s\n", bme280p ? "BME280" : "BMP280");
// //BMP280 stop
  
// //HDC1080 start
//   HDC1080_HandleTypedef hdc1080;
//   uint32_t humid_hdc;
//   int32_t temp_hdc;
  
//   hdc1080.params.rez_temp = HDC1080_REZ_TEMPERAT_14BIT;
// 	hdc1080.params.rez_temp = HDC1080_REZ_HUMIDITY_14BIT;
// 	hdc1080.i2c = &hi2c4;
//   while (!hdc1080_init(&hdc1080, &hdc1080.params)) 
//   {
// 		printf("hdc1080 initialization failed\n");
// 		HAL_Delay(1000);
// 	}

// //HDC1080 stop

// //CCS811 start
//   CCS811_HandleTypedef ccs811;
  
// 	ccs811.i2c = &hi2c4;
//   while (!ccs811_init(&ccs811)) 
//   {
// 		printf("ccs811 initialization failed\n");
// 		HAL_Delay(5000);
// 	}

// //CCS811 stop

// //MPU9250 start
//   MPU9250_HandleTypedef mpu9250;
  
// 	mpu9250.i2c = &hi2c4;
//   while (!mpu9250_init(&mpu9250)) 
//   {
// 		printf("mpu9250 initialization failed\n");
// 		HAL_Delay(5000);
// 	}

// //MPU9250 stop

// //APDS9960 start
//   APDS9960_HandleTypedef apds9960_hi2c4;
  
// 	apds9960_hi2c4.i2c = &hi2c4;
//   while (!apds9960_init(&apds9960_hi2c4)) 
//   {
// 		printf("apds9960_hi2c4 initialization failed\n");
// 		HAL_Delay(5000);
// 	}
//   setProximityGain(&apds9960_hi2c4, PGAIN_2X);
  
//   // Start running the APDS-9960 proximity sensor (no interrupts)
//   if ( enableProximitySensor(&apds9960_hi2c4, false) ) 
//   {
//     printf("Proximity sensor is now running\n");
//   } else 
//   {
//    printf("Proximity sensor is NOT running\n");
//   }
//   // Start running the APDS-9960 light sensor (no interrupts)
//   if ( enableLightSensor(&apds9960_hi2c4, false) ) 
//   {
//     printf("Light sensor is now running\n");
//   } else 
//   {
//     printf("Light sensor is NOT running\n");
//   }

//     // Start running the APDS-9960 gesture sensor engine
//   if ( enableGestureSensor(&apds9960_hi2c4, false) ) 
//   {
//     printf("Gesture sensor is now running\n");
//   } 
//   else 
//   {
//     printf("Gesture sensor is NOT running\n");
//   }

// /////////////////////////////////////////////////////////////////////////////////////////
//   APDS9960_HandleTypedef apds9960_hi2c1;
  
// 	apds9960_hi2c1.i2c = &hi2c1;
//   while (!apds9960_init(&apds9960_hi2c1)) 
//   {
// 		printf("apds9960_hi2c1 initialization failed\n");
// 		HAL_Delay(1000);
// 	}
//   setProximityGain(&apds9960_hi2c1, PGAIN_2X);
  
//   // Start running the APDS-9960 proximity sensor (no interrupts)
//   if ( enableProximitySensor(&apds9960_hi2c1, false) ) 
//   {
//     printf("Proximity sensor is now running\n");
//   } else 
//   {
//     printf("Proximity sensor is NOT running\n");
//   }

//   // Start running the APDS-9960 light sensor (no interrupts)
//   if ( enableLightSensor(&apds9960_hi2c1, false) ) 
//   {
//     printf("Light sensor is now running\n");
//   } else 
//   {
//     printf("Light sensor is NOT running\n");
//   }
//   // Start running the APDS-9960 gesture sensor engine
//   if ( enableGestureSensor(&apds9960_hi2c1, false) ) 
//   {
//     printf("Gesture sensor is now running\n");
//   } 
//   else 
//   {
//     printf("Gesture sensor is NOT running\n");
//   }
// //APDS9960 stop
// // 24LC512 start
//   //uint8_t config[10] = {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19};
//   if (u24lc512_write(&hi2c4,
//                     EPPROM_START_ADDR,
//                     (uint8_t*) img_on_head,
//                     sizeof(img_on_head)))
//   {
//     printf("24LC512 write OK");
//   }
//   else
//   {
//     printf("24LC512 write ERROR\n");
//   }
// // 24LC512 stop
  
//   osDelay(200);
//   /* Infinite loop */
//   for(;;)
//   {
// 		osDelay(1000);
//     printf("main_cnt=%lu\n",cntt++);
//     //ST7789_Init();
//     sprintf((char *)buff,"%li", cntt);
//     //ST7789_WriteString(3, 3, (const char *)buff, Font_7x10, RED, BLACK);
//     //HAL_GPIO_TogglePin(REZEV_I2S_SCKIN_GPIO_Port, REZEV_I2S_SCKIN_Pin);
//     //ST7789_Test(); osDelay(500); 
//     //continue;
//     uint16_t color = rand() % WHITE+1;
//     ST7789_DrawRectangle(0, 0, 239, 239, color);
//     ST7789_DrawRectangle(1, 1, 238, 238, color);
//     //PN532_test(&pn532);
// 		if(bmp280_read(&bmp280)) 
//     {
//       printf("Pressure: %lu Pa, Temperature: %li C\n", bmp280.fixed_pressure / 256, (int32_t) bmp280.fixed_temperature / 100);
//       // sprintf((char *)buff,"P = %lu Pa", (pressure*10)/256);
//       // ST7789_WriteString(10, 10, (const char *)buff, Font_11x18, YELLOW, BLACK);
//       // sprintf((char *)buff,"T = %li C", temperature);
//       // ST7789_WriteString(10, 30, (const char *)buff, Font_11x18, YELLOW, BLACK);
//     }
//     else
//     {
// 			printf("bmp280 reading failed\n");
// 			osDelay(1000);
// 		}

//     if(hdc1080_read(&hdc1080))
//     {
      
//       //printf("HDC1080 Humidity: %lu H, Temperature: %li C\n", humid_hdc, temp_hdc);
//       int32_t hum,tmp;
//       hum = (int32_t)(((float)hdc1080.fixed_humidity*10000.0)/65536.0);
//       tmp = (int32_t)((((float)hdc1080.fixed_temperature*16500.0)/65536.0)-4000.0);
//       printf("HDC1080 Humidity: %lu %%, Temperature: %lu C\n", hum, tmp);
      
//       sprintf((char *)buff,"T = %li C", tmp);
//       ST7789_WriteString(10, 50, (const char *)buff, Font_11x18, GREEN, BLACK);
//       sprintf((char *)buff,"H = %lu %%", hum);
//       ST7789_WriteString(10, 70, (const char *)buff, Font_11x18, GREEN, BLACK);
//     }
//     else
//     {
// 			printf("hdc1080 reading failed\n");
// 			osDelay(1000);
// 		}

//     if(ccs811_is_ready(&ccs811))
//     {
//       if(ccs811_read(&ccs811))
//       {
//         printf("CCS811 eCO2 = %u ppm, TVOC = %u ppb\n", ccs811.eco2, ccs811.tvoc);
        
//         // sprintf((char *)buff,"eCO2 = %5u ppm", eco2);
//         // ST7789_WriteString(10, 90, (const char *)buff, Font_11x18, MAGENTA, BLACK);
//         // sprintf((char *)buff,"TVOC = %5u ppb", tvoc);
//         // ST7789_WriteString(10, 110, (const char *)buff, Font_11x18, MAGENTA, BLACK);
//       }
//       else
//       {
//         printf("hdc1080 reading failed\n");
//         osDelay(1000);
//       }
//     }
    
//     if(mpu9250_read_ag(&mpu9250))
//     {
//       printf("acc_x = %06i, acc_y = %06i, acc_z = %06i\n", mpu9250.acc_x, mpu9250.acc_y, mpu9250.acc_z);
//       printf("gyr_x = %06i, gyr_y = %06i, gyr_z = %06i\n", mpu9250.gyr_x, mpu9250.gyr_y, mpu9250.gyr_z);
      
//       sprintf((char *)buff,"Acc X=%06i, Y=%06i, Z=%06i", mpu9250.acc_x, mpu9250.acc_y, mpu9250.acc_z);
//       ST7789_WriteString(10, 130, (const char *)buff, Font_7x10, RED, BLACK);
//       sprintf((char *)buff,"Gyr X=%06i, Y=%06i, Z=%06i", mpu9250.gyr_x, mpu9250.gyr_y, mpu9250.gyr_z);
//       ST7789_WriteString(10, 150, (const char *)buff, Font_7x10, RED, BLACK);

//       sprintf((char *)buff,"Temp=%06i", mpu9250.temp);
//       ST7789_WriteString(130, 30, (const char *)buff, Font_7x10, RED, BLACK);
//     }
//     else
//     {
//       printf("mpu9250 reading failed\n");
//       osDelay(1000);
//     }
    
//     if(mpu9250_is_ready_mag(&mpu9250))
//     {
//       if(mpu9250_read_mag(&mpu9250))
//       {
//         printf("mag_x = %06i, mag_y = %06i, mag_z = %06i\n", mpu9250.mag_x, mpu9250.mag_y, mpu9250.mag_z);
        
//         sprintf((char *)buff,"Mag X=%06i, Y=%06i, Z=%06i", mpu9250.mag_x, mpu9250.mag_y, mpu9250.mag_z);
//         ST7789_WriteString(10, 170, (const char *)buff, Font_7x10, RED, BLACK);
//       }
//       else
//       {
//         printf("mpu9250 reading failed\n");
//         osDelay(1000);
//       }
//     }
      
//     if ( readProximity(&apds9960_hi2c4) ) 
//     {
//       printf("\nProximityUP = %03i\n", apds9960_hi2c4.proximity_level);
//       sprintf((char *)buff,"Proximity UP=%03i", apds9960_hi2c4.proximity_level);
//       ST7789_WriteString(10, 190, (const char *)buff, Font_7x10, LIGHTBLUE, BLACK);
//     }
//     if ( readProximity(&apds9960_hi2c1) ) 
//     {
//       printf("ProximityDN= %03i\n", apds9960_hi2c1.proximity_level);
//       sprintf((char *)buff,"DWN = %03i", apds9960_hi2c1.proximity_level);
//       ST7789_WriteString(140, 190, (const char *)buff, Font_7x10, LIGHTBLUE, BLACK);
//     }


//     // Read the light levels (ambient, red, green, blue)
//     if (  !readAmbientLight(&apds9960_hi2c4) ||
//           !readRedLight(&apds9960_hi2c4) ||
//           !readGreenLight(&apds9960_hi2c4) ||
//           !readBlueLight(&apds9960_hi2c4) ) 
//     {
//       printf("Error reading light values hi2c4\n");
//     } 
//     else 
//     {
//       printf("Ambient= %05i, R= %05i, G= %05i, B= %05i\n",
//              apds9960_hi2c4.clear_level,
//              apds9960_hi2c4.red_level,
//              apds9960_hi2c4.green_level,
//              apds9960_hi2c4.blue_level);
//       sprintf((char *)buff,"A=%05i,R=%05i,G=%05i,B=%05i",
//              apds9960_hi2c4.clear_level,
//              apds9960_hi2c4.red_level,
//              apds9960_hi2c4.green_level,
//              apds9960_hi2c4.blue_level);
//         ST7789_WriteString(5, 210, (const char *)buff, Font_7x10, WHITE, BLACK);
//     }
//     // Read the light levels (ambient, red, green, blue)
//     if (  !readAmbientLight(&apds9960_hi2c1) ||
//           !readRedLight(&apds9960_hi2c1) ||
//           !readGreenLight(&apds9960_hi2c1) ||
//           !readBlueLight(&apds9960_hi2c1) ) 
//     {
//       printf("Error reading light values hi2c4\n");
//     } 
//     else 
//     {
//       printf("Ambient= %05i, R= %05i, G= %05i, B= %05i\n",
//              apds9960_hi2c1.clear_level,
//              apds9960_hi2c1.red_level,
//              apds9960_hi2c1.green_level,
//              apds9960_hi2c1.blue_level);
//       sprintf((char *)buff,"A=%05i,R=%05i,G=%05i,B=%05i",
//              apds9960_hi2c1.clear_level,
//              apds9960_hi2c1.red_level,
//              apds9960_hi2c1.green_level,
//              apds9960_hi2c1.blue_level);
//       ST7789_WriteString(5, 225, (const char *)buff, Font_7x10, WHITE, BLACK);
//     }

//     if ( isGestureAvailable(&apds9960_hi2c4) ) 
//     {
//       switch ( readGesture(&apds9960_hi2c4) ) 
//       {
//         case DIR_UP:
//           //printf("c4   UP\n"); osDelay(1000);
//           break;
//         case DIR_DOWN:
//           //printf("c4 DOWN\n"); osDelay(1000);
//           break;
//         case DIR_LEFT:
//          // printf("c4 LEFT\n"); osDelay(1000);
//           break;
//         case DIR_RIGHT:
//           //printf("c4 RIGHT\n"); osDelay(1000);
//           break;
//         case DIR_NEAR:
//           //printf("c4 NEAR\n"); osDelay(1000);
//           break;
//         case DIR_FAR:
//           //printf("c4 FAR\n"); osDelay(1000);
//           break;
//         default:
//           //printf("c4 NONE\n");
//           break;
//       }
//     }
    
//     if ( isGestureAvailable(&apds9960_hi2c1) ) 
//     {
//       switch ( readGesture(&apds9960_hi2c1) ) 
//       {
//         case DIR_UP:
//           //printf("c1   UP\n"); osDelay(1000);
//           break;
//         case DIR_DOWN:
//           //printf("c1 DOWN\n"); osDelay(1000);
//           break;
//         case DIR_LEFT:
//           //printf("c1 LEFT\n"); osDelay(1000);
//           break;
//         case DIR_RIGHT:
//           //printf("c1 RIGHT\n"); osDelay(1000);
//           break;
//         case DIR_NEAR:
//           //printf("c1 NEAR\n"); osDelay(1000);
//           break;
//         case DIR_FAR:
//           //printf("c1 FAR\n"); osDelay(1000);
//           break;
//         default:
//           //printf("c1 NONE\n");
//           break;
//       }
//     }

//   // 24LC512
// /*TODO: не отображает дисплей    if (u24lc512_read(&hi2c4,
//                       EPPROM_START_ADDR,
//                       bbuff,
// 		                  sizeof(bbuff)))
//     {
//       for (uint16_t i=0; i < sizeof(bbuff); i++ )
//       {
//         if (bbuff[i] != img_on_head[i]) 
//         {
//           printf("EPPROM data ERROR\n");
//           printf("buff[%i]=%i\n",i , bbuff[i]);
//           printf("reff[%i]=%i\n",i , img_on_head[i]);
//           osDelay(1000);
//         }
//       }
//     }
//     else
//     {
//       printf("24LC512 read ERROR\n");
//     }
// */
//     //qspi_read(&hqspi);
//     //qspi_test(&hqspi);
// 	}
//   /* USER CODE END StartDebugTask */
// }

/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 2012-2013  SEGGER Microcontroller GmbH & Co KG           *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : SWO.c
Purpose : Simple implementation for output via SWO for Cortex-M processors.
          It can be used with any IDE. This sample implementation ensures that
          output via SWO is enabled in order to gurantee that the application
          does not hang.
--------  END-OF-HEADER  ---------------------------------------------
*/

/*********************************************************************
*
*       Defines for Cortex-M debug unit
*/
#define ITM_STIM_U32 (*(volatile unsigned int*)0xE0000000)    // Stimulus Port Register word acces
#define ITM_STIM_U8  (*(volatile         char*)0xE0000000)    // Stimulus Port Register byte acces
#define ITM_ENA      (*(volatile unsigned int*)0xE0000E00)    // Trace Enable Ports Register
#define ITM_TCR      (*(volatile unsigned int*)0xE0000E80)    // Trace control register

/*********************************************************************
*
*   Checks if SWO is set up. If it is not, return,
*    to avoid program hangs if no debugger is connected.
*   If it is set up, print a character to the ITM_STIM register
*    in order to provide data for SWO.
* Parameters
*   c:    The Chacracter to be printed.
* Notes
*   Additional checks for device specific registers can be added.
*/
// DEBUG printf SWO f=119808000Hz
//TODO: непредсказуемо зависает камень
// int _write(int fd, char* ptr, int len)
// {
//   (void)fd;
//   int i = 0;
//     //
//   // Check if ITM_TCR.ITMENA is set
//   //
//   if ((ITM_TCR & 1) == 0) {
//     return 0;
//   }
//   //
//   // Check if stimulus port is enabled
//   //
//   if ((ITM_ENA & 1) == 0) {
//     return 0;
//   }

//   while (ptr[i] && (i < len)) 
//   {
//     while ((ITM_STIM_U8 & 1) == 0); //TODO: здесь не тормозить
//     ITM_STIM_U8 = ptr[i];
//     i++;
//   }
//   return len;
// }

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
/*
int _write(int fd, char* ptr, int len)
{
  (void)fd;
  //HAL_GPIO_TogglePin(REZEV_I2S_SCKIN_GPIO_Port, REZEV_I2S_SCKIN_Pin);
  HAL_UART_Transmit(&huart8, (uint8_t *) ptr, len, HAL_MAX_DELAY);
  //HAL_UART_Transmit_DMA(&huart2, (uint8_t *) ptr, len);
  return len;
}
*/
static volatile bool is_debug_task = false;
static volatile uint32_t oldtik;
static volatile bool is_need_print = false;

int _write(int fd, char* ptr, int len)
{
  (void)fd;

  // uint8_t * buff_print[SIZE_BUFF_PRINT_NUM][SIZE_BUFF_PRINT_SRT];
  // buff_print = pbuf;

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

// int __io_putchar(int ch)
// {
//   HAL_UART_Transmit(&huart8, (uint8_t *)&ch, 1, 1);
//   return ch;
// }

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

// static void IntToUnicode(uint32_t value, uint8_t * pbuf, uint8_t len)
// {
//   uint8_t idx = 0;

//   for (idx = 0; idx < len; idx++)
//   {
//     if (((value >> 28)) < 0xA)
//     {
//       pbuf[2 * idx] = (value >> 28) + '0';
//     }
//     else
//     {
//       pbuf[2 * idx] = (value >> 28) + 'A' - 10;
//     }

//     value = value << 4;

//     pbuf[2 * idx + 1] = 0;
//   }
// }

// void printkeymap(void)
// {
//   if (keymap_left == NULL)
//   {
//     printf("keymap_left == NULL\n");
//     return;
//   }
//   uint32_t layer, rows, cols;
//   for(layer =0; layer < layers_total; layer++)
//   {
//     //printf("\nlayer=%lu\n", layer);
//     printf("\n");
 
//     for ( rows = 0; rows < MATRIX_LOCAL_ROWS; rows++)
//     {
//       for(cols =0; cols <MATRIX_LOCAL_COLS; cols++)
//       {
//         //printf("%lX, ",keymap_left[layer][rows][cols]);
//         uint32_t index = layer*MATRIX_LOCAL_ROWS*MATRIX_LOCAL_COLS+rows*MATRIX_LOCAL_COLS+cols;
//         printf("[%03lu]%lu, ", index, keymap_left[index]);
//         //printf("%lu, ", keymap_right[layer][rows][cols]);
//       }
//       printf("\n");
//     }
//   }
// }
