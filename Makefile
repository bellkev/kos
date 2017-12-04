OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.s,%.o,$(wildcard *.s))
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -ggdb
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf -Fdwarf -g

GRUB_MODULES = part_msdos part_gpt fat normal multiboot video_cirrus video video_fb videoinfo efi_uga efi_gop all_video

all: os.iso

iso: os.iso

os.iso: kernel.elf hello grub.cfg
	grub-mkrescue -o os.iso img

uefi: bootia32.efi bootx64.efi hello kernel.elf grub.cfg OVMF.fd

# Note: Almost all targets are "phony", since everything's so fast and it's a pain to deal with the directories properly

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
	grub-mkimage -O i386-efi -o img/efi/boot/bootia32.efi -p "/boot/grub" $(GRUB_MODULES)

bootx64.efi:
	mkdir -p img/efi/boot
	grub-mkimage -O x86_64-efi -o img/efi/boot/bootx64.efi -p "/boot/grub" $(GRUB_MODULES)

# The existence of the original file in the root will prevent this target from running without .PHONY
.PHONY: grub.cfg
grub.cfg:
	mkdir -p img/boot/grub
	cp grub.cfg img/boot/grub/

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o img OVMF.fd os.iso
