#!/bin/bash

set -eux

mode=${MODE:-iso}

case $mode in
    iso)
        qemu_args="-cdrom os.iso"
        ;;
    uefi)
        qemu_args="-hda fat:rw:img -bios OVMF.fd"
        ;;
    uefi-shell)
        qemu_args="-bios OVMF.fd"
        ;;
    *)
        echo "Unsupported boot mode: $mode" 2>&1; exit 1
        ;;
esac

make clean && docker run -v $PWD:/opt/kos bellkev/kos make $mode
qemu-system-i386 -D qemu.log -monitor stdio -vga cirrus \
                 -serial file:kernel.log -full-screen \
                 -nodefaults -gdb tcp:0.0.0.0:1234 \
                 $qemu_args "$@"
