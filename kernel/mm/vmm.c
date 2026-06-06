/*------------------------------------------------------*/
/*!
 *      @file   vmm.c
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  virtual memory management
 *      @note
 */
/*------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "vmm.h"
#include "pmm.h"
#include "../../include/config.h"
#include "../../board/virt/board.h"
#include "../../arch/aarch64/mmu.h"

/*------------------------------------------------------*/
/*! @brief  PTE index operation macro
 */
#define get_pgd_idx(addr)           (((addr) >> 39) & (PTE_INDEX_MASK))
#define get_pud_idx(addr)           (((addr) >> 30) & (PTE_INDEX_MASK))
#define get_pmd_idx(addr)           (((addr) >> 21) & (PTE_INDEX_MASK))
#define get_pte_idx(addr)           (((addr) >> 12) & (PTE_INDEX_MASK))

/*------------------------------------------------------*/
/*! @brief  ioremap definition
 */
#define VMALLOC_START               (0xFFFF800000000000ULL)

/*------------------------------------------------------*/
/*! @brief  vmm control block
 */
struct vmm_ctrl_blk_t {
    pgd_t *p_kernel_pgd;                    //!< kernel PGD addr
    uint64_t next_vaddr;                    //!< pointer to empty vaddr
};

static struct vmm_ctrl_blk_t vmm_ctrl_blk;

#define get_myself()        (&vmm_ctrl_blk)

/*------------------------------------------------------*/
/*! @brief  zero clear
 */
static void memzero(void *p_start, uint64_t size)
{
    uint8_t *p = (uint8_t*)p_start;

    if (p == NULL) {
        return;
    }

    for (uint64_t i = 0; i < size; i++) {
        *p = 0;
        p++;
    }
}

/*------------------------------------------------------*/
/*! @brief  init
 */
void vmm_init()
{
    struct vmm_ctrl_blk_t *this = get_myself();

    memzero(this, sizeof(struct vmm_ctrl_blk_t));

    this->next_vaddr = VMALLOC_START;
}

/*------------------------------------------------------*/
/*! @brief  map
 */
void create_mapping(pgd_t *p_pgd, uint64_t va, uint64_t pa, uint64_t prot)
{
    // get PGD entry
    uint64_t pgd_idx = get_pgd_idx(va);
    pgd_t *p_pgd_entry = &p_pgd[pgd_idx];

    // check PGD entry
    if (*p_pgd_entry == 0) {
        pgd_t *p_new_pud = pmm_get_free_page();
        memzero(p_new_pud, PAGE_SIZE);
        *p_pgd_entry = (v2p((uint64_t)p_new_pud) & PAGE_MASK) | PTE_VALID | PTE_TABLE;
    }

    // get PUD entry
    pud_t *p_pud = (pud_t*)(p2v((*p_pgd_entry) & PAGE_MASK));
    uint64_t pud_idx = get_pud_idx(va);
    pud_t *p_pud_entry = &p_pud[pud_idx];

    // check PUD entry
    if (*p_pud_entry == 0) {
        pmd_t *p_new_pmd = (pmd_t*)pmm_get_free_page();
        memzero(p_new_pmd, PAGE_SIZE);
        *p_pud_entry = (v2p((uint64_t)p_new_pmd) & PAGE_MASK) | PTE_VALID | PTE_TABLE;
    }

    // get PMD entry
    pmd_t *p_pmd = (pmd_t*)p2v(((*p_pud_entry) & PAGE_MASK));
    uint64_t pmd_idx = get_pmd_idx(va);
    pmd_t *p_pmd_entry = &p_pmd[pmd_idx];

    // check PMD entry
    if (*p_pmd_entry == 0) {
        pte_t *p_new_pte = (pte_t*)pmm_get_free_page();
        memzero(p_new_pte, PAGE_SIZE);
        *p_pmd_entry = (v2p((uint64_t)p_new_pte) & PAGE_MASK) | PTE_VALID | PTE_TABLE;
    }

    // set physical address to PTE
    pte_t *p_pte = (pte_t*)p2v(((*p_pmd_entry)) & PAGE_MASK);
    uint64_t pte_idx = get_pte_idx(va);

    p_pte[pte_idx] = (pa & PAGE_MASK) | prot;
}

/*------------------------------------------------------*/
/*! @brief  setup page table
 */
pgd_t *setup_page_tables()
{
    struct vmm_ctrl_blk_t *this = get_myself();

    // get table for kernel PGD
    pgd_t *p_kernel_pgd = (pgd_t*)pmm_get_free_page();
    memzero(p_kernel_pgd, PAGE_SIZE);

    // mapping Normal Memory
    for (uint64_t pa = DDR_START; pa < DDR_END; pa += PAGE_SIZE) {
        create_mapping(p_kernel_pgd, p2v(pa), pa, PROT_NORMAL);
    }

    asm volatile("dsb ish" ::: "memory");

    this->p_kernel_pgd = p_kernel_pgd;

    return p_kernel_pgd;
}

/*------------------------------------------------------*/
/*! @brief  ioremap
 */
void *ioremap(uint64_t pa, uint64_t size)
{
    struct vmm_ctrl_blk_t *this = get_myself();

    // check kernel PGD
    if (this->p_kernel_pgd == NULL) {
        return NULL;
    }

    // get phyaddr to map
    uint64_t offset = pa % PAGE_SIZE;
    uint64_t aligned_pa = pa - offset;
    uint64_t aligned_size = ((size + offset + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;

    // get virtaddr to map
    uint64_t va = this->next_vaddr;
    this->next_vaddr += aligned_size;

    // get attribute
    uint64_t va_attr = PTE_VALID | PTE_PAGE | PTE_ATTR_INDEX0_DEVICE | PTE_AF;

    // mapping
    for (uint64_t i = 0; i < aligned_size; i += PAGE_SIZE) {
        create_mapping(this->p_kernel_pgd, va + i, aligned_pa + i, va_attr);
    }

    // flush TLB
    flush_tlb_all();

    return (void*)(va + offset);
}
