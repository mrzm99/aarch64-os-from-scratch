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
#include "../arch/aarch64/mmu.h"
#include "mm/pmm.h"
#include "mm/vmm.h"

extern void enable_exception();
extern void set_vector_table_el1(unsigned long vector_table_el1);
extern char vector_table_el1[];

/*------------------------------------------------------*/
/*! @brief  kernel entry point
 */
void kernel_main()
{
    printk("kernel_main() Initialize kernel start.\n");

    // init physical memory management
    pmm_init();
    printk("pmm_init() completed.\n");

    // set vector table
    set_vector_table_el1((unsigned long)vector_table_el1);
    printk("set_vector_table_el1() completed.\n");

    // set ttbl1_el1
    pgd_t *p_pgd = setup_page_tables();
    printk("setup_page_tables() completed.\n");

    // update page table for kernel
    uint64_t phys_pgd = v2p(p_pgd);
    mmu_set_kernel_page_table(phys_pgd);

    // invalidate unnecessary page table for user space
    mmu_invalidate_user_page_table();

    printk("Switched to Full 4-level Page Table! TTBR0 is now disabled.\n");

    while (1);
}
