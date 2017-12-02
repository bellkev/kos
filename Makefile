OBJECTS = interrupt_macro.o interrupt.o io.o pic.o loader.o kmain.o serial.o utils.o framebuffer.o scancodes.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -ggdb
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf -Fdwarf -g

all: bootia32.efi hello kernel.elf img/boot/grub/grub.cfg OVMF.fd

OVMF.fd:
	cp /root/edk2-vUDK2017/Build/OvmfIa32/RELEASE_GCC5/FV/OVMF.fd .

kernel.elf: $(OBJECTS)
	mkdir -p img/boot
	ld $(LDFLAGS) $(OBJECTS) -o img/boot/kernel.elf

hello:
	mkdir -p img/modules
	nasm -f bin hello.s -o img/modules/hello

bootia32.efi:
	mkdir -p img/efi/boot
	grub-mkimage -O i386-efi -o img/efi/boot/bootia32.efi -p "/boot/grub" part_msdos part_gpt fat normal multiboot video_cirrus video video_fb videoinfo efi_uga efi_gop all_video

img/boot/grub/grub.cfg:
	mkdir -p img/boot/grub
	cp grub.cfg img/boot/grub/

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o img
