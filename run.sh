#!/bin/bash

set -e

docker build -t bellkev/kos .
docker run -v $PWD:/root bellkev/kos make
qemu-system-i386 -D qemu.log -d int,cpu_reset -cdrom os.iso -monitor stdio -serial file:kernel.log -full-screen
