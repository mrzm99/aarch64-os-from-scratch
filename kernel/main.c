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
#include "../include/config.h"
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
    printk("Hello from Higher Half!\n");

    pmm_init();
    printk("pmm_init() completed.\n");

    set_vector_table_el1((unsigned long)vector_table_el1);
    printk("set_vector_table_el1() completed.\n");

    pgd_t *p_pgd = setup_page_tables();
    //printk("setup_page_tables() completed.\n");

    uint64_t phys_pgd = v2p(p_pgd);
    asm volatile (
        "msr ttbr1_el1, %0\n"
        "tlbi vmalle1is\n"
        "dsb ish\n"
        "isb\n"
        :: "r"(phys_pgd) : "memory"
    );

    asm volatile (
        "msr ttbr0_el1, xzr\n"
        "tlbi vmalle1is\n"
        "dsb ish\n"
        "isb\n"
    );

    printk("Switched to Full 4-level Page Table! TTBR0 is now disabled.\n");

    while (1);
}
