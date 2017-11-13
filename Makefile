OBJECTS = interrupt_macro.o interrupt.o io.o pic.o loader.o kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -ggdb
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf -Fdwarf -g

all: os.iso

kernel.elf: $(OBJECTS)
	echo "ld $(LDFLAGS) $(OBJECTS) -o kernel.elf"
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	mkdir -p disk/boot/grub
	cp grub.cfg disk/boot/grub
	cp kernel.elf disk/boot/kernel.elf
	grub-mkrescue -o os.iso disk

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso disk
