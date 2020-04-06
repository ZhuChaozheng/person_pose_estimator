/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"
#include "LIS3DH.h"
/* USER CODE BEGIN Includes */
#include "BSP_debug.h"
#include "BSP_pwr.h"
#include "math.h"
#include "adc.h"
#include "BSP_LRF215.H"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
void power_exit_stop(void);
void power_enter_stop(void);
/* USER CODE END PV */
int ACCdata[3] = {0,0,0};
float Roll, Pitch, Yaw;
uint8_t low_Power_flag = 0;
int ADC_Sampling_gap = 1;//sample per 5min
ADC_HandleTypeDef             AdcHandle;
#define PI               3.14159265358979f
uint32_t uwADCxConvertedValue = 0;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
__IO uint8_t sec = 0;
__IO uint8_t wake= 0;
	uint32_t res = 0;
/* USER CODE END 0 */
float OLd_Yaw[3]={0};
u8 Data_Emit_Cnt=5;
u8 TX_Buff[100];
extern UART_HandleTypeDef huart1;
int main(void)
{

	ADC_ChannelConfTypeDef sConfig;
	u8 err_code;
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */


  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
// RTC初始化，初始化以后，RTC定时器会定时唤醒单片机，将RTC初始化屏蔽以后，就可以测试系统休眠功耗
  MX_RTC_Init();
	MX_ADC_Init();
	MX_ADC_Read();
	
  /* USER CODE BEGIN 2 */
	//单片机初始化所有管脚，初始化以后，单片机会减少泄漏电流
	LowPower_Config();
  /* USER CODE END 2 */
	//LIS3DH管脚初始化，初始化以后，可以与传感器进行通信
  I2C_GPIO_Configuration();
	
	//通过IIC初始化LIS3DH传感器
	err_code = LIS3DH_Init();
	if( err_code!=NO_ERROR)
	{
		err_code = LIS3DH_Init();
//		printf("\r\nLIS3DH Init is failed! \r\n");
	}
//	  printf("\r\nLIS3DH Init is succeed! \r\n");    
	LRF215_EnterLowMode();
	HAL_UART_MspDeInit(&huart1);

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	while (1)
	{
    //单片机进入停止模式，屏蔽掉RTC初始化以后，单片机长期处在STOP模式下，拔掉传感器和zigbbe，测试电流为1uA左右
		LowPower_enter_stop();
		//未屏蔽掉RTC初始化，RTC会将单片机从停止模式下唤醒，程序从这里开始执行，唤醒以后初始化单片机IIC管脚，单片机就可以与传感器
		//进行IIC通信了
		I2C_GPIO_Configuration();
		//单片机从STOP模式唤醒以后，进行相关的唤醒配置
		LowPower_WakeUp();
		//单片机读取LIS3DH数据，三轴加速度的值
		Collect_Data(ACCdata);	
		//计算PITH和ROLL的值
		Roll=atan2(ACCdata[0]*1.0f,ACCdata[2]*1.0f)*180/ PI;
		Pitch=atan2(ACCdata[1]*(1.0f),ACCdata[2]*(-1.0f))*180/ PI ;
		
//			if( ((fabs(OLd_Yaw[0]-Roll))>=5.0)||((fabs(OLd_Yaw[1]-Pitch))>=5.0)||( Data_Emit_Cnt>=5))
//			{
		//调用单片机串口，配置ZIGBBE，发送数据
		    LRF215_SendMode();
     //将IIC管脚初始化模拟输入，节省功耗 
        I2C_GPIO_DeInit();
				OLd_Yaw[0]=Roll;
				OLd_Yaw[1]=Pitch;
				Data_Emit_Cnt=0;
				
				HAL_Delay(10);
			//ZIGGBE进入低功耗模式
			LRF215_EnterLowMode();
			//
			ADC_Sampling_gap --;
			if(ADC_Sampling_gap == 0)
			{
				MX_ADC_Read();
				ADC_Sampling_gap = 1;
			}
			
			if(low_Power_flag == 1){
				MX_LED_Init();
			}
				
//			}
//			else
//			{
	
				
//				Data_Emit_Cnt++;
//			}		
//		 printf( "Roll: %f, Pitch %f\r\n",
//              Roll, Pitch);
//			printf("X=%d, Y=%d, Z=%d\r\n\r\n", ACCdata[1], ACCdata[0], ACCdata[2]);
		
		
		}
		// Stangdby 唤醒后，直接复位MCU
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		

  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Configure LSE Drive Capability 
    */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/*###########################################################################*/
// 只是分割线，可以在下面添加代码
/*###########################################################################*/

// 外部中断唤醒回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	wake=1;
}


// rtc周期中断回调函数
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	wake=2;
}
/*###########################################################################*/
// 只是分割线
/*###########################################################################*/
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
