/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
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
#include "can.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  hcan.Instance = CAN;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SJW = CAN_SJW_1TQ;
  hcan.Init.BS1 = CAN_BS1_11TQ;
  hcan.Init.BS2 = CAN_BS2_4TQ;
  hcan.Init.TTCM = DISABLE;
  hcan.Init.ABOM = DISABLE;
  hcan.Init.AWUM = DISABLE;
  hcan.Init.NART = DISABLE;
  hcan.Init.RFLM = DISABLE;
  hcan.Init.TXFP = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(canHandle->Instance==CAN)
  {
  /* USER CODE BEGIN CAN_MspInit 0 */

  /* USER CODE END CAN_MspInit 0 */
    /* CAN clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN interrupt Init */
    HAL_NVIC_SetPriority(CAN_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN_TX_IRQn);
    HAL_NVIC_SetPriority(CAN_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN_SCE_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN_SCE_IRQn);
  /* USER CODE BEGIN CAN_MspInit 1 */

  /* USER CODE END CAN_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN)
  {
  /* USER CODE BEGIN CAN_MspDeInit 0 */

  /* USER CODE END CAN_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN_SCE_IRQn);
  /* USER CODE BEGIN CAN_MspDeInit 1 */

  /* USER CODE END CAN_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void CAN_filter(CAN_HandleTypeDef *phcan, int filterNumber, unsigned int filter1, unsigned int filter2,unsigned int filter3,unsigned int filter4)
{
		
	CAN_FilterConfTypeDef  FilterConfig;
	FilterConfig.FilterNumber = filterNumber;
  FilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
  FilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
  FilterConfig.FilterIdHigh = filter1 <<5;
  FilterConfig.FilterIdLow = filter2<<5;
  FilterConfig.FilterMaskIdHigh = filter3 << 5;
  FilterConfig.FilterMaskIdLow = filter4 << 5;
  FilterConfig.FilterFIFOAssignment = CAN_FIFO0;
  FilterConfig.FilterActivation = ENABLE;
  FilterConfig.BankNumber = 14;
	
	  if (HAL_CAN_ConfigFilter(phcan, &FilterConfig) != HAL_OK)
  {
    // Filter configuration Error 
    Error_Handler();
  }
}
void CAN_init(CAN_HandleTypeDef *phcan)
{
  static CanTxMsgTypeDef        TxMessage;
  static CanRxMsgTypeDef        RxMessage;
	phcan->Instance = CAN;

	phcan->pTxMsg = &TxMessage;
  phcan->pRxMsg = &RxMessage;



  /*##-3- Configure Transmission process #####################################*/
	//Fill
  phcan->pTxMsg->StdId = 0xAA;
  phcan->pTxMsg->ExtId = 0x00;
  phcan->pTxMsg->RTR = CAN_RTR_DATA;
  phcan->pTxMsg->IDE = CAN_ID_STD;

  phcan->pTxMsg->DLC = 8;
	for(int i =0; i<8;i++)
	{
		phcan->pTxMsg->Data[i] = 0x00;
	}
	//HAL_CAN_Transmit_IT(hcan);
}

void CAN_send(CAN_HandleTypeDef *phcan, unsigned int id, uint8_t data[8])
{
	phcan->pTxMsg->StdId = id;
	phcan->pTxMsg->DLC = 8;
	for(int i=0; i<8;i++){ 
		phcan->pTxMsg->Data[i]=data[i];
	}
	
	phcan->pTxMsg->IDE=CAN_ID_STD;
	HAL_CAN_Transmit_IT(phcan);
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan){
	for(int i =0;i<8;i++){
		can_receive[i] = hcan->pRxMsg->Data[i];
	}
	  if (HAL_CAN_Receive_IT(hcan, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan){
	//while(1)
	{
		
	}
}
void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef *hcan){
	static int a=0;

}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
