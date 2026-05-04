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
/*! @brief  print
 */
void printk(const char *format, ...);

/*------------------------------------------------------*/
/*! @brief  excpetion operate
 */
void enable_exception();
void disable_exception();
void set_vector_table_el(unsigned long vector_table);

#endif
