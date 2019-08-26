/*
 * Application.cpp
 *
 *  Created on: 2019/05/04
 *      Author: 裕汰
 */

#include "Application.hpp"
#include "DefineOrder.h"
extern unsigned char RxFIFO_Data[4];
extern CAN_RxHeaderTypeDef RXmsg;
extern bool CanRxFlag;

#define ORDER_BIT_Pos 6U

float App::RestoreData()
{


		((unsigned char*)&temp_data)[0]=RxFIFO_Data[0];
		((unsigned char*)&temp_data)[1]=RxFIFO_Data[1];
		((unsigned char*)&temp_data)[2]=RxFIFO_Data[2];
		((unsigned char*)&temp_data)[3]=RxFIFO_Data[3];

	return temp_data;
}


void App::SetDuty(float duty)
{
	switch(node_id)
	{
	case 1:
		plow->S1.setDuty(duty);
		break;
	case 2:
			plow->S2.setDuty(duty);
			break;
	case 3:
			plow->S3.setDuty(duty);
			break;
	case 4:
			plow->S4.setDuty(duty);
			break;
	case 5:
			plow->S5.setDuty(duty);
			break;
	case 6:
			plow->S6.setDuty(duty);
			break;
	case 7:
			plow->S7.setDuty(duty);
			break;
	case 8:
			plow->S8.setDuty(duty);
			break;
	}
}

void App::TaskShift()
{
	if(CanRxFlag)
	{
		if((RXmsg.ExtId)>>ORDER_BIT_Pos==SET_DUTY)//命令IDがSET_DUTYに一致したら
		{
			this->node_id=RXmsg.ExtId&0xF;//ノードIDを代入
			SetDuty(RestoreData());//4バイトに分けていたデータを復元してデューティとしてセット
		}
		CanRxFlag=false;
	}
}


