/*
 * Motor.cpp
 *
 *  Created on: 2019/05/01
 *      Author: 裕汰
 */

#include <Servo/Servo.hpp>

void Servo::setDuty(float d)
{

			__HAL_TIM_SetCompare(Mhandler,channel,map(d,0,100,0,Mhandler->Instance->ARR));//CRRにデューティを入れる

}

void Servo::Begin()
{
	HAL_TIM_PWM_Start(Mhandler,channel) ;       //HALの仕様
	HAL_GPIO_WritePin(GPIOx,pin,GPIO_PIN_RESET);
}

void Servo::SetFrequency(long freq)
{
		float freqency=freq;
		unsigned short counterperiod=0;
		unsigned short Prescaler=0;
		float ajustfreq=0;
		long sysclock=0;
		if(Mhandler==&htim1)
		{
			sysclock=HAL_RCC_GetPCLK2Freq();
		}
		else
		{
			sysclock=HAL_RCC_GetPCLK1Freq();
		}
		while(ajustfreq!=freqency)
				{
					Prescaler++;
				for(counterperiod=0;counterperiod<65535;counterperiod++)
				{
					ajustfreq=(float)sysclock/((float)Prescaler*((float)counterperiod+1));
					if(ajustfreq==freqency)
					{
						break;
					}
				}

				}
		Mhandler->Instance->PSC=(unsigned short)Prescaler-1;
		Mhandler->Instance->ARR=(unsigned short)counterperiod;
}
