cmake_minimum_required(VERSION 3.12)

project(uefi_loader)

# set versions
set (uefi_loader_VERSION_MAJOR 0)
set (uefi_loader_VERSION_MINOR 0)

enable_language(C ASM)

if (CMAKE_CROSSCOMPILING AND (CMAKE_SYSTEM_NAME STREQUAL "Generic"))
    # Can not build shared library when CMAKE_SYSTEM_NAME is "Generic"
    set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
endif ()

set(TARGET_NAME uefi_loader)

add_library(${TARGET_NAME} SHARED uefi_loader.c)

add_definitions(-DTARGET_QEMU)


# translate ELF to PE
add_custom_command(
   TARGET ${TARGET_NAME}
   POST_BUILD
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   DEPENDS ${TARGET_NAME}
   COMMAND llvm-objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .dynstr -j .rel -j .rel.dyn -j .rela -j .rela.dyn -j .reloc -O binary lib${TARGET_NAME}.so ${TARGET_NAME}.efi
)
