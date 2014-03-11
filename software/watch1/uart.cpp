#include "uart.h"
#include <stdint.h>
#include <system.h>

Uart::Uart()
{
    uart = (volatile uint32_t *)UART_0_BASE;
}

Uart *Uart::getInstance()
{
    static Uart instance;
    return &instance;
}

void Uart::putc(const char c)
{
    while ((uart[2] & (1<<6)) == 0) {
    }

    uart[1] = c;
}

void Uart::isr(void *context)
{
}

void Uart::puts(const char *s)
{
    while (*s)
        putc(*s++);
}



