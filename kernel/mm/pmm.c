/*------------------------------------------------------*/
/*!
 *      @file   pmm.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  page allocator
 *      @note
 */
/*------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include "../../board/virt/board.h"
#include "../../include/util.h"
#include "../../include/config.h"

/*------------------------------------------------------*/
/*! @brief  external symbol
 */
extern char __kernel_end[];

/*------------------------------------------------------*/
/*! @brief  header struct
 */
struct page_header_t {
    struct page_header_t *p_next;
};

/*------------------------------------------------------*/
/*! @brief  controll block
 */
struct pmm_ctrl_blk_t {
    struct page_header_t free_list_root;
};

static struct pmm_ctrl_blk_t pmm_ctrl_blk;

#define get_myself()            (&pmm_ctrl_blk)

/*------------------------------------------------------*/
/*! @brief  init controll block
 */
void pmm_init()
{
    struct pmm_ctrl_blk_t *this = get_myself();

    // align free start address just in case
    uint64_t free_start = mem_align((unsigned long)__kernel_end, PAGE_SIZE);
    printk("free_start addr = 0x%x\n", (unsigned long)free_start);

    // init root
    this->free_list_root.p_next = NULL;

    for (uint64_t addr = free_start; addr < p2v(DDR_END); addr += PAGE_SIZE) {
        // make header
        struct page_header_t *p_header = (struct page_header_t*)addr;

        // register mem block into free list
        p_header->p_next = this->free_list_root.p_next;
        this->free_list_root.p_next = p_header;
    }
}

/*------------------------------------------------------*/
/*! @brief  allocate page
 */
void *pmm_get_free_page()
{
    struct pmm_ctrl_blk_t *this = get_myself();

    if (this->free_list_root.p_next == NULL) {
        return NULL;
    }

    struct page_header_t *p_elm = this->free_list_root.p_next;

    this->free_list_root.p_next = p_elm->p_next;

    return (void*)p_elm;
}

/*------------------------------------------------------*/
/*! @brief  free page
 */
void pmm_free_page(void *p_mem)
{
    struct pmm_ctrl_blk_t *this = get_myself();
    struct page_header_t *p_page = (struct page_header_t*)p_mem;

    if (p_page == NULL) {
        return;
    }

    p_page->p_next = this->free_list_root.p_next;
    this->free_list_root.p_next = p_page;
}
