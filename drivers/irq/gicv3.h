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

#include <stdint.h>

/*------------------------------------------------------*/
/*! @brief  Interrupt ID
 */
#define INT_ID_NS_PHY_TIMER                     (30)
#define INT_ID_MAX                              (1024)


/*------------------------------------------------------*/
/*! @brief  interrupt trigger
 */
#define IRQ_TRIGGER_NONE                        (0)
#define IRQ_TRIGGER_EDGE                        (1)
#define IRQ_TRIGGER_LEVEL                       (2)
#define IRQ_TRIGGER_MAX                         (3)

/*------------------------------------------------------*/
/*! @brief  割り込みハンドラ定義
 */
typedef void (*irq_handler_t)(void *dev_id);

/*------------------------------------------------------*/
/*! @brief  割り込み制御関数
 */
void gicv3_enable_irq(uint32_t int_no);
void gicv3_disable_irq(uint32_t int_no);
int gicv3_register_irq_hadler(uint32_t int_no, irq_handler_t handler, uint8_t priority, uint32_t flags, void *dev_id);
uint32_t gicv3_acknowledge_irq(void);
void gicv3_end_of_irq(uint32_t int_no);

/*------------------------------------------------------*/
/*! @brief  GICv3初期化
 */
void gicv3_init_global(void);
void gicv3_init_per_core(void);

#endif
