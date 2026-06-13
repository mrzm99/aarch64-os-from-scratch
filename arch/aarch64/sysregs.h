/*------------------------------------------------------*/
/*!
 *      @file   sysregs.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  system register operation
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __SYSREGS_H__
#define __SYSREGS_H__

#include <stdint.h>
#include "../../include/util.h"

/*------------------------------------------------------*/
/*! @brief  Generic Timer
 */
#define CNTFRQ_EL0              cntfrq_el0          //!< Timer Frequency (Hz)
#define CNTP_TVAL_EL0           cntp_tval_el0       //!< Timer Value
#define CNTP_CTL_EL0            cntp_ctl_el0        //!< Control Register

/*------------------------------------------------------*/
/*! @brief  read system register macro
 */
#define read_sysreg(reg) ({ \
            uint64_t _val; \
            asm volatile("mrs %0, " __stringify(reg) : "=r" (_val)); \
            _val; \
        })

/*------------------------------------------------------*/
/*! @brief  write system register macro
 */
#define write_sysreg(val, reg) do { \
            uint64_t _val = (uint64_t)(val); \
            asm volatile("msr " __stringify(reg) ", %0" :: "r" (_val)); \
        } while (0)

#endif
