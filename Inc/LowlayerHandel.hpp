/*
 * LowlayerHandel.hpp
 *
 *  Created on: 2019/05/01
 *      Author: —T‘¿
 */

#ifndef INC_LOWLAYERHANDEL_HPP_
#define INC_LOWLAYERHANDEL_HPP_
#include "Servo/Servo.hpp"
#include "CAN/CAN.hpp"
#include "stm32f303x8.h"
#include "tim.h"
#include "main.h"
class LowlayerHandelTypedef
{
public:
	Servo S1,S2,S3,S4,S5,S6,S7,S8;
	CanBus extcan;
	LowlayerHandelTypedef():
	S1(&htim3,GPIOB,GPIO_PIN_5, TIM_CHANNEL_2),
	S2(&htim3,GPIOB,GPIO_PIN_4, TIM_CHANNEL_1),
	S3(&htim2,GPIOB,GPIO_PIN_3, TIM_CHANNEL_2),
	S4(&htim1,GPIOA,GPIO_PIN_10, TIM_CHANNEL_3),
	S5(&htim1,GPIOA,GPIO_PIN_9, TIM_CHANNEL_2),
	S6(&htim1,GPIOA,GPIO_PIN_8, TIM_CHANNEL_1),
	S7(&htim3,GPIOB,GPIO_PIN_0, TIM_CHANNEL_4),
	S8(&htim3,GPIOB,GPIO_PIN_1, TIM_CHANNEL_3),
	extcan(CAN_ID_EXT,CAN_RTR_DATA)
	{

	}
	void LEDCheck()
	{
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_6);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
		HAL_Delay(500);
	}
	void DebugServo(int num,float duty)
	{
		switch(num)
		{
		case 1:
			S1.setDuty(duty);
			break;
		case 2:
			S2.setDuty(duty);
			break;
		case 3:
			S3.setDuty(duty);
			break;
		case 4:
			S4.setDuty(duty);
			break;
		case 5:
			S5.setDuty(duty);
			break;
		case 6:
			S6.setDuty(duty);
			break;
		case 7:
			S7.setDuty(duty);
			break;
		case 8:
			S8.setDuty(duty);
			break;
		}
	}
};



#endif /* INC_LOWLAYERHANDEL_HPP_ */
