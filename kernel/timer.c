/*------------------------------------------------------*/
/*!
 *      @file   timer.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#include <stdint.h>

#include "../include/util.h"

/*------------------------------------------------------*/
/*! @brief  Timer Control Block
 */
typedef struct {
    volatile uint64_t system_ticks;             //!< system tick
} knl_time_ctrl_blk_t;

static knl_time_ctrl_blk_t knl_time_ctrl_blk;
#define get_myself()        (&knl_time_ctrl_blk)

/*------------------------------------------------------*/
/*! @brief  Tick for Scheduler
 */
void scheduler_tick(void)
{
    knl_time_ctrl_blk_t *this = get_myself();

    // count up kernel tick
    this->system_ticks++;

    // Debug Log
    if (this->system_ticks % 1000 == 0) {
        printk("System Time: %d seconds.\n", this->system_ticks / 1000);
    }
}
