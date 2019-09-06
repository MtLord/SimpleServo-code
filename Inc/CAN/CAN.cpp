/*
 * CAN.cpp
 *
 *  Created on: 2018/12/28
 *      Author: 裕汰
 */
#include "CAN.hpp"
#include "LowlayerHandel.hpp"
#include "can.h"
unsigned char RxFIFO_Data[4]={0,};
CAN_RxHeaderTypeDef RXmsg;

int rx_led=0;
#define MASKID_L 0xF0<<9|0x01<<2
#define FILTERID_L 0x40<<9|0x1<<2 //stdidと命令IDの上位ビットでマスクをかける

#define TOGGLE_TX_LED HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_6);
#define TOGGLE_RX_LED  HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
#define SET_ERROR_LED HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
#define RESET_ERRORLED HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);

bool CanRxFlag=false;
void FilterConfig()
{
	CAN_FilterTypeDef  sFilterConfig;
	sFilterConfig.FilterIdHigh=0x0000;
	sFilterConfig.FilterIdLow=FILTERID_L;
	sFilterConfig.FilterMaskIdHigh=0x0000;
	sFilterConfig.FilterMaskIdLow=MASKID_L;//stdidのデータフレームだけ受信
	sFilterConfig.FilterFIFOAssignment=CAN_FILTER_FIFO0;//受信フィルタをFIFO0に設定
	sFilterConfig.FilterBank=0; //フィルタバンク番号を設 0-13
	sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT; //フィルタスケールExtIdまで
	sFilterConfig.FilterMode=CAN_FILTERMODE_IDMASK; //マスクモード
	sFilterConfig.FilterActivation=ENABLE; //フィルタ有効
//	sFilterConfig.SlaveStartFilterBank=14;

	if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig)!=HAL_OK)
		{
			printf("filter config error!");
		}
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
 {
	   HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RXmsg,RxFIFO_Data);
	   CanRxFlag=true;

		   TOGGLE_RX_LED;




 }
void CanBus::SetError()
{
	error_code=(hcan.Instance->ESR>>4)&0b111;
}
short CanBus::Send(unsigned long ID,unsigned char DLC,unsigned char *data)
{
	uint32_t mailbox_num;
			 HAL_CAN_StateTypeDef state = hcan.State;
			 uint32_t TSR = hcan.Instance->TSR;
			 if ((state == HAL_CAN_STATE_READY) ||(state == HAL_CAN_STATE_LISTENING))
			   {
				  if (((TSR & CAN_TSR_TME0) != 0U) || ((TSR & CAN_TSR_TME1) != 0U) ||((TSR & CAN_TSR_TME2) != 0U))//どれかのメールボックスが空いていたら
				  {
					  mailbox_num = (TSR & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos; //空きメールボックス番号を取得
					  if (mailbox_num > 2)
					  {
					         /* Update error code */
					         hcan.ErrorCode |= HAL_CAN_ERROR_INTERNAL;
					         error_flag=true;
					         this->SetError();
					         return -1;
					   }

					  if(this->IDE==CAN_ID_STD)
					  {
						  	  hcan.Instance->sTxMailBox[mailbox_num].TIR=ID<<21|this->RTR;
					  }
					  else//ext id
					  {
						  hcan.Instance->sTxMailBox[mailbox_num].TIR=ID<<3U|IDE|RTR;
					  }
					  hcan.Instance->sTxMailBox[mailbox_num].TDTR = DLC;
					  hcan.Instance->sTxMailBox[mailbox_num].TDHR=(uint32_t)data[7]<<24|(uint32_t)data[6]<<16|(uint32_t)data[5]<<8|(uint32_t)data[4];//メールボックス上位レジスタにセット
					  hcan.Instance->sTxMailBox[mailbox_num].TDLR=(uint32_t)data[3]<<24|(uint32_t)data[2]<<16|(uint32_t)data[1]<<8|(uint32_t)data[0];
					  hcan.Instance->sTxMailBox[mailbox_num].TIR|=1;//送信ビットセット
					  return 0;
					  Txok=true;
					  error_flag=false;
				  }
				  else
				  {
					  hcan.ErrorCode |= HAL_CAN_ERROR_PARAM;
					  error_flag=true;
					  this->SetError();
					  return -1;
				  }

			   }
			 else
			 {
				 hcan.ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;
				 error_flag=true;
				 this->SetError();
				    return -1;
			 }
			  if(Txok)
			  {
			 	if(tx_led>5)
			 	{
			 		TOGGLE_TX_LED;
			 		tx_led=0;
			 	}
			 	else
			 	{
			 		tx_led++;
			 	}
			  }
		Txok=false;
}

