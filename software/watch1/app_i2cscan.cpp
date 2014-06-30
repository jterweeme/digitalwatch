#include <system.h>
#include "misc.h"

class App
{
    I2CBus rtcBus;
    I2CBus eepBus;
    Uart uart;
public:
    App();
    int run();
};

App::App()
  :
    rtcBus((void *)I2CBUS_0_BASE),
    eepBus((void *)EEPROM_BASE),
    uart((uint32_t *)UART_0_BASE)
{
    uart.puts("I2C Scanner...\r\n");
}

int App::run()
{
    rtcBus.scan();
    eepBus.scan();

    for (mstd::vector<uint8_t>::iterator it = rtcBus.slaves.begin();
            it < rtcBus.slaves.end(); it++)
    {
        uart.printf("0x%x\r\n", *it);
    }

    for (mstd::vector<uint8_t>::iterator it = eepBus.slaves.begin();
            it < eepBus.slaves.end(); it++)
    {
        uart.printf("0x%x\r\n", *it);
    }

    return 0;
}

int main()
{
    App app;
    return app.run();
}


