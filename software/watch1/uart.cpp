/*
2014 Jasper ter Weeme
*/

#include "uart.h"
#include <stdint.h>

void JtagUart::putc(const char c)
{
    while (*ctl & 0xffff0000 == 0) { }
    base[0] = c;
}

Uart *Uart::instance;
JtagUart *JtagUart::instance;

void Uart::putc(const char c)
{
    while ((base[2] & (1<<6)) == 0) { }
    base[1] = c;
}



