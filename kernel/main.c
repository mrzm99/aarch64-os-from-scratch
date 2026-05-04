/*------------------------------------------------------*/
/*!
 *      @file   main.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  kernl entry point
 *      @note
 */
/*------------------------------------------------------*/
#include <stdint.h>
#include "../include/util.h"
#include "mm/pmm.h"

/*------------------------------------------------------*/
/*! @brief  kernel entry point
 */
void kernel_main()
{
    printk("Hello World\n");

    pmm_init();
    uint64_t *p_page = pmm_get_free_page();
    printk("(1) p_page = 0x%x\n", (unsigned long)p_page);
    pmm_free_page(p_page);
    p_page = pmm_get_free_page();
    printk("(2) p_page = 0x%x\n", (unsigned long)p_page);


    while (1);
}
