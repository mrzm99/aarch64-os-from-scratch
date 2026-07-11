/*------------------------------------------------------*/
/*!
 *      @file   free_list.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#include "kmalloc.h"
#include "../../include/lib.h"
#include "../pmm.h"
#include "../../../include/config.h"
#include "../../../include/type.h"
#include "../../../include/util.h"
#include "../../include/kernel.h"
#include <stdint.h>

/*------------------------------------------------------*/
/*! @brief  Macro
 */
#define HEADER_SIZE                 (sizeof(block_header_t))
#define BLOCK_ALIGN_SIZE            (16)

#define can_coalesce(p_prev, p_next)      (((uint64_t)(p_prev) + (p_prev->size)) == ((uint64_t)(p_next)))

/*------------------------------------------------------*/
/*! @brief  Block Header Struct
 */
typedef struct _block_header_t {
    struct _block_header_t *p_next;
    uint64_t size;
} block_header_t;

/*------------------------------------------------------*/
/*! @brief  Control Block
 */
typedef struct {
    block_header_t root;
} free_list_ctl_blk_t;

static free_list_ctl_blk_t free_list_ctl_blk;
#define get_myself()            (&free_list_ctl_blk)

/*------------------------------------------------------*/
/*! @brief  combine block
 */
static void combine_block(block_header_t *p_prev, block_header_t *p_next)
{
    // check
    if ((p_prev == NULL) || (p_next == NULL)) {
        return;
    }

    p_prev->size += p_next->size;
    p_prev->p_next = p_next->p_next;

    p_next->size = 0;
    p_next->p_next = NULL;
}

/*------------------------------------------------------*/
/*! @brief  Init
 */
void kmalloc_init()
{
    free_list_ctl_blk_t *this = get_myself();
    block_header_t *p_top;

    // init control block
    memset(this, 0, sizeof(free_list_ctl_blk_t));

    // get free page
    p_top = (block_header_t*)pmm_get_free_page();

    printk("kmalloc_malloc page addr = %x\n", (uint64_t)p_top);

    // make tag
    p_top->p_next = NULL;
    p_top->size = PAGE_SIZE;

    // create free list
    this->root.p_next = p_top;
    this->root.size = 0;
}

/*------------------------------------------------------*/
/*! @brief  Malloc
 */
void *kmalloc_malloc(uint64_t size)
{
    free_list_ctl_blk_t*this = get_myself();
    block_header_t *p_block;
    block_header_t *p_prev;
    void *p_mem = NULL;

    // check
    if (size == 0) {
        return NULL;
    }

    // calc block size
    size += HEADER_SIZE;

    // align
    size = mem_align(size, BLOCK_ALIGN_SIZE);

    // search free list
    p_block = this->root.p_next;
    p_prev = &this->root;

    while (p_block != NULL) {

        // check size
        if (p_block->size >= size) {
            uint64_t remain_size = p_block->size - size;

            if (remain_size < HEADER_SIZE + 16) {
                p_prev->p_next = p_block->p_next;
                p_mem = (uint8_t*)p_block + HEADER_SIZE;
            } else {
                p_block->size = remain_size;
                block_header_t *p_new_block = (block_header_t*)((uint8_t*)p_block + remain_size);
                p_new_block->size = size;
                p_new_block->p_next = NULL;
                p_mem = (uint8_t*)p_new_block + HEADER_SIZE;
            }
            break;
        }

        // get next block
        p_prev = p_block;
        p_block = p_block->p_next;
    }

    return p_mem;
}

/*------------------------------------------------------*/
/*! @brief  Free
 */
void kmalloc_free(void *p_addr)
{
    free_list_ctl_blk_t *this = get_myself();
    block_header_t *p_block;

    // check addr
    if (p_addr == NULL) {
        return;
    }

    // get block
    p_block = (block_header_t*)((uint8_t*)p_addr - HEADER_SIZE);

    // 探索をrootノードから開始する
    block_header_t *p_curr = &this->root;

    // 
    while ((p_curr->p_next != NULL) && (p_curr->p_next < p_block)) {
        p_curr = p_curr->p_next;
    }

    p_block->p_next = p_curr->p_next;
    p_curr->p_next = p_block;

    // 
    if ((p_block->p_next != NULL) && (can_coalesce(p_block, p_block->p_next))) {
        combine_block(p_block, p_block->p_next);
    }

    // rootの場合は結合してはいけない
    if ((p_curr != &this->root) && (can_coalesce(p_curr, p_block))) {
        combine_block(p_curr, p_block);
    }
}
