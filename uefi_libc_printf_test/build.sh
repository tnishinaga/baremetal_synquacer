#!/bin/sh

cmake -H./uefi_loader -Bbuilddir/uefi_loader -GNinja -DCMAKE_TOOLCHAIN_FILE=$PWD/cmake_toolchains/aarch64-llvm-uefi.cmake
pushd builddir/uefi_loader && ninja
popd

cmake -H./kernel -Bbuilddir/kernel -GNinja -DCMAKE_TOOLCHAIN_FILE=$PWD/cmake_toolchains/aarch64-llvm-baremetal.cmake
pushd builddir/kernel && ninja
popd

