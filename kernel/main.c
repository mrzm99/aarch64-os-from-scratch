/*------------------------------------------------------*/
/*!
 *      @file   main.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  kernl entry point
 *      @note
 */
/*------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "../include/util.h"
#include "../include/config.h"
#include "../arch/aarch64/mmu.h"
#include "../arch/aarch64/uart.h"
#include "../arch/aarch64/cpu.h"
#include "../drivers/irq/gicv3.h"
#include "../drivers/timer/arm_timer.h"
#include "include/kernel.h"
#include "mm/pmm.h"
#include "mm/vmm.h"

extern void set_vector_table_el1(unsigned long vector_table_el1);
extern char vector_table_el1[];

/*------------------------------------------------------*/
/*! @brief  Test Task A
 */
static void task_a_main(void)
{
    while (1) {
        printk("Task A is running...\n");
        for (volatile int i = 0; i < 10000000; i++);
    }
}
/*------------------------------------------------------*/
/*! @brief  Test Task B
 */
static void task_b_main(void)
{
    while (1) {
        printk("Task B is running...\n");
        for (volatile int i = 0; i < 10000000; i++);
    }
}

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

    task_init();
    create_task(NULL);
    current = task[0];

    create_task(task_a_main);
    create_task(task_b_main);

    arm_timer_init();
    enable_exception();

    while (1) {
        asm volatile("wfi");
    }
}
