# kos

Kevin's OS.

## Miscellaneous notes

ISOs with GRUB 2:

* If xorriso is not installed, `grub-mkrescue` exits with just a warning and code 0 without actually making an iso...
* Some useful info: http://wiki.osdev.org/GRUB

Framebuffer:

* The [wikipedia article](https://en.wikipedia.org/wiki/VGA-compatible_text_mode#Text_buffer) suggests that the char/properties byte order is the reverse of littleosbook
* This is also the behavior I see in QEMU
