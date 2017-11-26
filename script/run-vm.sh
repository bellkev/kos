#!/bin/bash

set -e

rm *.log || true
docker build -t bellkev/kos .
make clean && docker run -v $PWD:/opt/kos bellkev/kos make
~/source/qemu/build/i386-softmmu/qemu-system-i386 -bios OVMF.fd -D qemu.log -hda fat:rw:img -monitor stdio -vga cirrus -serial file:kernel.log -full-screen -gdb tcp:0.0.0.0:1234 -S -nodefaults
