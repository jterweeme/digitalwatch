/*
2014 Jasper ter Weeme
*/

#include "uart.h"
#include <stdint.h>
#include <system.h>     // moet nog weg

/*
Quick and dirty implementatie van Singleton pattern
*/
JtagUart *JtagUart::getInstance()
{
    static JtagUart instance;
    return &instance;
}

void JtagUart::putc(const char c)
{
    while ((handle[2] & (1<<6)) == 0) {
    }

    handle[1] = c;
}

/*
Quick and dirty implementatie van Singleton pattern
*/

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



