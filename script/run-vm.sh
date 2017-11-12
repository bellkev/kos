#!/bin/bash

set -e

docker build -t bellkev/kos .
make clean && docker run -v $PWD:/root bellkev/kos make
qemu-system-i386 -S -gdb tcp:0.0.0.0:1234 -D qemu.log -d int,cpu_reset -cdrom os.iso -monitor stdio -serial file:kernel.log -full-screen
