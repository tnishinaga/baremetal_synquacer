#!/bin/sh

ovmf=$HOME/opt/ovmf/QEMU_EFI_AARCH64.fd

qemu-system-aarch64 \
    -m 512 \
    -cpu cortex-a57 \
    -M virt \
    -bios $ovmf \
    -hda fat:rw:.\
    -nographic
