#!/bin/sh -x

PWD_BACKUP=$PWD
SYSROOT_BASE=$HOME/opt/llvm/
SYSROOT_NAME=sysroot-aarch64-elf
SYSROOT_URL=https://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-elf/sysroot-newlib-linaro-2018.05-aarch64-elf.tar.xz

# fetch OVMF image
mkdir $HOME/opt/ovmf
cd $HOME/opt/ovmf
wget http://snapshots.linaro.org/components/kernel/leg-virt-tianocore-edk2-upstream/latest/QEMU-AARCH64/RELEASE_CLANG35/QEMU_EFI.fd -O QEMU_EFI_AARCH64.fd
cat QEMU_EFI_AARCH64.fd /dev/zero | dd iflag=fullblock bs=1M count=64 of=flash0.img
dd if=/dev/zero of=flash1.img bs=1M count=64

# sysroot setup
cd $PWD_BACKUP
mkdir -p $SYSROOT_BASE/$SYSROOT_NAME
cd $SYSROOT_BASE/$SYSROOT_NAME
wget -qO- $SYSROOT_URL | tar Jxv --strip-components=1

# install elf.h from newlib
cd $PWD_BACKUP
wget -qO- ftp://sourceware.org/pub/newlib/newlib-3.0.0.tar.gz | tar zxv 
cp ./newlib-3.0.0/newlib/libc/include/elf.h $SYSROOT_BASE/$SYSROOT_NAME/usr/include/
rm -rf newlib-3.0.0

# install gnu-efi
cd $PWD_BACKUP
wget -qO- https://jaist.dl.sourceforge.net/project/gnu-efi/gnu-efi-3.0.8.tar.bz2 | tar jxv 
cd gnu-efi-3.0.8
make CROSS_COMPILE=aarch64-elf- PREFIX=$SYSROOT_BASE/$SYSROOT_NAME/usr ARCH=aarch64 CFLAGS=--sysroot=$SYSROOT_BASE/$SYSROOT_NAME -j`nproc`
make CROSS_COMPILE=aarch64-elf- PREFIX=$SYSROOT_BASE/$SYSROOT_NAME/usr ARCH=aarch64 CFLAGS=--sysroot=$SYSROOT_BASE/$SYSROOT_NAME install
cd ..
rm -rf gnu-efi-3.0.8