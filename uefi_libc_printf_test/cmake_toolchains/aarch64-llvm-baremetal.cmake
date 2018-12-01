# toolchain
set(CMAKE_C_COMPILER clang)
set(CMAKE_AR llvm-ar CACHE FILEPATH "Arhiver")
set(CMAKE_RANLIB llvm-ranlib CACHE FILEPATH "Runlib")
set(CMAKE_AS llvm-as)
set(CMAKE_NM llvm-nm)
set(CMAKE_OBJCOPY llvm-objcopy)
set(CMAKE_OBJDUMP llvm-objdump)
set(CMAKE_LINKER ld.lld)

# to pass compiler test
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(triple aarch64-none-elf)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_ASM_COMPILER_TARGET ${triple})


# sysroot
set(CMAKE_SYSROOT $ENV{HOME}/opt/llvm/sysroot-aarch64-elf)
set(CMAKE_INCLUDE_PATH ${CMAKE_SYSROOT}/usr/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_SYSROOT}/usr/lib)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SYSROOT}/usr)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT}/usr)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# flags
# C
set(c_flags_aarch64_uefi "-fno-stack-protector -fshort-wchar")
set(c_flags "-std=c11 -Wall -Wextra -Wpedantic -fno-builtin -ffreestanding")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${c_flags_aarch64_uefi}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${c_flags}")


# linker
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -nostdlib")

# http://lists.llvm.org/pipermail/llvm-dev/2017-April/112534.html
set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_LINKER} <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
