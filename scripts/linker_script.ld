/*------------------------------------------------------*/
/*!
 *      @file   linker_script.ld
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  linker script for kernel
 *      @note
 */
/*------------------------------------------------------*/

KERNEL_OFFSET = 0xFFFF000000000000;

/* entry point */
ENTRY(_start)

/* The definition of Program Header */
PHDRS
{
    text_seg PT_LOAD FLAGS(5);      /* 5 = Read(4) + Execute(1) */
    data_seg PT_LOAD FLAGS(6);      /* 6 = Read(4) + Write(2) */
}

SECTIONS
{
    /* Virtual Memory Address */
    . = 0x40080000 + KERNEL_OFFSET;

    /* text segment */
    .text : AT(ADDR(.text) - KERNEL_OFFSET) {
        __text_start = .;
        *(.text.boot)
        *(.text)
        __text_end = .;
    } :text_seg

    /* read only segment */
    .rodata : AT(ADDR(.rodata) - KERNEL_OFFSET) {
        *(.rodata)
    } :text_seg

    /* data segment */
    .data : AT(ADDR(.data) - KERNEL_OFFSET) {
        *(.data)
    } :data_seg

    /* bss segment */
    .bss : AT(ADDR(.bss) - KERNEL_OFFSET) {
        . = ALIGN(8);
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    } :data_seg

    /* for page allocator */
    . = ALIGN(4096);
    __kernel_end = .;
}
