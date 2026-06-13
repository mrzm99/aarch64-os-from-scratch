/*------------------------------------------------------*/
/*!
 *      @file   irq.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  IRQ common header
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __IRQ_H__
#define __IRQ_H__

#include <stdint.h>

/*------------------------------------------------------*/
/*! @brief  IRQ type
 */
#define IRQ_TYPE_NONE                               (0x00000000)
#define IRQ_TYPE_EDGE                               (0x00000001)
#define IRQ_TYPE_LEVEL                              (0x00000004)

/*------------------------------------------------------*/
/*! @brief  IRQ Handler
 */
typedef void (*irq_handler_t)(void *dev_id);

/*------------------------------------------------------*/
/*! @brief  irq_chip structure
 */
struct irq_chip {
    const char *name;                                       //!< Name
    void (*irq_unmask)(uint32_t irq_no);                    //!< Interrupt Enable
    void (*irq_mask)(uint32_t irq_no);                      //!< Interrupt Disable
    void (*irq_eoi)(uint32_t irq_no);                       //!< End Of Interurupt
    int (*irq_set_type)(uint32_t irq_no, uint32_t type);    //!< Set Interrupt Type
};

/*------------------------------------------------------*/
/*! @brief  IRQ Discriptor
 */
struct irq_desc {
    struct irq_chip *chip;                                  //!< Hardware Driver
    irq_handler_t handler;                                  //!< IRQ Handler
    void *dev_id;                                           //!< IRQ Handler Argument
};

/*------------------------------------------------------*/
/*! @brief  IRQ API
 */
int request_irq(uint32_t irq_no, irq_handler_t handler, uint32_t flags, void *dev_id);
void enable_irq(uint32_t irq_no);
void disable_irq(uint32_t irq_no);
void irq_set_chip(uint32_t irq_no_start, uint32_t irq_no_end, struct irq_chip *chip);

#endif
