/*------------------------------------------------------*/
/*!
 *      @file   mmu.S
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  Memory Management Unit
 *      @note
 */
/*------------------------------------------------------*/

.global enable_mmu

enable_mmu:
    ldr x1, =((0xFF << 8) | 0x00)
    msr mair_el1, x1

    ldr x1, =((16 << 0) | (0 << 14) | (1 << 8) | (1 << 10) | (3 << 12))
    msr tcr_el1, x1

    msr ttbr0_el1, x0

    tlbi vmalle1is
    dsb ish
    isb

    mrs x1, sctlr_el1
    orr x1, x1, #1
    orr x1, x1, #(1<<2)
    orr x1, x1, #(1<<12)
    msr sctlr_el1, x1
    isb

    ret
