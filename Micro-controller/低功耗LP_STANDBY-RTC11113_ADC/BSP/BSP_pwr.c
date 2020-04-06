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


/* Includes ------------------------------------------------------------------*/
#include "BSP_pwr.h"



/****************************************************************************
* 名    称： void LowPower_Config(void)
* 功    能： 低功耗初始化
* 入口参数：
* 出口参数：
* 说    明：
****************************************************************************/
void LowPower_Config(void)
{
//#ifdef __rtc_H	
    // 禁止唤醒时钟计数
    //HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

    // 低功耗模式下RTC 允许访问 ，RTC唤醒需要
    HAL_PWR_EnableBkUpAccess();
//#else
//	HAL_PWR_DisableBkUpAccess();
//#endif
    // 禁止 PVD
    HAL_PWR_DisablePVD();

    // 关闭VREFINT	少3uA
    HAL_PWREx_EnableUltraLowPower();
    // 忽略VREFINT 加快启动速度
    HAL_PWREx_EnableFastWakeUp();
	
    // 唤醒后选择哪个启动时钟源
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

    // 降低核心工作电压，需要根据实际工作频率设置，建议在Cubemx内的RCC项目中配置
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
}


/****************************************************************************
* 名    称：
* 功    能： 进入STOP模式
* 入口参数：
* 出口参数：
* 说    明：
****************************************************************************/
void LowPower_enter_stop(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;
   

	//UART1;	减少16uA
    HAL_UART_MspDeInit(&huart1);

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
//    __HAL_RCC_GPIOH_CLK_DISABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    // 禁用 调试端口，下面2句函数功能一样！
	HAL_DBGMCU_DisableDBGStopMode();
//	HAL_DBGMCU_DBG_DisableLowPowerConfig(DBGMCU_STOP);
	
	// 若需要在低功耗模式下调试程序，可以打开此函数
	//HAL_DBGMCU_DBG_EnableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);
	
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	// 清除唤醒标志
	
    /*## Setting the RTC Wake up time ########################################*/
    /*  RTC Wakeup Interrupt Generation:
    Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI))
    Wakeup Time = Wakeup Time Base * WakeUpCounter
    = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI)) * WakeUpCounter
    ==> WakeUpCounter = Wakeup Time / Wakeup Time Base

    To configure the wake up timer to 4s the WakeUpCounter is:
    RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16
    Wakeup Time Base = 16 /(~32768Hz) = ~488.3us
    Wakeup Time = ~4s = 0,410ms  * WakeUpCounter
    ==> WakeUpCounter = 4s/488.3us = 8192 */

    // rtc 唤醒采用LSE时钟，第一参数，计数溢出唤醒；第二参数：进行16分配
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 4000, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

    /* Enter Stop Mode */
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}




/****************************************************************************
* 名    称：
* 功    能： 进入STANDBY模式
* 入口参数：
* 出口参数：
* 说    明：
****************************************************************************/
void LowPower_enter_standby(void)
{
    printf("=== Power standby ===\r\n");
    HAL_Delay(50);

    LED_OFF;

    //UART1;	减少16uA
    HAL_UART_MspDeInit(&huart1);

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();

    // 禁用 调试端口 少800uA，下面2句函数功能一样！
    //HAL_DBGMCU_DisableDBGStandbyMode();
	HAL_DBGMCU_DBG_DisableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);
	
	// 若需要在低功耗模式下调试程序，可以打开此函数
	//HAL_DBGMCU_DBG_EnableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);
	
    // PIN1 连接到 PWR->CR 的 PWR_FLAG_WU 标记
    // 用于待机模式IO唤醒，唤醒后判断WKUP按钮状态
	//		WKUP pin 1 (PA00) if enabled.
	//      WKUP pin 2 (PC13) if enabled.
	//      WKUP pin 3 (PE06) if enabled, for stm32l07xxx and stm32l08xxx devices only.
	//      WKUP pin 3 (PA02) if enabled, for stm32l031xx devices only.
//    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	// 清除唤醒标志
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);	// 清除待机标志
	   
    /*## Setting the RTC Wake up time ########################################*/
    /*  RTC Wakeup Interrupt Generation:
    Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI))
    Wakeup Time = Wakeup Time Base * WakeUpCounter
    = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI)) * WakeUpCounter
    ==> WakeUpCounter = Wakeup Time / Wakeup Time Base

    To configure the wake up timer to 4s the WakeUpCounter is:
    RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16
    Wakeup Time Base = 16 /(~32768Hz) = ~488.3us
    Wakeup Time = ~4s = 0,410ms  * WakeUpCounter
    ==> WakeUpCounter = 4s/488.3us = 8192  */
#ifdef __rtc_H
	// 该功能可以用Cubme RTC内配置，代码可以参考rtc.c文件内
    // rtc 唤醒采用LSE时钟，第一参数，计数溢出唤醒；第二参数：进行16分配
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 8192, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
#endif
    /* Enter Standby Mode */
    HAL_PWR_EnterSTANDBYMode();
	
}



/****************************************************************************
* 名    称：
* 功    能： 退出STOP模式，初始化外设
* 入口参数：
* 出口参数：
* 说    明：
****************************************************************************/
void LowPower_WakeUp(void)
{

//#ifdef __rtc_H
    // 禁止唤醒时钟计数，禁用后，需要重新执行 HAL_RTCEx_SetWakeUpTimer_IT 打开计数
    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
//#endif
    // 如果使用非MSI时钟，请恢复默认时钟
    SystemClock_Config();
     HAL_PWREx_EnableUltraLowPower();
    // 清除唤醒标记 减少重复唤醒电流
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	
	}

	// 清除待机标志
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); 
	}
	
	
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	//__GPIOC_CLK_ENABLE();
	//__GPIOH_CLK_ENABLE();
	
    LED_ON;


}


/********************** (C) COPYRIGHT Waiman *********  END OF FILE  *********/

