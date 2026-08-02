/*------------------------------------------------------*/
/*!
 *      @file   queue.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  queue operation
 *      @note
 */
/*------------------------------------------------------*/

#include "queue.h"

/*------------------------------------------------------*/
/*! @brief  Init queue root
 */
void queue_init(queue_t *p_root)
{
    if (p_root == NULL) {
        return;
    }

    p_root->p_prev = p_root;
    p_root->p_next = p_root;
}

/*------------------------------------------------------*/
/*! @brief  Get first element
 */
queue_t *queue_pop(queue_t *p_root)
{
    queue_t *p_elm;

    if (p_root == NULL) {
        return NULL;
    }

    if (queue_is_empty(p_root)) {
        return NULL;
    }

    p_elm = p_root->p_next;

    queue_remove(p_elm);

    return p_elm;
}

/*------------------------------------------------------*/
/*! @brief  Insert elemet
 */
void queue_push(queue_t *p_root, queue_t *p_elm)
{
    if ((p_root == NULL) || (p_elm == NULL)) {
        return;
    }

    queue_insert_after(p_root->p_prev, p_elm);
}

/*------------------------------------------------------*/
/*! @brief  Remove element
 */
void queue_remove(queue_t *p_elm)
{
    if (p_elm == NULL) {
        return;
    }

    p_elm->p_prev->p_next = p_elm->p_next;
    p_elm->p_next->p_prev = p_elm->p_prev;

    p_elm->p_prev = NULL;
    p_elm->p_next = NULL;
}

/*------------------------------------------------------*/
/*! @brief  Empty check
 */
bool queue_is_empty(queue_t *p_root)
{
    bool ret;

    if (p_root == NULL) {
        return false;
    }

    if (p_root->p_next == p_root) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

/*------------------------------------------------------*/
/*! @brief  Insert elment
 */
void queue_insert_after(queue_t *p_pos, queue_t *p_elm)
{
    if ((p_pos == NULL) || (p_elm == NULL)) {
        return;
    }

    p_elm->p_prev = p_pos;
    p_elm->p_next = p_pos->p_next;

    p_pos->p_next = p_elm;

    p_elm->p_next->p_prev = p_elm;
}
