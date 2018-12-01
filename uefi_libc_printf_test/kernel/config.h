#pragma once

#include "memmap.h"

#ifdef TARGET_QEMU
    #define UART_CLOCK (24000000 * 3UL) // QEMU set apb-pclk to 24MHz(x3 PLL?)
    #define STDIO UART
#elif TARGET_SYNQUACER
    #define UART_CLOCK (62500000UL)
    #define STDIO UART0
#endif
