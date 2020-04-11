/**
  ******************************************************************************
  * @file    BSP_debug.c
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


/* Includes ------------------------------------------------------------------*/
#include "BSP_debug.h"
#include "string.h"
#include "stdio.h"
#include "main.h"


__IO uint8_t SYSTEM_DEBUG = 0;


#ifdef DEBUG_USE_RTT	//=================================== use RTT

#pragma message("//=========================== DEBUG use RTT")
static char _acUpBuffer1  [BUFFER_SIZE_UP];
static char _acDownBuffer1[BUFFER_SIZE_DOWN];

void RTT_Init()
{
SEGGER_RTT_ConfigUpBuffer(1,"Log", &_acUpBuffer1[0],  BUFFER_SIZE_UP,  SEGGER_RTT_MODE_NO_BLOCK_SKIP);
SEGGER_RTT_ConfigUpBuffer(1,"Log", &_acDownBuffer1[0],BUFFER_SIZE_DOWN,SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}




#else
#ifdef DEBUG_USE_SWO	//=================================== use SWO
// SWO Initializing variables
#if defined(__STM32L0xx_HAL_H) || defined(__STM32F0xx_HAL_H)
#error	"STM32L0 & STM32F0 SWO not supported"
#endif

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

#if   defined ( __ICCARM__ )	/*!< IAR Compiler */

#elif defined(__ARMCC_VERSION) 	/*!< MDK Compiler */
struct __FILE {
    int handle; /* Add whatever needed */
};
#endif
FILE __stdout;
FILE __stdin;
volatile int32_t ITM_RxBuffer=ITM_RXBUFFER_EMPTY;


#else					//=================================== use UART	
#include "usart.h"
#endif




//===========================	PutChar
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
#ifdef DEBUG_USE_SWO
#pragma message("//=========================== DEBUG use SWO")
    if (DEMCR & TRCENA) {
        while (ITM_Port32(0) == 0);
        ITM_Port8(0) = ch;
    }
#else
#if defined (__BSP_UART_H)
#pragma message("//=========================== DEBUG use UART interrupt")
    huart1.pTxBuffPtr[huart1.TxXferSize]=ch;
    huart1.TxXferSize++;
    if( huart1.TxXferSize >= USART_MAX_LEN ) huart1.TxXferSize=0;
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);

#else
#pragma message("//=========================== DEBUG use UART poll")
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);

//  Cortex-M3
#if defined (__STM32F1xx_HAL_H) || defined (__STM32L1xx_HAL_H)
    huart1.Instance->DR = (uint8_t)ch;
#endif
	
//  Cortex-M0
#if defined (__STM32F0xx_HAL_H) || defined (__STM32L0xx_HAL_H)
    huart1.Instance->TDR = (uint8_t)ch;
#endif


#endif
#endif
    return ch;
}



//===========================	PopChar
#ifdef __GNUC__
#define POPCHAR_PROTOTYPE int __io_getchar(int ch)
#else
#define POPCHAR_PROTOTYPE int fgetc(FILE *f)
#endif
POPCHAR_PROTOTYPE
{
#ifdef DEBUG_USE_SWO
    while(ITM_CheckChar() == 0)
    {
    }
    return ITM_ReceiveChar();
#else
	return -1;// Ã»ÓÐ×Ö·û·µ»Ø
#endif
}



#endif // <- DEBUG_USE_RTT END //


/********************** (C) COPYRIGHT Waiman *********  END OF FILE  *********/
