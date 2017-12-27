OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.s,%.o,$(wildcard *.s))
TEST_FILES := $(wildcard test_*.c)
TEST_OBJECTS := $(patsubst %.c,%.o,$(TEST_FILES)) all_tests.o
SRC_OBJECTS := $(filter-out $(TEST_OBJECTS),$(OBJECTS))
CC = gcc
CFLAGS = -m32 -Wall -Wextra -Werror -c -ggdb
AS = nasm
ASFLAGS = -f elf -Fdwarf -g
LDFLAGS = -m32
GRUB_MODULES = part_msdos part_gpt fat normal multiboot video_cirrus video video_fb videoinfo efi_uga efi_gop all_video

$(SRC_OBJECTS): CFLAGS += -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs

all: os.iso

# The kernel

# Note: Almost all targets are "phony", since everything's so fast and it's a pain to deal with the directories properly
kernel.elf: $(SRC_OBJECTS)
	mkdir -p img/boot
	ld -melf_i386 -T link.ld $(SRC_OBJECTS) -o img/boot/kernel.elf

# Test programs

all_tests.c: $(TEST_FILES)
	script/make-all-tests.sh > all_tests.c

all_tests.o: all_tests.c

all_tests: $(filter-out loader.o, $(OBJECTS))

test: all_tests
	./all_tests

# A "User space" program

hello:
	mkdir -p img/modules
	nasm -f bin hello.s -o img/modules/hello

# Grub

# The existence of the original file in the root will prevent this target from running without .PHONY
.PHONY: grub.cfg
grub.cfg:
	mkdir -p img/boot/grub
	cp grub.cfg img/boot/grub/

# ISO boot

iso: os.iso

os.iso: kernel.elf hello grub.cfg
	grub-mkrescue -o os.iso img

# UEFI boot

OVMF.fd:
	cp /root/edk2-vUDK2017/Build/OvmfIa32/RELEASE_GCC5/FV/OVMF.fd .

bootia32.efi:
	mkdir -p img/efi/boot
	grub-mkimage -O i386-efi -o img/efi/boot/bootia32.efi -p "/boot/grub" $(GRUB_MODULES)

bootx64.efi:
	mkdir -p img/efi/boot
	grub-mkimage -O x86_64-efi -o img/efi/boot/bootx64.efi -p "/boot/grub" $(GRUB_MODULES)

uefi: bootia32.efi bootx64.efi hello kernel.elf grub.cfg OVMF.fd

uefi-shell: uefi

# Misc

clean:
	rm -rf *.o img OVMF.fd os.iso all_tests*
