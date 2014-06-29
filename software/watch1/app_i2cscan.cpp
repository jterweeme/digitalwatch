#include <system.h>
#include "misc.h"

class App
{
    I2CBus bus;
    Uart uart;
public:
    App();
    int run();
};

App::App()
  :
    bus((void *)I2CBUS_0_BASE),
    uart((uint32_t *)UART_0_BASE)
{
    uart.puts("I2C Scanner...\r\n");
}

int App::run()
{
    bus.scan();

    for (mstd::vector<uint8_t>::iterator it = bus.slaves.begin(); it < bus.slaves.end(); it++)
        uart.printf("0x%x\r\n", *it);

    return 0;
}

int main()
{
    App app;
    return app.run();
}


