/*------------------------------------------------------*/
/*!
 *      @file   irq.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  IRQ API
 *      @note
 */
/*------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>

#include "include/irq.h"
#include "../include/util.h"
#include "include/lib.h"

/*------------------------------------------------------*/
/*! @brief  Macro
 */
#define IRQ_MAX                         (1024)

/*------------------------------------------------------*/
/*! @brief  IRQ Control Block
 */
typedef struct {
    struct irq_desc irq_desc_list[IRQ_MAX];
} irq_ctrl_blk_t;

static irq_ctrl_blk_t irq_ctrl_blk;
#define get_myself()        (&irq_ctrl_blk)

/*------------------------------------------------------*/
/*! @brief  IRQ Init
 */
void irq_init(void)
{
    irq_ctrl_blk_t *this = get_myself();

    memset(this, 0, sizeof(irq_ctrl_blk_t));
}

/*------------------------------------------------------*/
/*! @brief  Register IRQ Handler
 */
int request_irq(uint32_t irq_no, irq_handler_t handler, uint32_t flags, void *dev_id)
{
    irq_ctrl_blk_t *this = get_myself();

    // check arguments
    if ((irq_no >= IRQ_MAX) || (handler == NULL)) {
        return -1;
    }

    // save handler and dev_id
    this->irq_desc_list[irq_no].handler = handler;
    this->irq_desc_list[irq_no].dev_id = dev_id;

    // enable interrupt
    if ((this->irq_desc_list[irq_no].chip != NULL) && (this->irq_desc_list[irq_no].chip->irq_unmask != NULL)) {
        this->irq_desc_list[irq_no].chip->irq_unmask(irq_no);
    }

    return 0;
}

/*------------------------------------------------------*/
/*! @brief  Do IRQ Handler
 */
void do_irq(uint32_t irq_no)
{
    irq_ctrl_blk_t *this = get_myself();

    // スブリアス割り込み対策
    if (irq_no >= 1023 || irq_no >= IRQ_MAX) {
        return;
    }

    // get the irq descriptor
    struct irq_desc *desc = &this->irq_desc_list[irq_no];

    // do handler
    if (desc->handler != NULL) {
        desc->handler(desc->dev_id);
    } else {
        printk("Unexpected IRQ trap at vector %02x\n", irq_no);
    }

    // notify end of interrupt
    if ((desc->chip != NULL) && (desc->chip->irq_eoi != NULL)) {
        desc->chip->irq_eoi(irq_no);
    }
}

/*------------------------------------------------------*/
/*! @brief  Set Chip info
 */
void irq_set_chip(uint32_t irq_no_start, uint32_t irq_no_end, struct irq_chip *chip)
{
    irq_ctrl_blk_t *this = get_myself();

    if (irq_no_end >= IRQ_MAX) {
        return;
    }

    for (int idx = irq_no_start; idx <= irq_no_end; idx++) {
        this->irq_desc_list[idx].chip = chip;
    }
}

/*------------------------------------------------------*/
/*! @brief  Enable Interrupt
 */
void enable_irq(uint32_t irq_no)
{
    irq_ctrl_blk_t *this = get_myself();

    if (this->irq_desc_list[irq_no].chip->irq_unmask != NULL) {
        this->irq_desc_list[irq_no].chip->irq_unmask(irq_no);
    }
}

/*------------------------------------------------------*/
/*! @brief  Do IRQ Handler
 */
void disable_irq(uint32_t irq_no)
{
    irq_ctrl_blk_t *this = get_myself();

    if (this->irq_desc_list[irq_no].chip->irq_mask != NULL) {
        this->irq_desc_list[irq_no].chip->irq_mask(irq_no);
    }
}
