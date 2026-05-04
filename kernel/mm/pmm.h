/*------------------------------------------------------*/
/*!
 *      @file   pmm.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  page allocator
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __PMM_H__
#define __PMM_H__

void pmm_init();
void *pmm_get_free_page();
void pmm_free_page(void *p_mem);

#endif
