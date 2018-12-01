# toolchain
set(CROSS_PREFIX aarch64-elf)
set(CMAKE_C_COMPILER ${CROSS_PREFIX}-gcc)
set(CMAKE_AR ${CROSS_PREFIX}-ar CACHE FILEPATH "Arhiver")
set(CMAKE_RANLIB ${CROSS_PREFIX}-ranlib CACHE FILEPATH "Runlib")
set(CMAKE_AS ${CROSS_PREFIX}-as)
set(CMAKE_NM ${CROSS_PREFIX}-nm)
set(CMAKE_OBJCOPY ${CROSS_PREFIX}-objcopy)
set(CMAKE_OBJDUMP ${CROSS_PREFIX}-objdump)
set(CMAKE_LINKER ${CROSS_PREFIX}-ld)

# to pass compiler test
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(triple aarch64-none-elf)
set(CMAKE_C_COMPILER_TARGET ${triple})


# sysroot
set(CMAKE_SYSROOT $ENV{HOME}/opt/llvm/sysroot-aarch64-elf)
set(CMAKE_INCLUDE_PATH ${CMAKE_SYSROOT}/usr/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_SYSROOT}/usr/lib)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SYSROOT}/usr)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT}/usr)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# find gnu-efi libraries
include_directories(${CMAKE_SYSROOT}/usr/include/efi)
include_directories(${CMAKE_SYSROOT}/usr/include/efi/${CMAKE_SYSTEM_PROCESSOR})

set(uefi-crt "${CMAKE_SYSROOT}/usr/lib/crt0-efi-${CMAKE_SYSTEM_PROCESSOR}.o")
#message(${uefi-crt0})

# flags
# C
set(c_flags_aarch64_uefi "-fno-stack-protector -fshort-wchar")
set(c_flags "-std=c11 -Wall -Wextra -Wpedantic -nostdlib -fno-builtin")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${c_flags_aarch64_uefi}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${c_flags}")


# linker
set(linkerscript "${CMAKE_SYSROOT}/usr/lib/elf_${CMAKE_SYSTEM_PROCESSOR}_efi.lds")
set(sharedlib_ld_flags "-nostdlib ")
set(sharedlib_ld_flags "${sharedlib_ld_flags} --build-id=sha1")
set(sharedlib_ld_flags "${sharedlib_ld_flags} -shared")
set(sharedlib_ld_flags "${sharedlib_ld_flags} -Bsymbolic")
set(sharedlib_ld_flags "${sharedlib_ld_flags} --defsym=EFI_SUBSYSTEM=0x0a")
set(sharedlib_ld_flags "${sharedlib_ld_flags} -L${CMAKE_SYSROOT}/usr/lib/")
set(sharedlib_ld_flags "${sharedlib_ld_flags} -T ${linkerscript}")
set(uefi_libraries "-lefi -lgnuefi -lgcc")

# http://lists.llvm.org/pipermail/llvm-dev/2017-April/112534.html
set(CMAKE_C_CREATE_SHARED_LIBRARY "${CMAKE_LINKER} ${sharedlib_ld_flags} ${uefi-crt} <OBJECTS> ${uefi_libraries} <LINK_LIBRARIES> -o <TARGET>")
