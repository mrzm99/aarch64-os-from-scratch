/*------------------------------------------------------*/
/*!
 *      @file   gicv3.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __GICV3_H__
#define __GICV3_H__

/*------------------------------------------------------*/
/*! @brief  Interrupt ID
 */
#define INT_ID_NS_PHY_TIMER                     (30)
#define INT_ID_MAX                              (1024)

/*------------------------------------------------------*/
/*! @brief  GICv3初期化
 */
void gicv3_init(void);

#endif
