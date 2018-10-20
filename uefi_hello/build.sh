#!/bin/sh

cmake -H. -Bbuilddir -GNinja -DCMAKE_TOOLCHAIN_FILE=$PWD/aarch64-uefi.cmake
cd builddir && ninja
