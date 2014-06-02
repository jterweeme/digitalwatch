/*
2014 Jasper ter Weeme
*/

#ifndef _UART_H_
#define _UART_H_
#include <stdint.h>
#include <system.h>     // moet nog weg

class Terminal
{
public:
    virtual void puts(const char *) = 0;
};

// JTAG is Singleton en Observable
class JtagUart : public Terminal
{
private:
    volatile uint32_t *handle;
public:
    static JtagUart *getInstance();
    void init() { handle = (volatile uint32_t *)JTAG_UART_0_BASE; }
    void putc(const char);
    void puts(const char *s) { while (*s) putc(*s++); }
private:
     JtagUart() { init(); }
};

// Uart is Singleton en Observable
class Uart : public Terminal
{
public:
    static Uart *getInstance();
    void putc(const char);
    void puts(const char *s) { while (*s) putc(*s++); }
private:
    volatile uint32_t *uart;
    Uart() { uart = (volatile uint32_t *)UART_0_BASE; }
    static void isr(void *context) { }

};
#endif


