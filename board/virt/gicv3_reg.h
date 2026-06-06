/*------------------------------------------------------*/
/*!
 *      @file   gicv3_reg.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief
 *      @note
 */
/*------------------------------------------------------*/

#ifndef __GICV3_REG_H__
#define __GICV3_REG_H__

/*------------------------------------------------------*/
/*! @brief  base address
 */
#define GICD_BASE_PHYS                              (0x08000000U)
#define GICR_BASE_PHYS                              (0x080A0000U)

#define GICD_SIZE                                   (0x10000)
#define GICR_SIZE_PER_CORE                          (0x20000)

/*------------------------------------------------------*/
/*! @brief  Distributor offset
 */
#define GICD_CTLR                                   (0x0000)
#define GICD_TYPER                                  (0x0004)
#define GICD_IIDR                                   (0x0008)

#define GICD_IGROUPR                                (0x0080)
#define GICD_ISENABLER                              (0x0100)
#define GICD_ICENABLER                              (0x0180)
#define GICD_ISPENDR                                (0x0200)
#define GICD_ICPENDR                                (0x0280)
#define GICD_ISACTIVER                              (0x0300)
#define GICD_ICACTIVER                              (0x0380)
#define GICD_IPRIORITYR                             (0x0400)
#define GICD_ICFGR                                  (0x0C00)
#define GICD_IGRPMODR                               (0x0D00)
#define GICD_IROUTER                                (0x6000)

/*------------------------------------------------------*/
/*! @brief  Redistributor offset
 */
#define GICR_CTLR                                   (0x0000)
#define GICR_IIDR                                   (0x0004)
#define GICR_TYPER                                  (0x0008)
#define GICR_WAKER                                  (0x0014)

#define GICR_SGI_OFFSET                             (0x10000)

#define GICR_IGROUPR0                               (GICR_SGI_OFFSET + 0x0080)
#define GICR_ISENABLER0                             (GICR_SGI_OFFSET + 0x0100)
#define GICR_ICENABLER0                             (GICR_SGI_OFFSET + 0x0180)
#define GICR_ISPENDR0                               (GICR_SGI_OFFSET + 0x0200)
#define GICR_ICPENDR0                               (GICR_SGI_OFFSET + 0x0280)
#define GICR_ISACTIVER0                             (GICR_SGI_OFFSET + 0x0300)
#define GICR_ICACTIVER0                             (GICR_SGI_OFFSET + 0x0380)
#define GICR_IPRIORITYR0                            (GICR_SGI_OFFSET + 0x0400)
#define GICR_ICFGR0                                 (GICR_SGI_OFFSET + 0x0C00)
#define GICR_ICFGR1                                 (GICR_SGI_OFFSET + 0x0C04)
#define GICR_IGRPMODR0                              (GICR_SGI_OFFSET + 0x0D00)

/*------------------------------------------------------*/
/*! @brief  CPU Interface system register
 */
#define ICC_SRE_EL1                                 (S3_0_C12_C15_5)
#define ICC_PMR_EL1                                 (S3_0_C4_C6_0)
#define ICC_IRGPEN1_EL1                             (S3_0_C12_C12_7)
#define ICC_IAR1_EL1                                (S3_0_C12_C12_0)
#define ICC_EOIR_EL1                                (S3_0_C12_C12_1)

/*------------------------------------------------------*/
/*! @brief  SPI
 */
#define SPI_NO_MIN                                  (32)
#define SPI_NO_MAX                                  (1020)

#endif
