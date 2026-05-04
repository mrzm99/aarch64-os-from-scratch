/*------------------------------------------------------*/
/*!
 *      @file   vmm.h
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  virtual memory management
 *      @note
 */
/*------------------------------------------------------*/
#ifndef __VMM_H__
#define __VMM_H__

#include <stdint.h>

/*------------------------------------------------------*/
/*! @brief  page table type
 */
typedef uint64_t pgd_t;                     //!< Page Global Directory (L0)
typedef uint64_t pud_t;                     //!< Page Upper Directory  (L1)
typedef uint64_t pmd_t;                     //!< Page Middle Directory (L2)
typedef uint64_t pte_t;                     //!< Page Table Entry      (L3)

/*------------------------------------------------------*/
/*! @brief  Page Entry Flag
 */
#define PTE_VALID                       (1ULL << 0)         //!< entry[0]
#define PTE_TABLE                       (1ULL << 1)         //!< entry[1]
#define PTE_PAGE                        (1ULL << 1)         //!< entry[1] the same value with table

/*------------------------------------------------------*/
/*! @brief  Page Attribute
 *  @note   pay attention to consistency with MAIR_EL1.
 */
#define PTE_ATTR_INDEX0_DEVICE          (0ULL << 2)         //!< MAIR[0] : Device Memory
#define PTE_ATTR_INDEX1_NORMAL          (1ULL << 2)         //!< MAIR[1] : Normal Memory

#define PTE_SH_INNER                    (3ULL << 8)         //!< Inner Share
#define PTE_AF                          (1ULL << 10)        //!< Access Flag

#define PTE_PXN                         (1ULL << 53)        //!< Privileged Execute Never
#define PTE_UXN                         (1ULL << 54)        //!< Unprivileged Execute Never

/*------------------------------------------------------*/
/*! @brief  Memory Type
 */
#define PROT_NORMAL     (PTE_VALID | PTE_PAGE | PTE_ATTR_INDEX1_NORMAL | PTE_SH_INNER | PTE_AF)
//#define PROT_DEVICE     (PTE_VALID | PTE_PAGE | PTE_ATTR_INDEX0_DEVICE | PTE_SH_INNER | PTE_AF | PTE_PXN | PTE_UXN)
#define PROT_DEVICE     (PTE_VALID | PTE_PAGE | PTE_ATTR_INDEX0_DEVICE | PTE_SH_INNER | PTE_AF)

/*------------------------------------------------------*/
/*! @brief  Mask
 */
#define PAGE_SHIFT_NUM              (12)                    //!< 
#define PTRS_PER_PTE                (512)                   //!< 
#define PTE_INDEX_MASK              (PTRS_PER_PTE - 1)      //!<

#define PAGE_MASK                   (~((1ULL << PAGE_SHIFT_NUM) - 1))

/*------------------------------------------------------*/
/*! @brief  ProtType
 */
pgd_t *setup_page_tables();
void create_mapping(pgd_t *p_pgd, uint64_t va, uint64_t pa, uint64_t prot);

#endif
