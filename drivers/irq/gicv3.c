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

/*------------------------------------------------------*/
/*! @brief  GICv3 control block
 */
typedef struct {
    irq_handler_t irq_handler_list[INT_ID_MAX];         //!< IRQ Handler List
    void *dev_id_list[INT_ID_MAX];                      //!< dev_id_list

    volatile uint8_t *p_gicd_base;                      //!< GICD base address
    volatile uint8_t *p_gicr_base;                      //!< GICR base address
} gicv3_ctrl_blk_t;

static gicv3_ctrl_blk_t gicv3_ctrl_blk;
#define get_myself()            (&gicv3_ctrl_blk)

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

    return this->p_gicr_base + (core_id * CORE_NUM);
}

/*------------------------------------------------------*/
/*! @brief  GICv3 Global Initializeation
 */
void gicv3_init_global(void)
{
    gicv3_ctrl_blk_t *this = get_myself();
    uint32_t val = 0;

    // set virtual base address
    this->p_gicd_base = (volatile uint8_t*)ioremap(GICD_BASE_PHYS, GICD_SIZE);
    this->p_gicr_base = (volatile uint8_t*)ioremap(GICR_BASE_PHYS, GICR_SIZE_PER_CORE * CORE_NUM);

    // disable Distributor temporaliy
    set_reg_t(uint32_t, GICD_CTLR, 0);

    // cleanup all SPI(INTID 32~1019)
    for (int i = SPI_NO_MIN; i < SPI_NO_MAX; i += 32) {
        set_reg_t(uint32_t, GICD_ICENABLER + (i / 8), 0xFFFFFFFF);  // disable
        set_reg_t(uint32_t, GICD_ICPENDR   + (i / 8), 0xFFFFFFFF);  // clear pending
        set_reg_t(uint32_t, GICD_IGROUPR   + (i / 8), 0xFFFFFFFF);  // all SPI is Group1.NS
    }

    // set all SPI init priority
    for (int i = SPI_NO_MIN; i < SPI_NO_MAX; i += 4) {
        set_reg_t(uint32_t, GICD_IPRIORITYR + i, 0xA0A0A0A0);
    }

    // enable Distributor
    val |= (1 << 5);    // ARE_NS=1
    val |= (1 << 1);    // EnableGrp1NS=1
    set_reg_t(uint32_t, GICD_CTLR, val);
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

    // wait untile wakeup
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
