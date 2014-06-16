/*
2014 Jasper ter Weeme
*/

#ifndef _UART_H_
#define _UART_H_
#include <stdint.h>

class Terminal
{
public:
    virtual void puts(const char *) = 0;
};

class JtagUart : public Terminal
{
    static JtagUart *instance;
    volatile uint32_t * const base;
    volatile uint32_t * const ctl;
public:
    JtagUart(volatile uint32_t * const base) : base(base), ctl(base + 4) { instance = this; }
    static JtagUart *getInstance() { return instance; }
    void putc(const char);
    void puts(const char *s) { while (*s) putc(*s++); }
};

class Uart : public Terminal
{
    static Uart *instance;
    volatile uint32_t * const base;
public:
    Uart(volatile uint32_t * const base) : base(base) { instance = this; }
    static void isr(void *context) { }
    static Uart *getInstance() { return instance; }
    void putc(const char);
    void puts(const char *s) { while (*s) putc(*s++); }
};

#endif


