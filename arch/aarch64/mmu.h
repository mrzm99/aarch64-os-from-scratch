/*------------------------------------------------------*/
/*!
 *      @file   mmu.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  MMU operation
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __MMU_H__
#define __MMU_H__

#include <stdint.h>

/*------------------------------------------------------*/
/*! @brief  prototype
 */
void mmu_set_kernel_page_table(uint64_t pgd);
void mmu_invalidate_user_page_table(void);

/*------------------------------------------------------*/
/*! @brief  flush all TLB
 */
inline void flush_tlb_all(void)
{
    asm volatile (
        "tlbi vmalle1is\n"
        "dsb ish\n"
        "isb\n"
        ::: "memory"
    );
}

/*------------------------------------------------------*/
/*! @brief  flush specified page
 */
inline void flush_tlb_page(uint64_t va)
{
    // get address
    uint64_t addr = va >> 12;

    // flush page
    asm volatile (
        "tlbi vaae1is, %0\n"
        "dsb ish\n"
        "isb\n"
        :: "r"(addr) : "memory"
    );
}

#endif
