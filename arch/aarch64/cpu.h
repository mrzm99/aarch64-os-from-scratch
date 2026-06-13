/*------------------------------------------------------*/
/*!
 *      @file   cpu.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  cpu operation
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __CPU_H__
#define __CPU_H__

static inline void issue_isb(void)
{
    asm volatile("isb");
}

inline void enable_exception(void)
{
    asm volatile("msr daifclr, #0x3");
}

inline void disable_exception(void)
{
    asm volatile("msr daifset, #0xF");
}

#endif
