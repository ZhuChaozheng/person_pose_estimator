#include "stm32l0xx_hal.h"
#include "main.h"
#include "gpio.h"
#define LRF215_CMD_EN  GPIO_PIN_0
#define LRF215_DATA_EN GPIO_PIN_7
#define LRF215_AWAKE   GPIO_PIN_6

#define LRF215_CMD_MO()    HAL_GPIO_WritePin(GPIOB,LRF215_CMD_EN,GPIO_PIN_SET);\
                          HAL_GPIO_WritePin(GPIOA,LRF215_DATA_EN,GPIO_PIN_RESET);
											 
#define LRF215_DATA_MO()   HAL_GPIO_WritePin(GPIOA,LRF215_DATA_EN,GPIO_PIN_SET);\
                          HAL_GPIO_WritePin(GPIOB,LRF215_CMD_EN,GPIO_PIN_RESET);
#define LRF215_MODEL 0x02                           //�豸���ͣ�0X01:Э���� 0X02���ն�   0x03:�͹����ն�
#define LRF215_LOW_MODEL 0x03                           //�豸���ͣ�0X01:Э���� 0X02���ն�   0x03:�͹����ն�


void LRF215_SendMode(void);
void LRF215_EnterLowMode(void);