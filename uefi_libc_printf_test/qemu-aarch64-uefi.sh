#!/bin/sh

flash0=$HOME/opt/ovmf/flash0.img
flash1=$HOME/opt/ovmf/flash1.img

qemu-system-aarch64 \
    -m 512 \
    -cpu cortex-a57 \
    -M virt \
    -pflash $flash0 -pflash $flash1 \
    -hda fat:rw:.\
    -nographic
