/*------------------------------------------------------*/
/*!
 *      @file   spinlock.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  simple spin lock for single core
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include "../../arch/aarch64/cpu.h"

/*------------------------------------------------------*/
/*! @brief  Spin lock control block
 */
typedef struct {
    volatile int lock;
} spinlock_t;

/*------------------------------------------------------*/
/*! @brief  Lock
 */
static inline void spin_lock(spinlock_t *p_lock)
{
    // disable context switch
    disable_exception();

    // set lock variable
    p_lock->lock = 1;
}

/*------------------------------------------------------*/
/*! @brief  Unlock
 */
static inline void spin_unlock(spinlock_t *p_lock)
{
    // unset lock variable
    p_lock->lock = 0;

    // enable context switch
    enable_exception();
}

#endif
