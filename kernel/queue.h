/*------------------------------------------------------*/
/*!
 *      @file   queue.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  queue defition
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "../include/type.h"

/*------------------------------------------------------*/
/*! @brief  Queue Struct
 */
typedef struct _queue_t {
    struct _queue_t *p_prev;
    struct _queue_t *p_next;
} queue_t;

/*------------------------------------------------------*/
/*! @brief  Queue Operation
 */
void queue_init(queue_t *p_elm);
queue_t *queue_pop(queue_t *p_root);
void queue_push(queue_t *p_root, queue_t *p_elm);
void queue_remove(queue_t *p_elm);
bool queue_is_empty(queue_t *p_root);
void queue_insert_after(queue_t *p_pos, queue_t *p_elm);

#endif
