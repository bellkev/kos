global loader
extern sum_of_three

        MAGIC_NUMBER equ 0x1BADB002
        FLAGS equ 0x0
        CHECKSUM equ -MAGIC_NUMBER
        KERNEL_STACK_SIZE equ 4096
        LETTER_A equ 0xf041
        LETTER_B equ 0xf042

section .bss
align 4
kernel_stack:
        resb KERNEL_STACK_SIZE

section .text:
align 4
        dd MAGIC_NUMBER
        dd FLAGS
        dd CHECKSUM

loader:
        mov esp, kernel_stack + KERNEL_STACK_SIZE
        mov [0x000B8000], word LETTER_A
        mov [0x000B8002], word LETTER_B
.loop:
        jmp .loop
