#ifndef _UART_H_
#define _UART_H_
#include <stdint.h>

// Uart is Singleton en Observable
class Uart
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


