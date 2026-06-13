/*------------------------------------------------------*/
/*!
 *      @file   schedule.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#include "../arch/aarch64/cpu.h"
#include "include/kernel.h"
#include <stdint.h>
#include <stdio.h>

#include "../include/util.h"

extern void cpu_switch_to(struct task_struct *prev, struct task_struct *next);

/*------------------------------------------------------*/
/*! @brief  Kernel Control Block
 */
typedef struct {
    volatile uint64_t system_ticks;
} knl_ctrl_blk_t;

static knl_ctrl_blk_t knl_time_ctrl_blk;
#define get_myself()        (&knl_time_ctrl_blk)

/*------------------------------------------------------*/
/*! @brief  Task Definition
 */
#define NR_TASKS                        (64)

struct task_struct *task[NR_TASKS] = { NULL };
struct task_struct *current = NULL;

/*------------------------------------------------------*/
/*! @brief  Task Scheduler
 */
void schedule(void)
{
    uint64_t next_idx = 0;
    uint64_t max_time_slice = 0;

    // check preempt enable
    if (current->preepmt_count > 0) {
        return;
    }

    // disable exception
    disable_exception();

    while (1) {
        // get next task
        for (int i = 0; i < NR_TASKS; i++) {
            struct task_struct *p_task = task[i];

            if ((p_task != NULL) && (p_task->state == TASK_RUNNING) && (p_task->counter > max_time_slice)) {
                next_idx = i;
                max_time_slice = p_task->counter;
            }
        }

        // check next task time
        if (max_time_slice > 0) {
            break;
        }

        // if all task has no left time, reset task time
        for (int i = 0; i < NR_TASKS; i++) {
            if (task[i]) {
                task[i]->counter = (task[i]->counter >> 1) + task[i]->priority;
            }
        }
    }

    struct task_struct *next = task[next_idx];

    // context switch
    if (current != next) {
        struct task_struct *prev = current;
        current = next;

        cpu_switch_to(prev, next);
    }

    // enable exception
    enable_exception();
}

/*------------------------------------------------------*/
/*! @brief  Tick for Scheduler
 */
void scheduler_tick(void)
{
    knl_ctrl_blk_t *this = get_myself();

    // count up kernel tick
    this->system_ticks++;

    // Debug Log
    if (this->system_ticks % 1000 == 0) {
        printk("System Time: %d seconds.\n", this->system_ticks / 1000);
    }

    current->counter--;
    if (current->counter <= 0) {
        current->counter = 0;

        current->flags |= TIF_NEED_RESCHED;
    }
}

/*------------------------------------------------------*/
/*! @brief  Check Reschedule
 */
void check_resched(void)
{
    if (current->flags & TIF_NEED_RESCHED) {
        current->flags &= ~TIF_NEED_RESCHED;
        schedule();
    }
}
