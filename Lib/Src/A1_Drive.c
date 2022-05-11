//
// Created by Trisoil on 2022/5/3.
//

#include "A1_Drive.h"
#include "dma.h"
#include "usart.h"

extern DMA_HandleTypeDef hdma_usart1_tx;
union Motor{
		uint8_t data[34];
		Send_Data Tx_Message;
}Motor_u;

void Usart1_TX_DMA_Init(void)
{
	//enable the DMA transfer for the receiver request
	//使能DMA串口接收
	SET_BIT(huart1.Instance->CR3, USART_CR3_DMAT);
}

void usart1_tx_dma_enable(uint8_t *data, uint16_t len)
{

	//disable DMA
	//失效DMA
	__HAL_DMA_DISABLE(&hdma_usart1_tx);
	while(hdma_usart1_tx.Instance->CR & DMA_SxCR_EN)
	{
		__HAL_DMA_DISABLE(&hdma_usart1_tx);
	}
	hdma_usart1_tx.Instance->PAR = (uint32_t) & (USART1->DR);
	//clear flag
	//清除标志位
	__HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_HISR_TCIF7);
	__HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_HISR_HTIF7);

	//set data address
	//设置数据地址
	hdma_usart1_tx.Instance->M0AR = (uint32_t)(data);
	//set data length
	//设置数据长度
	hdma_usart1_tx.Instance->NDTR = len;

	SET_BIT(huart1.Instance->CR3, USART_CR3_DMAT);
	//enable DMA
	//使能DMA
	__HAL_DMA_ENABLE(&hdma_usart1_tx);
	HAL_GPIO_TogglePin(LED_R_GPIO_Port,LED_R_Pin);
}


uint32_t crc32_core(uint32_t *ptr, uint32_t len) {
	uint32_t xbit = 0;
	uint32_t data = 0;
	uint32_t CRC32 = 0xFFFFFFFF;
	const uint32_t dwPolynomial = 0x04c11db7;
	for (uint32_t i = 0; i < len; i++) {
		xbit = 1 << 31;
		data = ptr[i];
		for (uint32_t bits = 0; bits < 32; bits++) {
			if (CRC32 & 0x80000000) {
				CRC32 <<= 1;
				CRC32 ^= dwPolynomial;
			} else
				CRC32 <<= 1;
			if (data & xbit)
				CRC32 ^= dwPolynomial;
			xbit >>= 1;
		}
	}
	return CRC32;
}

void Mode_Control(int ID,int Mode)
{
	Motor_u.Tx_Message.mode = Mode;
	Motor_u.Tx_Message.Motor_ID = ID;
	Motor_u.Tx_Message.start[0] = 0xFE;
	Motor_u.Tx_Message.start[1] = 0xEE;
	Motor_u.Tx_Message.CRCdata =  crc32_core((uint32_t *) &Motor_u.data, 7);
	usart1_tx_dma_enable((uint8_t *) &Motor_u.data, 34);
}

//void Speed_Mode_Control(int ID, int Speed)
//{
//	Tx_Message.Motor_ID = ID;
//	Tx_Message.mode = 10;
//	Tx_Message.T = 0;
//}