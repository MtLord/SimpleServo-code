/*
 * CAN.cpp
 *
 *  Created on: 2018/12/28
 *      Author: 裕汰
 */
#include "CAN.hpp"
#include "LowlayerHandel.hpp"
#include "can.h"
unsigned char RxFIFO_Data[6];
CAN_RxHeaderTypeDef RXmsg;

bool CanRxFlag=false;
void FilterConfig()
{
	CAN_FilterTypeDef  sFilterConfig;
	sFilterConfig.FilterIdHigh=0x0000;
	sFilterConfig.FilterIdLow=0x0004;
	sFilterConfig.FilterMaskIdHigh=0x0000;
	sFilterConfig.FilterMaskIdLow=0x0004;//extdidのデータフレームだけ受信
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
 }

short CanBus::Send(unsigned long ID,unsigned char DLC,unsigned char *data)
{
	Txmsg.DLC=DLC;
	Txmsg.ExtId=ID;
	Txmsg.IDE=this->IDE;
	Txmsg.RTR=this->RTR;
	while(Txok==false)
				{
					if((hcan.Instance->TSR>>26&0x1)==1)//TME0 is Empty
									{
										HAL_CAN_AddTxMessage(&hcan,&Txmsg,data,(uint32_t*)CAN_TX_MAILBOX0);
										if(HAL_CAN_AddTxMessage(&hcan,&Txmsg,data,(uint32_t*)CAN_TX_MAILBOX0)!=HAL_OK)
										{
											error_flag=true;
											error_code=hcan.Instance->ESR>>4&0b111;
											return -1;
										}
										else
										{
											Txok=true;
											error_flag=false;
										}

									}
					else if((hcan.Instance->TSR>>27&0x1)==1)//TME1 is empty
									{
										HAL_CAN_AddTxMessage(&hcan,&Txmsg,data,(uint32_t*)CAN_TX_MAILBOX1);
										if(HAL_CAN_AddTxMessage(&hcan,&Txmsg,data,(uint32_t*)CAN_TX_MAILBOX1)!=HAL_OK)
										{
											error_flag=true;
											error_code=hcan.Instance->ESR>>4&0b111;
											return -1;
										}
										else
										{
											Txok=true;
											error_flag=false;
										}

									}
					else if((hcan.Instance->TSR>>28&0x1)==1)//TME2 is empty
									{
										HAL_CAN_AddTxMessage(&hcan,&Txmsg,data,(uint32_t*)CAN_TX_MAILBOX2);
										if(	HAL_CAN_AddTxMessage(&hcan,&Txmsg,data,(uint32_t*)CAN_TX_MAILBOX2)!=HAL_OK)
										{
											error_flag=true;
											error_code=hcan.Instance->ESR>>4&0b111;
											return -1;
										}
										else
										{
											Txok=true;
											error_flag=false;
										}

									}

									if(error_flag)
									{
										switch(error_code)
										{
										case 1:
											printf("staff error\n\r");
											break;
										case 2:
											printf("form error\n\r");
											break;
										case 3:
											printf("ACK error\n\r");
											break;
										case 4:
											printf("bit recessive error\n\r");
											break;
										case 5:
											printf("bit dominant error\n\r");
											break;
										case 6:
											printf("CRC error\n\r");
											break;
										}
									}
									else if(Txok)
									{
										Txok=false;
										HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_4);
										return 0;
									}
				}
}

