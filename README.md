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
