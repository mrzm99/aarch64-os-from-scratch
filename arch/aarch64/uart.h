/*------------------------------------------------------*/
/*!
 *      @file   uart.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  uart for log during boot process
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __UART_H__
#define __UART_H__

void uart_putc(char c);
void uart_puts(const char *str);

#endif
