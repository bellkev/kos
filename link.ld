ENTRY(loader)                /* the name of the entry label */

SECTIONS {
    /* the code should be relocated to 3GB + 1MB */
    . = 0xC0100000;

    /* align at 4 KB and load at 1 MB*/
    .text ALIGN (0x1000) : AT(ADDR(.text) - 0xC0000000)
    {
        /* all text sections from all files */
        *(.text)
    }
    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata*)
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data)
    }

    .bss ALIGN (0x1000) : AT(ADDR(.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss)
    }
}
