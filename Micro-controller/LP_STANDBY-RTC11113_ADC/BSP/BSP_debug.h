/**
  ******************************************************************************
  * @file    BSP_debug.h
  * @author  Waiman
  * @version V1.0.0
  * @date    21-January-2018
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 Waiman </center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_DEBUG_H__
#define __BSP_DEBUG_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifndef _BSP_CONFIG_
//__________________________ F
#if defined (STM32F303xC)
#include "stm32f3xx_hal.h"
#endif
#if defined (STM32F103xB)
#include "stm32f1xx_hal.h"
#endif
	 
//__________________________ F0	 
#if defined (STM32F072xB) || defined (STM32F030x8)
#include "stm32f0xx_hal.h"
#endif
	 
//__________________________ L
#if defined (STM32L433xx)
#include "stm32l4xx_hal.h"
#endif
#if defined (STM32L151xB) || defined (STM32L151xBA)
#include "stm32l1xx_hal.h"
#endif
#if defined (STM32L053xx)
#include "stm32l0xx_hal.h"
#endif
//////////////////////////////////////////////////////////
	 
	 
#else	//////////////////////////////////////////////////
#include "BSP_pcb_config.h"
#endif




extern __IO uint8_t SYSTEM_DEBUG;


// 不打开以下两个宏定义，默认使用UART1作为Printf输出通道
//#define DEBUG_USE_SWO
//#define DEBUG_USE_RTT	// 打开此宏定义，自动取消SWO输出
	 
#ifdef DEBUG_USE_RTT
#undef DEBUG_USE_SWO
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

extern void RTT_Init(void);

#endif



#ifndef _BSP_CONFIG_
// 如果有定义 _BSP_CONFIG_ 全部功能交由 BSP_pcb_config.h 控制

#define BUILD_DEBUG		(1)
#define BUILD_LOG		(1)


#if (BUILD_DEBUG==1)
  #ifndef DEBUG_PRINT
    #define DEBUG_PRINT(...)   do {(void) printf(__VA_ARGS__);}while(0)
  #endif
#else
  #ifndef DEBUG_PRINT
    #define DEBUG_PRINT(...)
  #endif
#endif


#if (BUILD_LOG==1)
  #ifndef LOG
    #define LOG(...)   do {(void) printf(__VA_ARGS__);}while(0)
  #endif
#else
  #ifndef LOG
    #define LOG(...)
  #endif
#endif


#define PRINT_ALWAYS(...)			do {(void) printf(__VA_ARGS__);}while(0)

	
// 带调试选择打开功能
//#ifdef BUILD_DEBUG
//#define DEBUG(format, ...)  if(SYSTEM_DEBUG==1)printf (format, ##__VA_ARGS__)
//#else
//#define DEBUG(format,...) 
//#endif
//
//#ifdef BUILD_LOG
//#define LOG(format, ...)  if(SYSTEM_DEBUG==2) printf (format, ##__VA_ARGS__)
//#else
//#define LOG(format,...) 
//#endif


#endif	/* _BSP_CONFIG_*/


#ifdef __cplusplus
}
#endif
#endif /* __BSP_DEBUG_H__ */


/********************** (C) COPYRIGHT Waiman *********  END OF FILE  *********/

