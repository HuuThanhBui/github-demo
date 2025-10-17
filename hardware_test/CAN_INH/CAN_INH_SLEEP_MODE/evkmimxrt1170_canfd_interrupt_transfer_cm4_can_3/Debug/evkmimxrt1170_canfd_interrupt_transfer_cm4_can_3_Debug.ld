/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright 2008-2013 Code Red Technologies Ltd,
 * Copyright 2013-2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 * Generated linker script file for MIMXRT1176xxxxx
 * Created from linkscript.ldt by FMCreateLinkLibraries
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v25.6 [Build 136] [2025-06-27] on Oct 16, 2025, 3:20:05 PM
 */

INCLUDE "evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3_Debug_library.ld"
INCLUDE "evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3_Debug_memory.ld"

ENTRY(ResetISR)

SECTIONS
{
     /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        LONG(LOADADDR(.data_RAM4));
        LONG(    ADDR(.data_RAM4));
        LONG(  SIZEOF(.data_RAM4));
        LONG(LOADADDR(.data_RAM5));
        LONG(    ADDR(.data_RAM5));
        LONG(  SIZEOF(.data_RAM5));
        LONG(LOADADDR(.data_RAM6));
        LONG(    ADDR(.data_RAM6));
        LONG(  SIZEOF(.data_RAM6));
        LONG(LOADADDR(.data_RAM7));
        LONG(    ADDR(.data_RAM7));
        LONG(  SIZEOF(.data_RAM7));
        LONG(LOADADDR(.data_RAM8));
        LONG(    ADDR(.data_RAM8));
        LONG(  SIZEOF(.data_RAM8));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        LONG(    ADDR(.bss_RAM4));
        LONG(  SIZEOF(.bss_RAM4));
        LONG(    ADDR(.bss_RAM5));
        LONG(  SIZEOF(.bss_RAM5));
        LONG(    ADDR(.bss_RAM6));
        LONG(  SIZEOF(.bss_RAM6));
        LONG(    ADDR(.bss_RAM7));
        LONG(  SIZEOF(.bss_RAM7));
        LONG(    ADDR(.bss_RAM8));
        LONG(  SIZEOF(.bss_RAM8));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)

       *(.text*)
       *(.rodata .rodata.* .constdata .constdata.*)
       . = ALIGN(4);
    } > SRAM_DTC_cm4
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this.
     */
    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > SRAM_DTC_cm4

    .ARM.exidx : ALIGN(4)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > SRAM_DTC_cm4
 
    _etext = .;
        
    /* DATA section for SRAM_ITC_cm4 */

    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        PROVIDE(__start_data_SRAM_ITC_cm4 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$SRAM_ITC_cm4)
        *(.data.$RAM2)
        *(.data.$SRAM_ITC_cm4)
        *(.data.$RAM2.*)
        *(.data.$SRAM_ITC_cm4.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
        PROVIDE(__end_data_SRAM_ITC_cm4 = .) ;
     } > SRAM_ITC_cm4 AT>SRAM_DTC_cm4

    /* DATA section for SRAM_OC1 */

    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        PROVIDE(__start_data_SRAM_OC1 = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$SRAM_OC1)
        *(.data.$RAM3)
        *(.data.$SRAM_OC1)
        *(.data.$RAM3.*)
        *(.data.$SRAM_OC1.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
        PROVIDE(__end_data_SRAM_OC1 = .) ;
     } > SRAM_OC1 AT>SRAM_DTC_cm4

    /* DATA section for NCACHE_REGION */

    .data_RAM4 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM4 = .) ;
        PROVIDE(__start_data_NCACHE_REGION = .) ;
        *(.ramfunc.$RAM4)
        *(.ramfunc.$NCACHE_REGION)
       *(NonCacheable.init)
        *(.data.$RAM4)
        *(.data.$NCACHE_REGION)
        *(.data.$RAM4.*)
        *(.data.$NCACHE_REGION.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM4 = .) ;
        PROVIDE(__end_data_NCACHE_REGION = .) ;
     } > NCACHE_REGION AT>SRAM_DTC_cm4

    /* DATA section for SRAM_OC2 */

    .data_RAM5 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM5 = .) ;
        PROVIDE(__start_data_SRAM_OC2 = .) ;
        *(.ramfunc.$RAM5)
        *(.ramfunc.$SRAM_OC2)
        *(.data.$RAM5)
        *(.data.$SRAM_OC2)
        *(.data.$RAM5.*)
        *(.data.$SRAM_OC2.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM5 = .) ;
        PROVIDE(__end_data_SRAM_OC2 = .) ;
     } > SRAM_OC2 AT>SRAM_DTC_cm4

    /* DATA section for SRAM_OC_ECC1 */

    .data_RAM6 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM6 = .) ;
        PROVIDE(__start_data_SRAM_OC_ECC1 = .) ;
        *(.ramfunc.$RAM6)
        *(.ramfunc.$SRAM_OC_ECC1)
        *(.data.$RAM6)
        *(.data.$SRAM_OC_ECC1)
        *(.data.$RAM6.*)
        *(.data.$SRAM_OC_ECC1.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM6 = .) ;
        PROVIDE(__end_data_SRAM_OC_ECC1 = .) ;
     } > SRAM_OC_ECC1 AT>SRAM_DTC_cm4

    /* DATA section for SRAM_OC_ECC2 */

    .data_RAM7 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM7 = .) ;
        PROVIDE(__start_data_SRAM_OC_ECC2 = .) ;
        *(.ramfunc.$RAM7)
        *(.ramfunc.$SRAM_OC_ECC2)
        *(.data.$RAM7)
        *(.data.$SRAM_OC_ECC2)
        *(.data.$RAM7.*)
        *(.data.$SRAM_OC_ECC2.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM7 = .) ;
        PROVIDE(__end_data_SRAM_OC_ECC2 = .) ;
     } > SRAM_OC_ECC2 AT>SRAM_DTC_cm4

    /* DATA section for BOARD_SDRAM */

    .data_RAM8 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM8 = .) ;
        PROVIDE(__start_data_BOARD_SDRAM = .) ;
        *(.ramfunc.$RAM8)
        *(.ramfunc.$BOARD_SDRAM)
        *(.data.$RAM8)
        *(.data.$BOARD_SDRAM)
        *(.data.$RAM8.*)
        *(.data.$BOARD_SDRAM.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM8 = .) ;
        PROVIDE(__end_data_BOARD_SDRAM = .) ;
     } > BOARD_SDRAM AT>SRAM_DTC_cm4

    /* MAIN DATA SECTION */
    .uninit_RESERVED (NOLOAD) : ALIGN(4)
    {
        _start_uninit_RESERVED = .;
        KEEP(*(.bss.$RESERVED*))
       . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > SRAM_DTC_cm4 AT> SRAM_DTC_cm4

    /* Main DATA section (SRAM_DTC_cm4) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       PROVIDE(__start_data_RAM = .) ;
       PROVIDE(__start_data_SRAM_DTC_cm4 = .) ;
       *(vtable)
       *(.ramfunc*)
       KEEP(*(CodeQuickAccess))
       KEEP(*(DataQuickAccess))
       *(RamFunction)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
       PROVIDE(__end_data_RAM = .) ;
       PROVIDE(__end_data_SRAM_DTC_cm4 = .) ;
    } > SRAM_DTC_cm4 AT>SRAM_DTC_cm4

    /* BSS section for SRAM_ITC_cm4 */
    .bss_RAM2 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       PROVIDE(__start_bss_SRAM_ITC_cm4 = .) ;
       *(.bss.$RAM2)
       *(.bss.$SRAM_ITC_cm4)
       *(.bss.$RAM2.*)
       *(.bss.$SRAM_ITC_cm4.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
       PROVIDE(__end_bss_SRAM_ITC_cm4 = .) ;
    } > SRAM_ITC_cm4 AT> SRAM_ITC_cm4

    /* BSS section for SRAM_OC1 */
    .bss_RAM3 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       PROVIDE(__start_bss_SRAM_OC1 = .) ;
       *(.bss.$RAM3)
       *(.bss.$SRAM_OC1)
       *(.bss.$RAM3.*)
       *(.bss.$SRAM_OC1.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
       PROVIDE(__end_bss_SRAM_OC1 = .) ;
    } > SRAM_OC1 AT> SRAM_OC1

    /* BSS section for NCACHE_REGION */
    .bss_RAM4 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM4 = .) ;
       PROVIDE(__start_bss_NCACHE_REGION = .) ;
       *(NonCacheable)
       *(.bss.$RAM4)
       *(.bss.$NCACHE_REGION)
       *(.bss.$RAM4.*)
       *(.bss.$NCACHE_REGION.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM4 = .) ;
       PROVIDE(__end_bss_NCACHE_REGION = .) ;
    } > NCACHE_REGION AT> NCACHE_REGION

    /* BSS section for SRAM_OC2 */
    .bss_RAM5 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM5 = .) ;
       PROVIDE(__start_bss_SRAM_OC2 = .) ;
       *(.bss.$RAM5)
       *(.bss.$SRAM_OC2)
       *(.bss.$RAM5.*)
       *(.bss.$SRAM_OC2.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM5 = .) ;
       PROVIDE(__end_bss_SRAM_OC2 = .) ;
    } > SRAM_OC2 AT> SRAM_OC2

    /* BSS section for SRAM_OC_ECC1 */
    .bss_RAM6 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM6 = .) ;
       PROVIDE(__start_bss_SRAM_OC_ECC1 = .) ;
       *(.bss.$RAM6)
       *(.bss.$SRAM_OC_ECC1)
       *(.bss.$RAM6.*)
       *(.bss.$SRAM_OC_ECC1.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM6 = .) ;
       PROVIDE(__end_bss_SRAM_OC_ECC1 = .) ;
    } > SRAM_OC_ECC1 AT> SRAM_OC_ECC1

    /* BSS section for SRAM_OC_ECC2 */
    .bss_RAM7 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM7 = .) ;
       PROVIDE(__start_bss_SRAM_OC_ECC2 = .) ;
       *(.bss.$RAM7)
       *(.bss.$SRAM_OC_ECC2)
       *(.bss.$RAM7.*)
       *(.bss.$SRAM_OC_ECC2.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM7 = .) ;
       PROVIDE(__end_bss_SRAM_OC_ECC2 = .) ;
    } > SRAM_OC_ECC2 AT> SRAM_OC_ECC2

    /* BSS section for BOARD_SDRAM */
    .bss_RAM8 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM8 = .) ;
       PROVIDE(__start_bss_BOARD_SDRAM = .) ;
       *(.bss.$RAM8)
       *(.bss.$BOARD_SDRAM)
       *(.bss.$RAM8.*)
       *(.bss.$BOARD_SDRAM.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM8 = .) ;
       PROVIDE(__end_bss_BOARD_SDRAM = .) ;
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* MAIN BSS SECTION */
    .bss (NOLOAD) : ALIGN(4)
    {
        _bss = .;
        PROVIDE(__start_bss_RAM = .) ;
        PROVIDE(__start_bss_SRAM_DTC_cm4 = .) ;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(__end_bss_RAM = .) ;
        PROVIDE(__end_bss_SRAM_DTC_cm4 = .) ;
        PROVIDE(end = .);
    } > SRAM_DTC_cm4 AT> SRAM_DTC_cm4

    /* NOINIT section for SRAM_ITC_cm4 */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM2 = .) ;
       PROVIDE(__start_noinit_SRAM_ITC_cm4 = .) ;
       *(.noinit.$RAM2)
       *(.noinit.$SRAM_ITC_cm4)
       *(.noinit.$RAM2.*)
       *(.noinit.$SRAM_ITC_cm4.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM2 = .) ;
       PROVIDE(__end_noinit_SRAM_ITC_cm4 = .) ;
    } > SRAM_ITC_cm4 AT> SRAM_ITC_cm4

    /* NOINIT section for SRAM_OC1 */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM3 = .) ;
       PROVIDE(__start_noinit_SRAM_OC1 = .) ;
       *(.noinit.$RAM3)
       *(.noinit.$SRAM_OC1)
       *(.noinit.$RAM3.*)
       *(.noinit.$SRAM_OC1.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM3 = .) ;
       PROVIDE(__end_noinit_SRAM_OC1 = .) ;
    } > SRAM_OC1 AT> SRAM_OC1

    /* NOINIT section for NCACHE_REGION */
    .noinit_RAM4 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM4 = .) ;
       PROVIDE(__start_noinit_NCACHE_REGION = .) ;
       *(.noinit.$RAM4)
       *(.noinit.$NCACHE_REGION)
       *(.noinit.$RAM4.*)
       *(.noinit.$NCACHE_REGION.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM4 = .) ;
       PROVIDE(__end_noinit_NCACHE_REGION = .) ;
    } > NCACHE_REGION AT> NCACHE_REGION

    /* NOINIT section for SRAM_OC2 */
    .noinit_RAM5 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM5 = .) ;
       PROVIDE(__start_noinit_SRAM_OC2 = .) ;
       *(.noinit.$RAM5)
       *(.noinit.$SRAM_OC2)
       *(.noinit.$RAM5.*)
       *(.noinit.$SRAM_OC2.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM5 = .) ;
       PROVIDE(__end_noinit_SRAM_OC2 = .) ;
    } > SRAM_OC2 AT> SRAM_OC2

    /* NOINIT section for SRAM_OC_ECC1 */
    .noinit_RAM6 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM6 = .) ;
       PROVIDE(__start_noinit_SRAM_OC_ECC1 = .) ;
       *(.noinit.$RAM6)
       *(.noinit.$SRAM_OC_ECC1)
       *(.noinit.$RAM6.*)
       *(.noinit.$SRAM_OC_ECC1.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM6 = .) ;
       PROVIDE(__end_noinit_SRAM_OC_ECC1 = .) ;
    } > SRAM_OC_ECC1 AT> SRAM_OC_ECC1

    /* NOINIT section for SRAM_OC_ECC2 */
    .noinit_RAM7 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM7 = .) ;
       PROVIDE(__start_noinit_SRAM_OC_ECC2 = .) ;
       *(.noinit.$RAM7)
       *(.noinit.$SRAM_OC_ECC2)
       *(.noinit.$RAM7.*)
       *(.noinit.$SRAM_OC_ECC2.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM7 = .) ;
       PROVIDE(__end_noinit_SRAM_OC_ECC2 = .) ;
    } > SRAM_OC_ECC2 AT> SRAM_OC_ECC2

    /* NOINIT section for BOARD_SDRAM */
    .noinit_RAM8 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM8 = .) ;
       PROVIDE(__start_noinit_BOARD_SDRAM = .) ;
       *(.noinit.$RAM8)
       *(.noinit.$BOARD_SDRAM)
       *(.noinit.$RAM8.*)
       *(.noinit.$BOARD_SDRAM.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM8 = .) ;
       PROVIDE(__end_noinit_BOARD_SDRAM = .) ;
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        PROVIDE(__start_noinit_RAM = .) ;
        PROVIDE(__start_noinit_SRAM_DTC_cm4 = .) ;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
       PROVIDE(__end_noinit_RAM = .) ;
       PROVIDE(__end_noinit_SRAM_DTC_cm4 = .) ;        
    } > SRAM_DTC_cm4 AT> SRAM_DTC_cm4

    /* Reserve and place Heap within memory map */
    _HeapSize = 0x1000;
    .heap (NOLOAD) :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > SRAM_DTC_cm4

     _StackSize = 0x1000;
     /* Reserve space in memory for Stack */
    .heap2stackfill (NOLOAD) :
    {
        . += _StackSize;
    } > SRAM_DTC_cm4
    /* Locate actual Stack in memory map */
    .stack ORIGIN(SRAM_DTC_cm4) + LENGTH(SRAM_DTC_cm4) - _StackSize - 0 (NOLOAD) :  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        _vStackTop = . + _StackSize;
    } > SRAM_DTC_cm4

    /* Provide basic symbols giving location and size of main text
     * block, including initial values of RW data sections. Note that
     * these will need extending to give a complete picture with
     * complex images (e.g multiple Flash banks).
     */
    _image_start = LOADADDR(.text);
    _image_end = LOADADDR(.data) + SIZEOF(.data);
    _image_size = _image_end - _image_start;
    /* Provide symbols for MIMXRT1170 parts for boot header generation code
     * to set image to be plain load image or XIP.
     * Config : Plain load image = false
     */
    _boot_loadaddr = ORIGIN(SRAM_DTC_cm4);
    _boot_size = LENGTH(SRAM_DTC_cm4);
}