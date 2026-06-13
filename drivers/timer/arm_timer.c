/*------------------------------------------------------*/
/*!
 *      @file   arm_timer.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>

#include "../../kernel/include/irq.h"
#include "../../arch/aarch64/sysregs.h"
#include "../../kernel/include/kernel.h"

/*------------------------------------------------------*/
/*! @brief  Interrupt No
 */
#define INT_NO_NS_PHY_TIMER             (30)

/*------------------------------------------------------*/
/*! @brief  Timer Control Block
 */
typedef struct {
    uint64_t ticks_per_1ms;             //!< count value for 1ms
} timer_ctrl_blk_t;

static timer_ctrl_blk_t timer_ctrl_blk;
#define get_myself()    (&timer_ctrl_blk)

/*------------------------------------------------------*/
/*! @brief  Timer Hanlder
 */
static void timer_handler(void *dev_id)
{
    timer_ctrl_blk_t *this = get_myself();

    // assert interrupt and set next time
    write_sysreg(this->ticks_per_1ms, CNTP_TVAL_EL0);

    // Notify kernel
    scheduler_tick();
}

/*------------------------------------------------------*/
/*! @brief  Timer Init
 */
void arm_timer_init(void)
{
    timer_ctrl_blk_t *this = get_myself();

    // save timer frecency
    uint64_t freq = read_sysreg(CNTFRQ_EL0);
    this->ticks_per_1ms = freq / 1000;

    // register handler
    request_irq(INT_NO_NS_PHY_TIMER, timer_handler, IRQ_TYPE_LEVEL, NULL);

    // init timer value
    write_sysreg(this->ticks_per_1ms, CNTP_TVAL_EL0);

    // enable timer
    write_sysreg(1, CNTP_CTL_EL0);
}
