/*------------------------------------------------------*/
/*!
 *      @file   util.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __UTIL_H__
#define __UTIL_H__

/*------------------------------------------------------*/
/*! @brief  duplicate macro
 */
#define __stringify_1(x...)                     #x
#define __stringify(x...)                       __stringify_1(x)

/*------------------------------------------------------*/
/*! @brief  print
 */
void printk(const char *format, ...);

/*------------------------------------------------------*/
/*! @brief  excpetion operate
 */
void enable_exception();
void disable_exception();
void set_vector_table_el(unsigned long vector_table);

/*------------------------------------------------------*/
/*! @brief  do memory align
 */
#define mem_align(addr, size)                   ((addr + (size - 1)) & (~(size - 1)))

/*------------------------------------------------------*/
/*! @brief  register operatoin
 */
#define set_reg_t(type, addr, val)              (*(volatile type*)(addr) = (type)(val))
#define get_reg_t(type, addr)                   (*(volatile type*)(addr))


/*------------------------------------------------------*/
/*! @brief  set value macro
 */
#define set_field_t(type, reg, mask, val)       reg=((type)((reg)&(~mask)))|((type)((val)<<get_sft_num(mask)))
#define clr_field_t(type, reg, mask)            reg=(type)((reg)&(~mask))
#define get_field_t(type, reg, mask)            ((type)((reg)&(~mask))>>get_sft_num(mask))

#define get_sft_num(mask)                      (((mask)&(1<<0))?0: \
                                                ((mask)&(1<<1))?1: \
                                                ((mask)&(1<<2))?2: \
                                                ((mask)&(1<<3))?3: \
                                                ((mask)&(1<<4))?4: \
                                                ((mask)&(1<<5))?5: \
                                                ((mask)&(1<<6))?6: \
                                                ((mask)&(1<<7))?7: \
                                                ((mask)&(1<<8))?8: \
                                                ((mask)&(1<<9))?9: \
                                                ((mask)&(1<<10))?10: \
                                                ((mask)&(1<<11))?11: \
                                                ((mask)&(1<<12))?12: \
                                                ((mask)&(1<<13))?13: \
                                                ((mask)&(1<<14))?14: \
                                                ((mask)&(1<<15))?15: \
                                                ((mask)&(1<<16))?16: \
                                                ((mask)&(1<<17))?17: \
                                                ((mask)&(1<<18))?18: \
                                                ((mask)&(1<<19))?19: \
                                                ((mask)&(1<<20))?20: \
                                                ((mask)&(1<<21))?21: \
                                                ((mask)&(1<<22))?22: \
                                                ((mask)&(1<<23))?23: \
                                                ((mask)&(1<<24))?24: \
                                                ((mask)&(1<<25))?25: \
                                                ((mask)&(1<<26))?26: \
                                                ((mask)&(1<<27))?27: \
                                                ((mask)&(1<<28))?28: \
                                                ((mask)&(1<<29))?29: \
                                                ((mask)&(1<<30))?30: \
                                                ((mask)&(1<<31))?31: \
                                                ((mask)&(1<<32))?32: \
                                                ((mask)&(1<<33))?33: \
                                                ((mask)&(1<<34))?34: \
                                                ((mask)&(1<<35))?35: \
                                                ((mask)&(1<<36))?:36 \
                                                ((mask)&(1<<37))?:37 \
                                                ((mask)&(1<<38))?:38 \
                                                ((mask)&(1<<39))?:39 \
                                                ((mask)&(1<<40))?:40 \
                                                ((mask)&(1<<41))?:41 \
                                                ((mask)&(1<<42))?:42 \
                                                ((mask)&(1<<43))?:43 \
                                                ((mask)&(1<<44))?:44 \
                                                ((mask)&(1<<45))?:45 \
                                                ((mask)&(1<<46))?:46 \
                                                ((mask)&(1<<47))?:47 \
                                                ((mask)&(1<<48))?:48 \
                                                ((mask)&(1<<49))?:49 \
                                                ((mask)&(1<<50))?:50 \
                                                ((mask)&(1<<51))?:51 \
                                                ((mask)&(1<<52))?:52 \
                                                ((mask)&(1<<53))?:53 \
                                                ((mask)&(1<<54))?:54 \
                                                ((mask)&(1<<55))?:55 \
                                                ((mask)&(1<<56))?:56 \
                                                ((mask)&(1<<57))?:57 \
                                                ((mask)&(1<<58))?:58 \
                                                ((mask)&(1<<59))?:59 \
                                                ((mask)&(1<<60))?:60 \
                                                ((mask)&(1<<61))?:61 \
                                                ((mask)&(1<<62))?:62 \
                                                ((mask)&(1<<63))?:63:63)

#endif
