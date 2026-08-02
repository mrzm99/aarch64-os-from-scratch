/*------------------------------------------------------*/
/*!
 *      @file   lib.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  library
 *      @note
 */
/*------------------------------------------------------*/

#include "include/lib.h"
#include <stdint.h>

void memset(void *p_start, uint8_t val, uint64_t size)
{
    uint8_t *s = (uint8_t*)p_start;

    while (size > 0) {
        *s = val;
        s++;
        size--;
    }
}
