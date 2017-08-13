global loader
extern hello

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

section .text
align 4
        dd MAGIC_NUMBER
        dd FLAGS
        dd CHECKSUM

loader:
        mov esp, kernel_stack + KERNEL_STACK_SIZE
        call hello
        ; 0x3D4 is the port that describes the data
        ; and port 0x3D5 is the data itself
        mov dx, 0x3D4
        mov al, 14
        out dx, al

        mov dx, 0x3D5
        mov al, 0
        out dx, al

        mov dx, 0x3D4
        mov al, 15
        out dx, al

        mov dx, 0x3D5
        mov al, 15
        out dx, al

.loop:
        jmp .loop
