/**
  ******************************************************************************
  * @file    BSP_pwr.c
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
#ifndef __BSP_PWM_H__
#define __BSP_PWM_H__

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
//__________________________ user includes ___________
#include "usart.h"
#include "rtc.h"


#else	//____________________________________________
#include "BSP_pcb_config.h"
#endif

	 
	 
/* Public API functions  -----------------------------------------------------*/
extern	void 	LowPower_Config(void);
extern	void	LowPower_enter_stop(void);
extern	void 	LowPower_enter_standby(void);
extern	void	LowPower_WakeUp(void);


#ifdef __cplusplus
}
#endif
#endif /* __LCD_GDC21310_H__ */


/************************ (C) COPYRIGHT Waiman *****END OF FILE****/

