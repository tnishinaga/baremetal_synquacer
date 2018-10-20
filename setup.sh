#!/bin/sh

# fetch OVMF image
mkdir $HOME/opt/ovmf
wget http://snapshots.linaro.org/components/kernel/leg-virt-tianocore-edk2-upstream/latest/QEMU-AARCH64/RELEASE_CLANG35/QEMU_EFI.fd -O $HOME/opt/ovmf/QEMU_EFI_AARCH64.fd


# create sysroot dir
mkdir -p $HOME/opt/llvm/aarch64-none-eabi
wget -qO- https://jaist.dl.sourceforge.net/project/gnu-efi/gnu-efi-3.0.8.tar.bz2 | tar xvf 
cd gnu-efi-3.0.8/inc
make PREFIX=$HOME/opt/llvm/aarch64-none-eabi ARCH=aarch64 install
