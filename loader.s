global loader
extern kmain

        MAGIC_NUMBER equ 0x1BADB002
        ALIGN_MODULES equ 0x00000001
        CHECKSUM equ -(MAGIC_NUMBER + ALIGN_MODULES)
        KERNEL_STACK_SIZE equ 4096
        LETTER_A equ 0xf041
        LETTER_B equ 0xf042

section .bss
align 4
kernel_stack:
        resb KERNEL_STACK_SIZE

section .text
align 4
        dd MAGIC_NUMBER
        dd ALIGN_MODULES
        dd CHECKSUM

loader:
        ;; Set up stack and call hello fn
        mov esp, kernel_stack + KERNEL_STACK_SIZE
        ;; Give kmain a pointer to multiboot info
        push ebx
        call kmain

.loop:
        jmp .loop
