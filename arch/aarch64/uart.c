/*------------------------------------------------------*/
/*!
 *      @file   uart.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  uart for log during boot process
 *      @note   do not need configure pin settings and clock settins etc because this OS runs on QEMU.
 */
/*------------------------------------------------------*/

#include <stdint.h>
#include "../../include/config.h"

/*------------------------------------------------------*/
/*! @brief  register base address
 */
#define UART0_BASE              (0x09000000u + KERNEL_OFFSET)

/*------------------------------------------------------*/
/*! @brief  register structure
 */
struct pl011_reg_t {
    volatile uint32_t dr;
    volatile uint32_t rsr_ecr;
    volatile uint32_t reserved[4];
    volatile uint32_t fr;
};

/*------------------------------------------------------*/
/*! @brief  base register
 */
static struct pl011_reg_t * const p_reg_base = (struct pl011_reg_t *)UART0_BASE;

/*------------------------------------------------------*/
/*! @brief  send a character
 */
void uart_putc(char c)
{
    // wait send enable state
    while ((p_reg_base->fr & (1 << 5)) != 0) {
        // nope
    }

    // send character
    p_reg_base->dr = c;
}

/*------------------------------------------------------*/
/*! @brief  send a string
 */
void uart_puts(const char *str)
{
    while (*str) {
        uart_putc(*str);
        str++;
    }
}
