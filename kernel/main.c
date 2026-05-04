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
#include "mm/vmm.h"

extern void enable_mmu(pgd_t *p_pgd);
extern void enable_exception();
extern void set_vector_table_el1(unsigned long vector_table_el1);
extern char vector_table_el1[];

/*------------------------------------------------------*/
/*! @brief  kernel entry point
 */
void kernel_main()
{
    printk("Hello World\n");

    pmm_init();

    printk("pmm_init() completed.\n");

    set_vector_table_el1((unsigned long)vector_table_el1);

    printk("set_vector_table_el1() completed.\n");

    pgd_t *p_pgd = setup_page_tables();

    printk("setup_page_tables() completed.\n");

    enable_mmu(p_pgd);

    printk("enable_mmu() completed.\n");

    while (1);
}
