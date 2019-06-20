/*
 * printf.h
 *
 *  Created on: 2019/05/01
 *      Author: —T‘¿
 */

#ifndef INC_PRINTF_H_
#define INC_PRINTF_H_
#include "usart.h"
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifdef __cplusplus
 extern "C" {
#endif
void __io_putchar(uint8_t ch)
{
	HAL_UART_Transmit(&huart2, &ch, 1, 1);
}
int __io_getchar(void)
{
	uint8_t c;
	HAL_UART_Receive(&huart2, &c, sizeof(c),0xF);
	return c;
}
#ifdef __cplusplus
}
#endif



#endif /* INC_PRINTF_H_ */
