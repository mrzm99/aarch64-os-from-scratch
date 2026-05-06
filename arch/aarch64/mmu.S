/*------------------------------------------------------*/
/*!
 *      @file   mmu.S
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  Memory Management Unit
 *      @note
 */
/*------------------------------------------------------*/

.global mmu_set_kernel_page_table
.global mmu_invalidate_user_page_table

/*------------------------------------------------------*/
/*! @brief  void mmu_set_user_page_table(uint64_t pgd)
 */
mmu_set_kernel_page_table:
    msr ttbr1_el1, x0
    tlbi vmalle1is
    dsb ish
    isb
    ret

/*------------------------------------------------------*/
/*! @brief  void mmu_invalidate_user_page_table(void)
 */
mmu_invalidate_user_page_table:
    msr ttbr0_el1, xzr
    tlbi vmalle1is
    dsb ish
    isb
    ret

