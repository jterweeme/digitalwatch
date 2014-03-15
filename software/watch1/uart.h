#ifndef _UART_H_
#define _UART_H_
#include <stdint.h>

class Terminal
{
public:
    virtual void puts(const char *) = 0;
};

// JTAG is Singleton en Observable
class JtagUart : public Terminal
{
public:
    static JtagUart *getInstance();
    void init();
    void puts(const char *);
    void putc(const char);
private:
    JtagUart();
    volatile uint32_t *handle;
};

// Uart is Singleton en Observable
class Uart : public Terminal
{
public:
    static Uart *getInstance();
    void putc(const char);
    void puts(const char *);
private:
    Uart();
    static void isr(void *context);
    volatile uint32_t *uart;
};
#endif


