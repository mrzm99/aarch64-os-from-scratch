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
    mrs x1, mpidr_el1
    and x1, x1, #3
    cbz x1, boot_core

halt:
    wfe
    b halt

boot_core:
    ldr x0, =__stack_top
    mov sp, x0

    bl kernel_main

    b halt

.section ".bss"
.align 4
__stack_bottom:
    .skip 1 * 1024 * 1024
__stack_top:
