ENTRY(loader)                /* the name of the entry label */

SECTIONS {
    /* the code should be relocated to 3GB + 1MB */
    kernel_virtual_offset = 0xC0000000;
    . = kernel_virtual_offset + 0x00100000;
    kernel_virtual_start = .;
    kernel_physical_start = . - 0xC0000000;

    /* align at 4 KB and load at 1 MB*/
    .text ALIGN (0x1000) : AT(ADDR(.text) - kernel_virtual_offset)
    {
        /* all text sections from all files */
        *(.text)
    }
    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - kernel_virtual_offset)
    {
        *(.rodata*)
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - kernel_virtual_offset)
    {
        *(.data)
    }

    .bss ALIGN (0x1000) : AT(ADDR(.bss) - kernel_virtual_offset)
    {
        *(COMMON)
        *(.bss)
    }

    kernel_virtual_end = .;
    kernel_physical_end = . - 0xC0000000;
}
