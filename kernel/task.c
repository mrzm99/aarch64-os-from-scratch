/*------------------------------------------------------*/
/*!
 *      @file   task.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>

#include "include/kernel.h"
#include "../include/config.h"
#include "include/lib.h"
#include "../include/util.h"
#include "mm/pmm.h"

extern void ret_from_fork(void);

/*------------------------------------------------------*/
/*! @brief  Task Control Block
 */
typedef struct {
    uint64_t task_count;
} task_ctrl_blk_t;

static task_ctrl_blk_t task_ctrl_blk;
#define get_myself()    (&task_ctrl_blk)

/*------------------------------------------------------*/
/*! @brief Task Init
 */
void task_init(void)
{
    task_ctrl_blk_t *this = get_myself();

    memset(this, 0, sizeof(task_ctrl_blk_t));
}

/*------------------------------------------------------*/
/*! @brief
 */
int create_task(void (*entry)(void))
{
    task_ctrl_blk_t *this = get_myself();

    //  get free page for new task struct
    struct task_struct *p_task_struct = (struct task_struct*)pmm_get_free_page();

    if (p_task_struct == NULL) {
        return -1;
    }

    // zero clear
    memset(p_task_struct, 0, sizeof(struct task_struct));

    // get stack pointer start
    uint64_t stack_top = (uint64_t)p_task_struct + PAGE_SIZE;

    // set stack pointer and entry point
    p_task_struct->context.sp = stack_top;
    p_task_struct->context.pc = (uint64_t)ret_from_fork;
    p_task_struct->context.x19 = (uint64_t)entry;

    // init task
    p_task_struct->state = TASK_RUNNING;
    p_task_struct->counter = 10;
    p_task_struct->priority = 10;
    p_task_struct->preepmt_count = 0;
    p_task_struct->pid = this->task_count;

    // register ready queue
    task[this->task_count] = p_task_struct;
    this->task_count++;

    return p_task_struct->pid;
}
