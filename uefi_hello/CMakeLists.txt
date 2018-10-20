
cmake_minimum_required (VERSION 2.6)

project(Uefi_main)

# set versions
set (Uefi_main_VERSION_MAJOR 0)
set (Uefi_main_VERSION_MINOR 0)

enable_language(C ASM)

add_executable(uefi_main.efi uefi_main.c)

file(COPY startup.nsh DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY qemu-aarch64-uefi.sh DESTINATION ${CMAKE_BINARY_DIR}/)
