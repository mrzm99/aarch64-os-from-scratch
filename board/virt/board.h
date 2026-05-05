/*------------------------------------------------------*/
/*!
 *      @file   board.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  board config
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __BOARD_H__
#define __BOARD_H__

#include "../../include/config.h"

#define DDR_START               (0x40000000)
#define DDR_END                 (0x5FFFFFFF)
#define DDR_SIZE                (DDR_END - DDR_START)

#define UART_BASE_ADDR          (0x09000000)

#endif
