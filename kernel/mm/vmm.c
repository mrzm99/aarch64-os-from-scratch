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
#include "vmm.h"
#include "pmm.h"
#include "../../include/config.h"
#include "../../board/virt/board.h"

/*------------------------------------------------------*/
/*! @brief  PTE index operation macro
 */
#define get_pgd_idx(addr)           (((addr) >> 39) & (PTE_INDEX_MASK))
#define get_pud_idx(addr)           (((addr) >> 30) & (PTE_INDEX_MASK))
#define get_pmd_idx(addr)           (((addr) >> 21) & (PTE_INDEX_MASK))
#define get_pte_idx(addr)           (((addr) >> 12) & (PTE_INDEX_MASK))

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
    // get table for kernel PGD
    pgd_t *p_kernel_pgd = (pgd_t*)pmm_get_free_page();
    memzero(p_kernel_pgd, PAGE_SIZE);

    // mapping Normal Memory (16M)
    for (uint64_t pa = DDR_START; pa < DDR_END; pa += PAGE_SIZE) {
        create_mapping(p_kernel_pgd, p2v(pa), pa, PROT_NORMAL);
    }

    // mapping Device Memory
    uint64_t uart_base = UART_BASE_ADDR;
    create_mapping(p_kernel_pgd, p2v(uart_base), uart_base, PROT_DEVICE);

    asm volatile("dsb ish" ::: "memory");

    return p_kernel_pgd;
}
