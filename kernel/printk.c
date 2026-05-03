/*------------------------------------------------------*/
/*!
 *      @file   printk.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  print function for kernel
 *      @note
 */
/*------------------------------------------------------*/

#include <stdarg.h>
#include "../arch/aarch64/uart.h"

/*------------------------------------------------------*/
/*! @brief  print number
 */
static void print_number(unsigned int val, int base)
{
    char buff[32];
    int i = 0;

    if (val == 0) {
        uart_putc('0');
        return;
    }

    while (val > 0) {
        int rem = val % base;

        if (rem < 10) {
            buff[i] = '0' + rem;
        } else {
            buff[i] = 'a' + rem;
        }

        i++;
        val = val / base;
    }

    while (i > 0) {
        i--;
        uart_putc(buff[i]);
    }
}

/*------------------------------------------------------*/
/*! @brief  printk
 */
void printk(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                // string
                case 's': {
                    const char *p_str = va_arg(args, const char *);
                    uart_puts(p_str);
                    break;
                }

                // 10 base integer
                case 'd': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number(val, 10);
                    break;
                }

                // 16 base integer
                case 'x': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number(val, 16);
                    break;
                }

                // % itself
                case '%': {
                    uart_putc('%');
                    break;
                }

                // other (as it is)
                default: {
                    uart_putc('%');
                    uart_putc(*format);
                    break;
                }
            }
        } else {
            uart_putc(*format);
        }

        format++;
    }

    va_end(args);
}
