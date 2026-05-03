/*------------------------------------------------------*/
/*!
 *      @file   boot.S
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  boot process
 *      @note
 */
/*------------------------------------------------------*/

.section ".text.boot"
.global _start

_start:
    mrs x1, mpidr_el1               /* read MPIDR_EL1 register  */
    and x1, x1, #0xFF               /* extract Core ID          */
    cbz x1, boot_core               /* jump boot process if core id is zero */

halt:
    wfe
    b halt

boot_core:
    msr daifset, #0xF               /* set Debug mask / SError mask / IRQ mask / FIQ mask bit */
    msr spsel, #1                   /* selct SP_EL1 as stack pointer */
    ldr x0, =__stack_top            /* set stack pointer */
    mov sp, x0

    bl bss_clr                      /* zero clar bss segment */

    bl kernel_main                  /* jump kernel_main */

    b halt                          /* should never reach */

bss_clr:
    ldr x0, =__bss_start
    ldr x1, =__bss_end

    cmp x0, x1
    b.eq bss_clr_ext

bss_clr_loop:
    str xzr, [x0], #8

    cmp x0, x1
    b.lt bss_clr_loop

bss_clr_ext:
    ret

.section ".bss"
.align 4
__stack_bottom:
    .skip 1 * 1024 * 1024           /* reserve 1M byte for SP_EL1 */
__stack_top:
