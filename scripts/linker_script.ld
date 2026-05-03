/*------------------------------------------------------*/
/*!
 *      @file   linker_script.ld
 *      @date   2026.xx.xx
 *      @author mrzm99
 *      @brief  linker script for kernel
 *      @note
 */
/*------------------------------------------------------*/

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
    /* DDR address */
    . = 0x40000000;

    /* text segment */
    .text : {
        *(.text.boot)
        *(.text)
    } :text_seg

    /* read only segment */
    .rodata : {
        *(.rodata)
    } :text_seg

    /* data segment */
    .data : {
        *(.data)
    } :data_seg

    /* bss segment */
    .bss : {
        . = ALIGN(8);
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    } :data_seg
}
