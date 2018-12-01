#pragma once

#include <stdint.h>

#ifdef TARGET_QEMU
    #include "drivers/arm_pl011.h"

    // memmap
    #define UART ((PL011_Type *)0x09000000)
    #define GPIO ((PL061_Type *)0x09030000)

#elif TARGET_SYNQUACER
    #include "drivers/arm_pl011.h"

    // memmap
    #define UART0 ((PL011_Type *)0x2a400000)
#endif

