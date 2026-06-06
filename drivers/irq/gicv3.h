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

/*------------------------------------------------------*/
/*! @brief  割り込みハンドラ定義
 */
typedef void (*irq_handler_t)(void *dev_id);

/*------------------------------------------------------*/
/*! @brief  割り込み制御関数
 */
void gicv3_enable_irq(uint32_t irq);
void gicv3_disable_irq(uint32_t irq);
void gicv3_set_irq_priority(uint32_t irq, uint8_t priority);
int gicv3_register_irq(uint32_t irq, irq_handler_t handler, uint32_t flags, void *dev_id);
uint32_t gicv3_acknowledge_irq(void);
void gicv3_end_of_irq(uint32_t irq);

/*------------------------------------------------------*/
/*! @brief  GICv3初期化
 */
void gicv3_init(void);



#endif
