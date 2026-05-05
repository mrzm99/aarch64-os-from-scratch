/*------------------------------------------------------*/
/*!
 *      @file   config.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  kernel config
 *      @note
 */
/*------------------------------------------------------*/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

#define PAGE_SIZE                   (4096)                              //!< Page Size
#define KERNEL_OFFSET               (0xFFFF000000000000ULL)             //!< Kernel Memory Space
#define v2p(va)                     ((uint64_t)(va) - KERNEL_OFFSET)    //!< convert virtual to physical memory macro
#define p2v(pa)                     ((uint64_t)(pa) + KERNEL_OFFSET)    //!< convert physical to virtual memory macro

#endif
