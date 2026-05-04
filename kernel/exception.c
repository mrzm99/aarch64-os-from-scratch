/*------------------------------------------------------*/
/*!
 *      @file   exception.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  exception handler
 *      @note
 */
/*------------------------------------------------------*/

#include "../include/util.h"

/*------------------------------------------------------*/
/*! @brief  Synchronous Exception Handler (Current EL with SPx)
 */
void sync_el1_handler(unsigned long esr, unsigned long elr)
{
    printk("\n========== KERNEL PANIC ==========\n");
    printk("ELR_EL1 (Address) : 0x%x\n", elr);
    printk("ESR_EL1 (Syndrome): 0x%x\n", esr);
    printk("System Halted.\n");

    while (1);
}

/*------------------------------------------------------*/
/*! @brief  IRQ/vIRQ Exception Handler (Current EL with SPx)
 */
void irq_el1_handler()
{
    printk("IRQ Received!\n");
}

/*------------------------------------------------------*/
/*! @brief  FIQ/vFIQ Exception Handler (Current EL with SPx)
 */
void fiq_el1_handler()
{
    printk("FIQ Received!\n");
}

/*------------------------------------------------------*/
/*! @brief  SError/vSError Exception Handler (Current EL with SPx)
 */
void serr_el1_handler()
{
    printk("SError Received!\n");
}

/*------------------------------------------------------*/
/*! @brief  Dummy Exception Handler
 */
void dummy_el1_handler()
{
    printk("Dummy Received!\n");
}
