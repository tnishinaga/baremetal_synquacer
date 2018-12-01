#include <stdint.h>

#include "memmap.h"
#include "config.h"

void easy_fputs(PL011_Type *stdio, char *s)
{
    // MEMO: this function is not secure
    while (*s != '\0') {
        pl011_putc(*s, stdio);
        s++;
    }
}


void early_init(void)
{
    PL011_Type *stdio = STDIO;
    pl011_init(stdio, UART_CLOCK);

    easy_fputs(stdio, "Hello BareMetal World!\n");
    printf("Hello World!\n");

    // infinite loop
    while (1) {
        __asm volatile ("wfi");
    }
}
