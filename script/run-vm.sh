#!/bin/bash

set -e

rm *.log || true
docker build -t bellkev/kos .
make clean && docker run -v $PWD:/root bellkev/kos make
~/source/qemu/build/i386-softmmu/qemu-system-i386 -D qemu.log -cdrom os.iso -monitor stdio -serial file:kernel.log -full-screen -gdb tcp:0.0.0.0:1234 -S
