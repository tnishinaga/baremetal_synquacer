cmake_minimum_required(VERSION 3.12)

project(kernel)

# set versions
set (kernel_VERSION_MAJOR 0)
set (kernel_VERSION_MINOR 0)

enable_language(C ASM)

set(TARGET_NAME kernel)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ldscript.lds)
set(KERNEL_ENTRYADDR 0x1000)

# executable
add_executable(${TARGET_NAME} start.S early_init.c syscalls.c drivers/arm_pl011.c)
#include
message(${TARGET_NAME})
message(${CMAKE_CURRENT_SOURCE_DIR}/drivers/)
# target_include_directories(${TARGET_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/drivers/")

# linker script
# set_target_properties(${TARGET_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${LINKER_SCRIPT}")

# add libc
find_library(libc NAMES libc.a)
find_library(libgcc NAMES libgcc.a)

target_link_libraries(${TARGET_NAME} ${libc})
target_link_libraries(${TARGET_NAME} ${libgcc})

# select target
add_definitions(-DTARGET_QEMU)
add_definitions(-DKERNEL_ENTRYADDR=${KERNEL_ENTRYADDR})

# translate ELF to binary
add_custom_command(
   TARGET ${TARGET_NAME}
   POST_BUILD
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   DEPENDS ${TARGET_NAME}
   COMMAND llvm-objcopy -O binary ${TARGET_NAME} ${TARGET_NAME}.bin
)
