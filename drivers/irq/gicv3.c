/*------------------------------------------------------*/
/*!
 *      @file   gicv3.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  GICv3 driver
 *      @note
 */
/*------------------------------------------------------*/

#include <stdint.h>

#include "gicv3.h"
#include "../../board/virt/board.h"
#include "../../board/virt/gicv3_reg.h"
#include "../../arch/aarch64/sysregs.h"
#include "../../arch/aarch64/cpu.h"
#include "../../kernel/mm/vmm.h"
#include "../../kernel/include/lib.h"
#include "../../include/type.h"
#include "../../kernel/include/irq.h"

/*------------------------------------------------------*/
/*! @brief  GICv3 control block
 */
typedef struct {
    volatile uint8_t *p_gicd_base;                      //!< GICD base address
    volatile uint8_t *p_gicr_base;                      //!< GICR base address
} gicv3_ctrl_blk_t;

static gicv3_ctrl_blk_t gicv3_ctrl_blk;
#define get_myself()            (&gicv3_ctrl_blk)

/*------------------------------------------------------*/
/*! @brief  Prototype
 */
static void gicv3_irq_unmask(uint32_t irq_no);
static void gicv3_irq_mask(uint32_t irq_no);
static int gicv3_irq_set_type(uint32_t irq_no, uint32_t type);
static void gicv3_irq_eoi(uint32_t irq_no);

/*------------------------------------------------------*/
/*! @brief  Chip Structure for GICv3
 */
static struct irq_chip gicv3_chip = {
    .name = "GICv3 Chip",
    .irq_unmask = gicv3_irq_unmask,
    .irq_mask = gicv3_irq_mask,
    .irq_eoi = gicv3_irq_eoi,
    .irq_set_type = gicv3_irq_set_type,
};

/*------------------------------------------------------*/
/*! @brief  get GICR base address per core
 */
static volatile uint8_t *get_my_gicr_base(void)
{
    gicv3_ctrl_blk_t *this = get_myself();

    // get MPIDR_EL1 register value
    uint64_t mpidr = read_sysreg(MPIDR_EL1);

    // get Affinity0 (core ID)
    uint32_t core_id = mpidr & 0xFF;

    return this->p_gicr_base + (core_id * GICR_SIZE_PER_CORE);
}

/*------------------------------------------------------*/
/*! @brief  Enable Interrupt
 */
static void gicv3_irq_unmask(uint32_t irq_no)
{
    gicv3_ctrl_blk_t *this = get_myself();
    volatile uint8_t *my_gicr_base = get_my_gicr_base();

    if (irq_no > SPI_NO_MAX) {
        return;
    }

    uint32_t bit_pos = 1 << (irq_no % 32);

    if (irq_no <= PPI_NO_MAX) {
        set_reg_t(uint32_t, my_gicr_base + GICR_ISENABLER0, bit_pos);
    } else {
        uint32_t offset = irq_no / 32;
        set_reg_t(uint32_t, this->p_gicd_base + GICD_ISENABLER + (offset * 4), bit_pos);
    }
}

/*------------------------------------------------------*/
/*! @brief  Disable Interrupt
 */
static void gicv3_irq_mask(uint32_t irq_no)
{
    gicv3_ctrl_blk_t *this = get_myself();
    volatile uint8_t *my_gicr_base = get_my_gicr_base();

    if (irq_no > SPI_NO_MAX) {
        return;
    }

    uint32_t bit_pos = 1 << (irq_no % 32);

    if (irq_no <= PPI_NO_MAX) {
        set_reg_t(uint32_t, my_gicr_base + GICR_ICENABLER0, bit_pos);
    } else {
        uint32_t offset = irq_no / 32;
        set_reg_t(uint32_t, this->p_gicd_base + GICD_ICENABLER + (offset * 4), bit_pos);
    }
}

/*------------------------------------------------------*/
/*! @brief  Set Interrupt Trigger Type
 */
static int gicv3_irq_set_type(uint32_t irq_no, uint32_t type)
{
    gicv3_ctrl_blk_t *this = get_myself();

    if (irq_no > SPI_NO_MAX) {
        return -1;
    }

    // PPI
    if ((irq_no >= PPI_NO_MIN) && (irq_no <= PPI_NO_MAX)) {
        volatile uint8_t *my_gicr_base = get_my_gicr_base();
        uint32_t shift_num = ((irq_no - 16) % 16) * 2;
        uint32_t val = get_reg_t(uint32_t, my_gicr_base + GICR_ICFGR1);

        val &= ~(3 << shift_num);
        val |= (type == IRQ_TYPE_EDGE ? IRQ_TYPE_EDGE : IRQ_TYPE_LEVEL) << shift_num;
        set_reg_t(uint32_t, my_gicr_base + GICR_ICFGR1, val);

    // SPI
    } else {
        uint32_t offset = (irq_no / 16) * 4;
        uint32_t shift_num = (irq_no % 16) * 2;
        uint32_t val = get_reg_t(uint32_t, this->p_gicd_base + GICD_ICFGR + offset);

        val &= ~(3 << shift_num);
        val |= (type == IRQ_TYPE_EDGE ? IRQ_TYPE_EDGE : IRQ_TYPE_LEVEL) << shift_num;
        set_reg_t(uint32_t, this->p_gicd_base + GICD_ICFGR + offset, val);
    }

    return 0;
}

