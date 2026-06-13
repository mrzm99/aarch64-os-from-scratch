/*------------------------------------------------------*/
/*!
 *      @file   switch.S
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  context switch
 *      @note
 */
/*------------------------------------------------------*/

.section ".text"

/*------------------------------------------------------*/
/*! @brief  Do context switch
 *
 *  @param  x0 : prev task_struct pointer
 *  @param  x1 : next task_struct pointer
 */
.global cpu_switch_to
cpu_switch_to:
    /* save current context (prev) */
    stp x19, x20, [x0, #16 * 0]             // offset:  0, 8
    stp x21, x22, [x0, #16 * 1]             // offset: 16, 24
    stp x23, x24, [x0, #16 * 2]             // offset: 32, 40
    stp x25, x26, [x0, #16 * 3]             // offset: 48, 56
    stp x27, x28, [x0, #16 * 4]             // offset: 64, 72

    mov x9, sp
    stp x29,  x9, [x0, #16 * 5]             // offset : 80, 88

    str x30,      [x0, #16 * 6]             // offset : 96

    /* restore next context (next) */
    ldp x19, x20, [x1, #16 * 0]
    ldp x21, x22, [x1, #16 * 1]
    ldp x23, x24, [x1, #16 * 2]
    ldp x25, x26, [x1, #16 * 3]
    ldp x27, x28, [x1, #16 * 4]

    ldp x29,  x9, [x1, #16 * 5]
    mov sp, x9

    ldr x30, [x1, #16 * 6]

    ret

/*------------------------------------------------------*/
/*! @brief  wapper function of new entry point
 */
.global ret_from_fork
ret_from_fork:
    msr daifclr, #0x3
    blr x19
