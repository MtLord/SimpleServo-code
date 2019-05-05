/*
 * Motor.hpp
 *
 *  Created on: 2019/05/01
 *      Author: —T‘¿
 */

#ifndef INC_MOTOR_MOTOR_HPP_
#define INC_MOTOR_MOTOR_HPP_
#include "stm32f3xx.h"
#include "tim.h"
class Servo
{
private:

	TIM_HandleTypeDef *Mhandler;
	GPIO_TypeDef *GPIOx;
	unsigned short pin;
	unsigned int channel;
	long map(float x, long in_min, long in_max, long out_min, long out_max)
		{
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}
public:
	Servo(TIM_HandleTypeDef *Mhand,GPIO_TypeDef *_GPIOx,unsigned short _pin,unsigned int _channel):
		Mhandler(Mhand),GPIOx(_GPIOx),pin(_pin),channel(_channel)
	{

	}
	void setDuty(float d);
	void SetFrequency(long freq);
	void Begin();
};



#endif /* INC_MOTOR_MOTOR_HPP_ */