/*------------------------------------------------------*/
/*! @brief  Notify End of Interrupt
 */
static void gicv3_irq_eoi(uint32_t irq_no)
{
    if (irq_no > SPI_NO_MAX) {
        return;
    }

    write_sysreg(irq_no, ICC_EOIR_EL1);
}

/*------------------------------------------------------*/
/*! @brief  Acknowledge Interrupt
 */
uint32_t gicv3_acknowledge_irq(void)
{
    return read_sysreg(ICC_IAR1_EL1);
}

/*------------------------------------------------------*/
/*! @brief  GICv3 Global Initializeation
 */
void gicv3_init_global(void)
{
    gicv3_ctrl_blk_t *this = get_myself();
    uint32_t val = 0;

    // clear control block
    memset(this, 0, sizeof(gicv3_ctrl_blk_t));

    // set virtual base address
    this->p_gicd_base = (volatile uint8_t*)ioremap(GICD_BASE_PHYS, GICD_SIZE);
    this->p_gicr_base = (volatile uint8_t*)ioremap(GICR_BASE_PHYS, GICR_SIZE_PER_CORE * CORE_NUM);

    // disable Distributor temporaliy
    set_reg_t(uint32_t, this->p_gicd_base + GICD_CTLR, 0);

    // cleanup all SPI(INTID 32~1019)
    for (int i = SPI_NO_MIN; i < SPI_NO_MAX; i += 32) {
        set_reg_t(uint32_t, this->p_gicd_base + GICD_ICENABLER + (i / 8), 0xFFFFFFFF);  // disable
        set_reg_t(uint32_t, this->p_gicd_base + GICD_ICPENDR   + (i / 8), 0xFFFFFFFF);  // clear pending
        set_reg_t(uint32_t, this->p_gicd_base + GICD_IGROUPR   + (i / 8), 0xFFFFFFFF);  // all SPI is Group1.NS
    }

    // set all SPI init priority
    for (int i = SPI_NO_MIN; i < SPI_NO_MAX; i += 4) {
        set_reg_t(uint32_t, this->p_gicd_base + GICD_IPRIORITYR + i, 0xA0A0A0A0);
    }

    // enable Distributor
    val |= (1 << 5);    // ARE_NS=1
    val |= (1 << 1);    // EnableGrp1NS=1
    set_reg_t(uint32_t, this->p_gicd_base + GICD_CTLR, val);
}

/*------------------------------------------------------*/
/*! @brief  GICv3 Init per core
 */
void gicv3_init_per_core(void)
{
    uint32_t val32;
    uint64_t val64;

    // get my own GICR base address
    volatile uint8_t *my_gicr_base = get_my_gicr_base();

    // wakeup Redistributor
    val32 = get_reg_t(uint32_t, my_gicr_base + GICR_WAKER);
    val32 &= ~(1 << 1);
    set_reg_t(uint32_t, my_gicr_base + GICR_WAKER, val32);

    // wait until wakeup
    while (get_reg_t(uint32_t, my_gicr_base + GICR_WAKER) & (1 << 2)) {
        asm volatile("nop");
    }

    // cleanup all SGI/PPI(INTID 0~31)
    set_reg_t(uint32_t, my_gicr_base + GICR_ICENABLER0, 0xFFFFFFFF);    // disable
    set_reg_t(uint32_t, my_gicr_base + GICR_ICPENDR0,   0xFFFFFFFF);    // clear pending
    set_reg_t(uint32_t, my_gicr_base + GICR_IGROUPR0,   0xFFFFFFFF);    // Group1.NS
    set_reg_t(uint32_t, my_gicr_base + GICR_IGRPMODR0,  0x00000000);    // Non secure

    // set all PPI/SGI init priority
    for (int i = 0; i < 32; i += 4) {
        set_reg_t(uint32_t, my_gicr_base + GICR_IPRIORITYR0 + i, 0xA0A0A0A0);
    }

    // use system register to access CPU inteface
    val64 = read_sysreg(ICC_SRE_EL1);
    val64 |= 1;
    write_sysreg(val64, ICC_SRE_EL1);

    // set min priority filter
    write_sysreg(0xFF, ICC_PMR_EL1);

    // unuse subpriority
    write_sysreg(0, ICC_BPR1_EL1);

    // EOI = priority drop and diactivate
    write_sysreg(0, ICC_CTLR_EL1);

    // enable Group1 interrupt
    write_sysreg(1, ICC_IGRPEN1_EL1);

    issue_isb();
}

/*------------------------------------------------------*/
/*! @brief  GICv3 Init
 */
void gicv3_init(void)
{
    gicv3_init_global();
    gicv3_init_per_core();

    irq_set_chip(0, SPI_NO_MAX, &gicv3_chip);
}
