#!/bin/bash

set -e

docker build -t bellkev/kos .
docker run -v $PWD:/root bellkev/kos make
qemu-system-x86_64 -cdrom os.iso -monitor stdio -serial file:kernel.log -full-screen
