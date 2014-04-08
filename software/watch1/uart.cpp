/*
2014 Jasper ter Weeme
*/

#include "uart.h"
#include <stdint.h>
#include <system.h>

JtagUart::JtagUart()
{
    init();
}

/*
Quick and dirty implementatie van Singleton pattern
*/
JtagUart *JtagUart::getInstance()
{
    static JtagUart instance;
    return &instance;
}

void JtagUart::init()
{
    handle = (volatile uint32_t *)JTAG_UART_0_BASE;
}

void JtagUart::putc(const char c)
{
    while ((handle[2] & (1<<6)) == 0) {
    }

    handle[1] = c;
}

void JtagUart::puts(const char *s)
{
    while (*s)
    {
        putc(*s++);
    }
}

/*
Quick and dirty implementatie van Singleton pattern
*/
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
    {
        putc(*s++);
    }
}



