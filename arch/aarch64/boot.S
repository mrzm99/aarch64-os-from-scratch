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
    and x1, x1, #0xFF               /* get Core ID              */

    cbz x1, boot_core               /* jump boot process if core id is zero (boot core) */

halt:
    wfe                             /* wait for event */
    b halt

boot_core:
    msr daifset, #0xF               /* set Debug mask / SError mask / IRQ mask / FIQ mask bit */
    msr spsel, #1                   /* select SP_EL1 as stack pointer                         */

    ldr x20, =0xFFFF000000000000    /* kernel address space offset */

    ldr x0, =__stack_top            /* get stack pointer (virtual address)         */
    sub x0, x0, x20                 /* convert virtual address to physical address */
    mov sp, x0                      /* set stack pointer                           */

    /* bss zero clear */
    ldr x0, =__bss_start            /* get bss segment start address               */
    sub x0, x0, x20                 /* convert virtual address to physical address */
    ldr x1, =__bss_end              /* get bss segment end address                 */
    sub x1, x1, x20                 /* convert virtual address to physical address */

bss_clr_loop:
    cmp x0, x1                      /* bss clear completed check      */
    b.ge build_early_pgt            /* jump to build_early_pgt        */
    str xzr, [x0], #8               /* zero clear and advance pointer */
    b bss_clr_loop                  /* jump to bss_clr_loop           */

build_early_pgt:
    ldr x0, =boot_pgd               /* get pgd address                             */
    sub x0, x0, x20                 /* convert virtual address to physical address */
    ldr x1, =boot_pud               /* get pud address                             */
    sub x1, x1, x20                 /* convert virtual address to physical address */

    /* clear pgd + pud(8K) */
    mov x2, x0
    mov x3, #8192
clear_pgd:
    str xzr, [x2], #8
    subs x3, x3, #8
    b.ne clear_pgd

    /* set 1G block to PUD[0] (Device Memory) */
    ldr x2, =0x6000000000000701     /* PXN=1, UXN=1, AF=1, Attr0 */
    str x2, [x1, #0]                /* set PUD entry             */

    /* set 1G block t PUD[1] (Normal Memory) */
    ldr x2, =0x40000705             /* AF=1, SH=3, Attr1 */
    str x2, [x1, #8]                /* set PUD entry     */

    orr x2, x1, #3                  /* x2 = PUD address (physical) | VALID(1) | TALBE(1) */
    str x2, [x0, #0]                /* set PGD entry                                     */

    /* set mair_el1a*/
    ldr x1, =((0xFF << 8) | 0x00)
    msr mair_el1, x1

    /* set tcr_el1 (virtual address has 48bit and 4K paging (ttbr0, ttbr1)) */
    ldr x1, =0x00000005B5103510
    msr tcr_el1, x1

    /* set pgd to ttbr0, ttbr1 */
    msr ttbr0_el1, x0
    msr ttbr1_el1, x0

    /* enable MMU */
    tlbi vmalle1is
    dsb ish
    isb

    mrs x1, sctlr_el1
    orr x1, x1, #1                  /* Enable the MMU           */
    orr x1, x1, #(1<<2)             /* Data cache enable        */
    orr x1, x1, #(1<<12)            /* Instruction cache enable */
    msr sctlr_el1, x1
    isb

    ldr x0, =__stack_top            /* set stack pointer (virtual address) */
    mov sp, x0

    /* jump to kernel main */
    ldr x0, =kernel_main
    br x0

    /* never reach */
    b halt

.section ".bss"
.align 12
boot_pgd:                           /* reserve 4K byte for PGD used by boot process */
    .skip 4096

boot_pud:                           /* reserve 4K byte for PUD used by boot process */
    .skip 4096

.align 4
__stack_bottom:
    .skip 1 * 1024 * 1024           /* reserve 1M byte for SP_EL1 */
__stack_top:
