# kos

Kevin's OS.

## Miscellaneous notes

ISOs with GRUB 2:

* If xorriso is not installed, `grub-mkrescue` exits with just a warning and code 0 without actually making an iso...
* Some useful info: http://wiki.osdev.org/GRUB

Framebuffer:

* The [wikipedia article](https://en.wikipedia.org/wiki/VGA-compatible_text_mode#Text_buffer) suggests that the char/properties byte order is the reverse of littleosbook
* This is also the behavior I see in QEMU
* Okay, I guess this could actually be a difference in endianness somewhere between my setup and littleosbook...

LD:

* https://www.math.utah.edu/docs/info/ld_3.html is a useful overview of the scripting format
* Turns out `section .text:` (with a colon) is extra not-good. (The `.text:` section isn't caught by the `.text` label in the linker script, meaning it winds up too far down the file for GRUB to find the multiboot header.)

VGA:

* Further reading: http://wiki.osdev.org/VGA_Hardware


x86:
* Syntax like `out 0x3D4, 14` doesn't seem to work at all... Even `out imm8 AL` type operands don't seem to work for the ~16bit cursor port address, resulting in `warning: unsigned byte value exceeds bounds` from `nasm`. Only had success with `out dx, al`.
* Useful register reference: http://www.cs.virginia.edu/~evans/cs216/guides/x86.html

qemu fullscreen:
* `s/[NSScreen mainScreen]/[[NSScreen screens] objectAtIndex:0]/` (in QEMU source) let's you start the QEMU screen in a specific display, rather than the one currently receiving keyboard events

Storing programs in modules:
* [BITS 32] seems to be necessary
* multiboot.h on master in grub seems to have issues now, using https://www.gnu.org/software/grub/manual/multiboot/html_node/multiboot_002eh.html
