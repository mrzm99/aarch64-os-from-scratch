/*------------------------------------------------------*/
/*!
 *      @file   kmalloc.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __KMALLOC_H__
#define __KMALLOC_H__

#include <stdint.h>

/*------------------------------------------------------*/
/*! @brief  prototype
 */
void kmalloc_init(void);
void *kmalloc_malloc(uint64_t size);
void kmalloc_free(void *p_addr);

#endif
