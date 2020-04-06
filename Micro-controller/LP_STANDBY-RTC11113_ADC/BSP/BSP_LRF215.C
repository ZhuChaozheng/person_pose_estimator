
#include "BSP_LRF215.h"
extern UART_HandleTypeDef huart1;
extern float Roll, Pitch, Yaw;
	u16 ID_num = 0,sum=0,DataLength = 8; 
	u8 data_to_send[20];
	
void LRF215_Init()
{
		GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = LRF215_DATA_EN|LRF215_AWAKE;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
//  HAL_GPIO_Init(LRF215_PORT, &GPIO_InitStruct);
//	  GPIO_InitStruct.Pin = (GPIO_PIN_5);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	GPIO_InitStruct.Pin = LRF215_CMD_EN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 
}

void LRF215_EnterLowMode()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	HAL_UART_MspInit(&huart1);
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	LRF215_Init();
	
	LRF215_CMD_MO();
	HAL_Delay(10);
	u8 TX_Buff[50]={0};
	
  TX_Buff[0]=0xfc;
	TX_Buff[1]=0x01;
	TX_Buff[2]=0x69;
	TX_Buff[3]=0x08;
	TX_Buff[4]=LRF215_LOW_MODEL;
	TX_Buff[5]=(TX_Buff[0]+TX_Buff[1]+TX_Buff[2]+TX_Buff[3]+TX_Buff[4])&0xff;
	HAL_UART_Transmit(&huart1,TX_Buff,6,5);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,LRF215_DATA_EN|LRF215_AWAKE,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,LRF215_CMD_EN,GPIO_PIN_SET);
	HAL_UART_MspDeInit(&huart1);
	
	GPIO_InitStructure.Pin =  LRF215_DATA_EN|LRF215_AWAKE;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.Pin =  LRF215_CMD_EN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	  __HAL_RCC_GPIOA_CLK_DISABLE();
	  __HAL_RCC_GPIOB_CLK_DISABLE();
}


void LRF215_EnterNorMode(void)
{

	LRF215_Init();
//    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
//		HAL_GPIO_WritePin(LRF215_PORT,LRF215_AWAKE,GPIO_PIN_SET);
		HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA,LRF215_AWAKE,GPIO_PIN_RESET);

}



void Send_Data_Zigbee()
{
	unsigned char i=0;
	unsigned char _cnt=0;
	unsigned int _temp;
  

	data_to_send[_cnt++]=0xAA; //帧头
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=BYTE1(ID_num);//设备地址 设备号
	data_to_send[_cnt++]=BYTE0(ID_num);
	
  data_to_send[_cnt++]=	BYTE1(DataLength);//数据域长度 
	data_to_send[_cnt++]=	BYTE0(DataLength);
	
	_temp = (int)(Yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);//偏航角yaw
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Roll*100);
	data_to_send[_cnt++]=BYTE1(_temp);//翻滚角
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0-(int)(Pitch*100);
	data_to_send[_cnt++]=BYTE1(_temp);//俯仰角
	data_to_send[_cnt++]=BYTE0(_temp);
	
	//和校验
	for(i=0;i<_cnt;i++)
		sum+= data_to_send[i];
	data_to_send[_cnt++]=BYTE1(sum);
	data_to_send[_cnt++]=BYTE0(sum);
	
	//串口发送数据
	HAL_UART_Transmit_IT(&huart1,data_to_send,_cnt);
}

void LRF215_SendMode(void)
{
	//enter data send mode
	LRF215_EnterNorMode();
	HAL_Delay(100);
	HAL_UART_MspInit(&huart1);//重新初始化串口
//	 printf( "Roll: %f, Pitch %f\r\n",
//              Roll, Pitch);
//			printf("X=%d, Y=%d, Z=%d\r\n\r\n", ACCdata[1], ACCdata[0], ACCdata[2]);
	LRF215_DATA_MO();
	Send_Data_Zigbee();
}
