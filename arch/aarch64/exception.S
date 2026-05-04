/*------------------------------------------------------*/
/*!
 *      @file   exception.S
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  exception handle
 *      @note
 */
/*------------------------------------------------------*/

.section ".text"

.global enable_exception
.global disable_exception
.global set_vector_table_el1
.global vector_table_el1

enable_exception:
    msr daifclr, #0x3                           /* clear IRQ/FIQ mask */
    ret

disable_exception:
    msr daifset, #0xF                           /* set DAIF mask */
    ret

set_vector_table_el1:
    msr vbar_el1, x0
    ret

/* entry point into kernel */
.macro kernel_entry
    sub sp, sp, #256                            /**/
    stp x0,  x1,  [sp, #16 *  0]                /**/
    stp x2,  x3,  [sp, #16 *  1]                /**/
    stp x4,  x5,  [sp, #16 *  2]                /**/
    stp x6,  x7,  [sp, #16 *  3]                /**/
    stp x8,  x9,  [sp, #16 *  4]                /**/
    stp x10, x11, [sp, #16 *  5]                /**/
    stp x12, x13, [sp, #16 *  6]                /**/
    stp x14, x15, [sp, #16 *  7]                /**/
    stp x16, x17, [sp, #16 *  8]                /**/
    stp x18, x19, [sp, #16 *  9]                /**/
    stp x20, x21, [sp, #16 * 10]                /**/
    stp x22, x23, [sp, #16 * 11]                /**/
    stp x24, x25, [sp, #16 * 12]                /**/
    stp x26, x27, [sp, #16 * 13]                /**/
    stp x28, x29, [sp, #16 * 14]                /**/
    str x30,      [sp, #16 * 15]                /**/
.endm

/* exit point from kernel */
.macro kernel_exit
    ldr x30,      [sp, #16 * 15]                /**/
    ldp x28, x29, [sp, #16 * 14]                /**/
    ldp x26, x27, [sp, #16 * 13]                /**/
    ldp x24, x25, [sp, #16 * 12]                /**/
    ldp x22, x23, [sp, #16 * 11]                /**/
    ldp x20, x21, [sp, #16 * 10]                /**/
    ldp x18, x19, [sp, #16 *  9]                /**/
    ldp x16, x17, [sp, #16 *  8]                /**/
    ldp x14, x15, [sp, #16 *  7]                /**/
    ldp x12, x13, [sp, #16 *  6]                /**/
    ldp x10, x11, [sp, #16 *  5]                /**/
    ldp x8,  x9,  [sp, #16 *  4]                /**/
    ldp x6,  x7,  [sp, #16 *  3]                /**/
    ldp x4,  x5,  [sp, #16 *  2]                /**/
    ldp x2,  x3,  [sp, #16 *  1]                /**/
    ldp x0,  x1,  [sp, #16 *  0]                /**/
    add sp, sp, #256                            /**/
    eret                                        /**/
.endm

/* create entry macro */
.macro create_entry label
    .align 7                                    /* each entry should be aligned by 128 */
    b \label
.endm

/* vector table for EL1 */
.align 11                                       /* vector table should be aligned by 2048 */
vector_table_el1:
    /* Current EL with SP0 */
    create_entry    dummy_el1                   /* Synchronous    */
    create_entry    dummy_el1                   /* IRQ/vIRQ       */
    create_entry    dummy_el1                   /* FIQ/vFIQ       */
    create_entry    dummy_el1                   /* SError/vSError */

    /* Current EL with SPx */
    create_entry    sync_el1                    /* Synchronous    */
    create_entry    irq_el1                     /* IRQ/vIRQ       */
    create_entry    fiq_el1                     /* FIQ/vFIQ       */
    create_entry    serr_el1                    /* SError/vSError */

    /* Lower EL using AArch64 */
    create_entry    dummy_el1                   /* Synchronous    */
    create_entry    dummy_el1                   /* IRQ/vIRQ       */
    create_entry    dummy_el1                   /* FIQ/vFIQ       */
    create_entry    dummy_el1                   /* SError/vSError */

    /* Lower EL using AArch32 */
    create_entry    dummy_el1                   /* Synchronous    */
    create_entry    dummy_el1                   /* IRQ/vIRQ       */
    create_entry    dummy_el1                   /* FIQ/vFIQ       */
    create_entry    dummy_el1                   /* SError/vSError */

/* Synchronous_EL1 */
sync_el1:
    kernel_entry                                /* entry */
    mrs x0, esr_el1                             /* the reason for the exception */
    mrs x1, elr_el1                             /* the address of the instruction which caused the exception */
    bl sync_el1_handler                         /* C handler */
    kernel_exit                                 /* exit */

/* IRQ_EL1 */
irq_el1:
    kernel_entry                                /* entry */
    bl irq_el1_handler                          /* C handler */
    kernel_exit                                 /* exit */

/* FIQ_EL1 */
fiq_el1:
    kernel_entry                                /* entry */
    bl fiq_el1_handler                          /* C hanlder */
    kernel_exit                                 /* exit */

/* SError_EL1 */
serr_el1:
    kernel_entry                                /* entry */
    bl serr_el1_handler                         /* C handler */
    kernel_exit                                 /* exit */

/* Dummy hanlder*/
dummy_el1:
    kernel_entry                                /* entry */
    bl dummy_el1_handler                        /* C handler */
    kernel_exit                                 /* exit */
