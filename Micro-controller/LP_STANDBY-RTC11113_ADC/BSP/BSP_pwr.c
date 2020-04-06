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
* ��    �ƣ� void LowPower_Config(void)
* ��    �ܣ� �͹��ĳ�ʼ��
* ��ڲ�����
* ���ڲ�����
* ˵    ����
****************************************************************************/
void LowPower_Config(void)
{
//#ifdef __rtc_H	
    // ��ֹ����ʱ�Ӽ���
    //HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

    // �͹���ģʽ��RTC ������� ��RTC������Ҫ
    HAL_PWR_EnableBkUpAccess();
//#else
//	HAL_PWR_DisableBkUpAccess();
//#endif
    // ��ֹ PVD
    HAL_PWR_DisablePVD();

    // �ر�VREFINT	��3uA
    HAL_PWREx_EnableUltraLowPower();
    // ����VREFINT �ӿ������ٶ�
    HAL_PWREx_EnableFastWakeUp();
	
    // ���Ѻ�ѡ���ĸ�����ʱ��Դ
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

    // ���ͺ��Ĺ�����ѹ����Ҫ����ʵ�ʹ���Ƶ�����ã�������Cubemx�ڵ�RCC��Ŀ������
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
}


/****************************************************************************
* ��    �ƣ�
* ��    �ܣ� ����STOPģʽ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
****************************************************************************/
void LowPower_enter_stop(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;
   

	//UART1;	����16uA
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
    // ���� ���Զ˿ڣ�����2�亯������һ����
	HAL_DBGMCU_DisableDBGStopMode();
//	HAL_DBGMCU_DBG_DisableLowPowerConfig(DBGMCU_STOP);
	
	// ����Ҫ�ڵ͹���ģʽ�µ��Գ��򣬿��Դ򿪴˺���
	//HAL_DBGMCU_DBG_EnableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);
	
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	// ������ѱ�־
	
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

    // rtc ���Ѳ���LSEʱ�ӣ���һ����������������ѣ��ڶ�����������16����
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 4000, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

    /* Enter Stop Mode */
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}




/****************************************************************************
* ��    �ƣ�
* ��    �ܣ� ����STANDBYģʽ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
****************************************************************************/
void LowPower_enter_standby(void)
{
    printf("=== Power standby ===\r\n");
    HAL_Delay(50);

    LED_OFF;

    //UART1;	����16uA
    HAL_UART_MspDeInit(&huart1);

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();

    // ���� ���Զ˿� ��800uA������2�亯������һ����
    //HAL_DBGMCU_DisableDBGStandbyMode();
	HAL_DBGMCU_DBG_DisableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);
	
	// ����Ҫ�ڵ͹���ģʽ�µ��Գ��򣬿��Դ򿪴˺���
	//HAL_DBGMCU_DBG_EnableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);
	
    // PIN1 ���ӵ� PWR->CR �� PWR_FLAG_WU ���
    // ���ڴ���ģʽIO���ѣ����Ѻ��ж�WKUP��ť״̬
	//		WKUP pin 1 (PA00) if enabled.
	//      WKUP pin 2 (PC13) if enabled.
	//      WKUP pin 3 (PE06) if enabled, for stm32l07xxx and stm32l08xxx devices only.
	//      WKUP pin 3 (PA02) if enabled, for stm32l031xx devices only.
//    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	// ������ѱ�־
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);	// ���������־
	   
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
	// �ù��ܿ�����Cubme RTC�����ã�������Բο�rtc.c�ļ���
    // rtc ���Ѳ���LSEʱ�ӣ���һ����������������ѣ��ڶ�����������16����
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 8192, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
#endif
    /* Enter Standby Mode */
    HAL_PWR_EnterSTANDBYMode();
	
}



/****************************************************************************
* ��    �ƣ�
* ��    �ܣ� �˳�STOPģʽ����ʼ������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
****************************************************************************/
void LowPower_WakeUp(void)
{

//#ifdef __rtc_H
    // ��ֹ����ʱ�Ӽ��������ú���Ҫ����ִ�� HAL_RTCEx_SetWakeUpTimer_IT �򿪼���
    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
//#endif
    // ���ʹ�÷�MSIʱ�ӣ���ָ�Ĭ��ʱ��
    SystemClock_Config();
     HAL_PWREx_EnableUltraLowPower();
    // ������ѱ�� �����ظ����ѵ���
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	
	}

	// ���������־
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

