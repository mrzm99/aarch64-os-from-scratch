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
#include "../arch/aarch64/uart.h"
#include "../drivers/irq/gicv3.h"
#include "../drivers/timer/arm_timer.h"
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
    printk("[%s:%] Initialize kernel start.\n", __FUNCTION__, __LINE__);

    // init physical memory management
    pmm_init();
    printk("pmm_init() completed.\n");

    // set vector table
    set_vector_table_el1((unsigned long)vector_table_el1);
    printk("set_vector_table_el1() completed.\n");

    //init virtual memory management
    vmm_init();
    printk("vmm_init() completed.\n");

    // creating direct mapping
    pgd_t *p_pgd = setup_page_tables();
    printk("setup_page_tables() completed.\n");
    printk("kernel pgd = 0x%x\n", (uint64_t)p_pgd);

    // update page table for kernel
    uint64_t phys_pgd = v2p(p_pgd);
    mmu_set_kernel_page_table(phys_pgd);

    printk("set kernel page table\n");

    // invalidate unnecessary page table for user space
    mmu_invalidate_user_page_table();

    // uart ioremap
    uart_ioremap();

    printk("uart_ioremap() completed.\n");

    gicv3_init();

    printk("gicv3_init completed.\n");

    arm_timer_init();
    enable_exception();


    while (1) {
        asm volatile("wfi");
    }
}
