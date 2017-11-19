global loader
extern kmain

MAGIC_NUMBER equ 0x1BADB002
ALIGN_MODULES equ 0x00000001
CHECKSUM equ -(MAGIC_NUMBER + ALIGN_MODULES)
KERNEL_STACK_SIZE equ 4096
LETTER_A equ 0xf041
LETTER_B equ 0xf042
KERNEL_VIRTUAL_BASE equ 0xC0000000                  ; 3GB
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)  ; Page directory index of kernel's 4MB PTE.

section .bss
align 4
kernel_stack:
        resb KERNEL_STACK_SIZE

section .data
align 0x1000
page_directory:
        ; This page directory entry identity-maps the first 4MB of the 32-bit physical address space.
        ; All bits are clear except the following:
        ; bit 7: PS The kernel page is 4MB.
        ; bit 1: RW The kernel page is read/write.
        ; bit 0: P  The kernel page is present.
        ; This entry must be here -- otherwise the kernel will crash immediately after paging is
        ; enabled because it can't fetch the next instruction! It's ok to unmap this page later.
        dd 0x00000083
        ; times (KERNEL_PAGE_NUMBER - 1) dd 0                 ; Pages before kernel space.
        ; ; This page directory entry defines a 4MB page containing the kernel.
        ; dd 0x00000083
        ; times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0  ; Pages after the kernel image.


section .text
align 4
        dd MAGIC_NUMBER
        dd ALIGN_MODULES
        dd CHECKSUM

loader:
        ;; NOTE: Make sure not to touch ebx before kmain
        mov eax, page_directory
        mov cr3, eax

        mov eax, cr4        ; read current cr4
        or  eax, 0x00000010 ; set PSE
        mov cr4, eax        ; update cr4

        mov eax, cr0        ; read current cr0
        or  eax, 0x80000000 ; set PG
        mov cr0, eax        ; update cr0

        ; now paging is enabled

        ;; Set up stack and call hello fn
        mov esp, kernel_stack + KERNEL_STACK_SIZE
        ;; Give kmain a pointer to multiboot info
        push ebx
        call kmain

.loop:
        jmp .loop
