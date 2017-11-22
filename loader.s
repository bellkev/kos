global loader
extern kmain

        MAGIC_NUMBER equ 0x1BADB002
        ALIGN_MODULES equ 1<<0
        MEM_INFO equ 1<<1
        FLAGS equ ALIGN_MODULES
        CHECKSUM equ -(MAGIC_NUMBER + FLAGS)
        KERNEL_STACK_SIZE equ 4096
        LETTER_A equ 0xf041
        LETTER_B equ 0xf042
        KERNEL_VIRTUAL_BASE equ 0xC0000000                  ; 3GB
        KERNEL_PAGE_INDEX equ (KERNEL_VIRTUAL_BASE >> 22)  ; Page directory index of kernel's 4MB PTE.

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
        times (KERNEL_PAGE_INDEX - 1) dd 0                 ; Pages before kernel space.
        ; This page directory entry defines a 4MB page containing the kernel.
        dd 0x00000083
        times (1024 - KERNEL_PAGE_INDEX - 1) dd 0  ; Pages after the kernel image.


section .text
align 4
        dd MAGIC_NUMBER
        dd FLAGS
        dd CHECKSUM

loader:
        ; NOTE: Make sure not to touch ebx before kmain
        lea eax, [page_directory - KERNEL_VIRTUAL_BASE]
        mov cr3, eax

        mov eax, cr4        ; read current cr4
        or  eax, 0x00000010 ; set PSE
        mov cr4, eax        ; update cr4

        mov eax, cr0        ; read current cr0
        or  eax, 0x80000000 ; set PG
        mov cr0, eax        ; update cr0

        ; now paging is enabled
        ; Start fetching instructions in kernel space.
        ; Since eip at this point holds the physical address of this command (approximately 0x00100000)
        ; we need to do a long jump to the correct virtual address of aStartInHigherHalf which is
        ; approximately 0xC0100000.
        lea eax, [higher_half]
        jmp eax ; NOTE: Must be absolute jump!


higher_half:
        ; Unmap the identity-mapped first 4MB of physical address space. It should not be needed
        ; anymore.
        mov dword [page_directory], 0
        invlpg [0]

        ; NOTE: From now on, paging should be enabled. The first 4MB of physical address space is
        ; mapped starting at KERNEL_VIRTUAL_BASE. Everything is linked to this address, so no more
        ; position-independent code or funny business with virtual-to-physical address translation
        ; should be necessary. We now have a higher-half kernel.
        mov esp, kernel_stack + KERNEL_STACK_SIZE           ; set up the stack

        ; pass Multiboot info structure -- WARNING: This is a physical address and may not be
        ; in the first 4MB!
        push ebx

        call kmain
        jmp $
