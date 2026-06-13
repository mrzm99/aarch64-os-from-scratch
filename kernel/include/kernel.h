/*------------------------------------------------------*/
/*!
 *      @file   kernel.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdint.h>

/*------------------------------------------------------*/
/*! @brief  Task States
 */
#define TASK_RUNNING                                (0)     //!< Running or Ready
#define TASK_INTERRUPTIBLE                          (1)     //!< Sleep (I/O wait etc)
#define TASK_DEAD                                   (2)     //!< completed

/*------------------------------------------------------*/
/*! @brief  Task Flags
 */
#define TIF_NEED_RESCHED                            (1)     //!< need reschedule

/*------------------------------------------------------*/
/*! @brief  AArch64 CPU Context
 */
struct cpu_context {
    uint64_t    x19;                    //!< x19 register
    uint64_t    x20;                    //!< x20 register
    uint64_t    x21;                    //!< x21 register
    uint64_t    x22;                    //!< x22 register
    uint64_t    x23;                    //!< x23 register
    uint64_t    x24;                    //!< x24 register
    uint64_t    x25;                    //!< x25 register
    uint64_t    x26;                    //!< x26 register
    uint64_t    x27;                    //!< x27 register
    uint64_t    x28;                    //!< x28 register
    uint64_t    fp;                     //!< Frame Pointer (x29)
    uint64_t    sp;                     //!< Stack Pointer
    uint64_t    pc;                     //!< Program Counter (x30: Link Register)
};

/*------------------------------------------------------*/
/*! @brief  Process Control Block (PCB)
 */
struct task_struct {
    struct cpu_context context;         //!< Task Context
    uint64_t state;                     //!< Task State
    uint64_t counter;                   //!< Time Slice
    uint64_t priority;                  //!< Task Priority
    uint64_t preepmt_count;             //!< Preemt Disable Count
    uint64_t flags;                     //!< Task Flag
    uint64_t pid;                       //!< Process ID
};

/*------------------------------------------------------*/
/*! @brief  running Task Pointer
 */
extern struct task_struct *current;
extern struct task_struct *task[];

/*------------------------------------------------------*/
/*! @brief  Tick API for Kernel
 */
void scheduler_tick(void);

void task_init(void);
int create_task(void (*entry)(void));

#endif
