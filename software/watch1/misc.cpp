/*
2014 Jasper ter Weeme
*/

#include "misc.h"
#include "mystl.h"
#include <unistd.h>
#include <sys/alt_irq.h>

void DS1302::update()
{
    start();
    toggleWrite(CLOCK_BURST_READ, true);
    uint8_t *p = (uint8_t *)&rtc;

    for (int i = 0; i < 8; i++)
        *p++ = toggleRead();

    stop();
}

void DS1302::write(int address, uint8_t data)
{
    address &= ~(1<<0);
    start();
    toggleWrite(address, false);
    toggleWrite(data, false);
    stop();
}

const char *TimeStamp::toString()
{
    static char buffer[80];
    Utility::strcpy(buffer, "Time = xx:xx:xx\r\n");
    buffer[7]  = ds.h24.Hour10 + '0';
    buffer[8]  = ds.h24.Hour   + '0';
    buffer[10] = ds.Minutes10  + '0';
    buffer[11] = ds.Minutes    + '0';
    buffer[13] = ds.Seconds10  + '0';
    buffer[14] = ds.Seconds    + '0';
    return buffer;
}

Timer::Timer(volatile void * const base, const unsigned ctl, const unsigned irq)
  :
    base(base),
    base32((uint32_t *)base)
{
    instance = this;
    alt_ic_isr_register(ctl, irq, isr, 0, 0);
}

void JtagUart::putc(const char c)
{
    while (*ctl & 0xffff0000 == 0) { }
    base[0] = c;
}

Uart *Uart::instance;
JtagUart *JtagUart::instance;
Timer *Timer::instance;
Buttons *Buttons::instance;

void Uart::putc(const char c)
{
    while ((base[2] & (1<<6)) == 0) { }
    base[1] = c;
}

Buttons::Buttons(volatile void * const base, const unsigned ctl, const unsigned irq)
  :
    base(base),
    base32((uint32_t *)base)
{
    instance = this;
    base32[2] = 0xf;
#ifdef BUTTONS_IRQ
    alt_ic_isr_register(ctl, irq, isr, 0, 0);
#endif
}

void Buttons::update()
{
    switch (base32[0])
    {
    case BUTTON_S4:
        if (s4)
            s4->update();

        return;
    case BUTTON_S5:
        if (s5)
            s5->update();

        return;
    }

    base32[3] = 0;
}

void Buttons::setObserver(Observer *obs, int n)
{
    switch (n)
    {
    case 4:
        s4 = obs;
        return;
    case 5:
        s5 = obs;
        return;
    case 6:
        s6 = obs;
        return;
    }
}

void DS1302::stop()
{
    *reset_handle = 0;
    ::usleep(4);
}

uint8_t DS1302::toggleRead()
{
    uint8_t data = 0;

    for (int i = 0; i <= 7; i++)
    {
        *clk_handle = 1;
        ::usleep(1);
        *clk_handle = 0;
        ::usleep(1);
        *io_handle ? data |= (1<<i) : data &= ~(1<<i);
    }

    return data;
}

void DS1302::burstWrite(uint8_t *p)
{
    start();
    toggleWrite(CLOCK_BURST_WRITE, false);
    
    for (int i = 0; i < 8; i++)
        toggleWrite(*p++, false);

    stop();
}

I2CBus::I2CBus(volatile void * const base)
  :
    base(base),
    sda((uint8_t *)((uint8_t *)base + 0)),
    sda_dir((uint8_t *)((uint8_t *)base + 1)),
    scl((uint8_t *)((uint8_t *)base + 2))
{
}

DS1302::DS1302(volatile void * const io)
  :
    base(io),
    io_handle((uint8_t *)io),
    io_direction((uint8_t *)((uint8_t *)io + 1)),
    clk_handle((uint8_t *)((uint8_t *)io + 2)),
    reset_handle((uint8_t *)((uint8_t *)io + 3))
{
    write(ENABLE, 0);
    write(TRICKLE, 0);
}

void DS1302::incrementHours()
{
    if (rtc.h24.Hour10 >= 2 && rtc.h24.Hour >= 3)
    {
        rtc.h24.Hour10 = 0;
        rtc.h24.Hour = 0;
    }
    else if (rtc.h24.Hour++ >= 9)
    {
        rtc.h24.Hour = 0;
        rtc.h24.Hour10++;
    }

    burstWrite((uint8_t *)&rtc);
}

void DS1302::incrementMinutes()
{
    if (++rtc.Minutes > 9)
    {
        rtc.Minutes = 0;

        if (++rtc.Minutes10 > 5)
            rtc.Minutes10 = 0;
    }

    burstWrite((uint8_t *)&rtc);
}

void DS1302::toggleWrite(uint8_t data, uint8_t release)
{
    for (int i = 0; i <= 7; i++)
    {
        *io_handle = ((data >> i) & 1);
        ::usleep(1);
        *clk_handle = 1;
        ::usleep(1);

        if (release && i == 7)
        {
            *io_direction = INPUT;
        }
        else
        {
            *clk_handle = 0;
            ::usleep(1);
        }
    }
}

void PCF8563::update()
{
    i2cBus->start();
    i2cBus->stop();
}

void DS1302::start()
{
    *reset_handle = 0;
    *clk_handle = 0;
    *io_direction = OUTPUT;
    *reset_handle = 1;
    ::usleep(4);
}

FallBackRTC *FallBackRTC::getInstance()
{
    Uart::getInstance()->puts("Get FallBackRTC instance\r\n");
    static FallBackRTC instance;
    return &instance;
}

void FallBackRTC::incrementMinutes()
{
    if (++rtc.Minutes > 9)
    {
        rtc.Minutes = 0;

        if (++rtc.Minutes10 > 5)
            rtc.Minutes10 = 0;
    }
}

void FallBackRTC::incrementHours()
{
    if (rtc.h24.Hour10 >= 2 && rtc.h24.Hour >= 3)
    {
        rtc.h24.Hour10 = 0;
        rtc.h24.Hour = 0;
    }
    else if (rtc.h24.Hour++ >= 9)
    {
        rtc.h24.Hour = 0;
        rtc.h24.Hour10++;
    }
}

void I2CBus::start()
{
    *sda_dir = OUTPUT;
    *sda = 1;
    *scl = 1;
    ::usleep(1);
    *sda = 0;
    ::usleep(1);
    *scl = 0;
    ::usleep(1);
}

void I2CBus::stop()
{
    *sda_dir = OUTPUT;
    *sda = 0;
    *scl = 1;
    ::usleep(1);
    *sda = 1;
    ::usleep(1);
}

uint8_t SegDisplayEx::lookup[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

void TimeDisplay::setTime(const uint8_t uur, const uint8_t min)
{
    const uint8_t d = lookup[uur / 10];
    uint8_t c = lookup[uur % 10];
    c &= ~DOT;
    const uint8_t b = lookup[min / 10];
    const uint8_t a = lookup[min % 10];
    write(a | b << 8 | c << 16 | d << 24);
}

void TimeDisplay::setTime(TimeStamp ts)
{
    const uint8_t d = lookup[ts.getHour10()];
    uint8_t c = lookup[ts.getHour()];
    c &= ~DOT;
    const uint8_t b = lookup[ts.getMinutes10()];
    const uint8_t a = lookup[ts.getMinutes()];
    write(a | b << 8 | c << 16 | d << 24);
}

void SegDisplay::write(const uint32_t data)
{
    *handle = data;
    handle[1] = 0xffffffff;
}

void FallBackRTC::update()
{
    if (rtc.Seconds++ >= 9)
    {   rtc.Seconds = 0;
        rtc.Seconds10++;
    }   else return;

    if (rtc.Seconds10 > 5)
    {   rtc.Seconds10 = 0;
        rtc.Minutes++;
    }   else return;

    if (rtc.Minutes > 9)
    {   rtc.Minutes = 0;
        rtc.Minutes10++;
    }   else return;

    if (rtc.Minutes10 > 5)
    {   rtc.Minutes10 = 0;;
        rtc.h24.Hour++;
    }   else return;

    if (rtc.h24.Hour > 9)
    {   rtc.h24.Hour = 0;;
        rtc.h24.Hour10++;
    }   else return;

    if (rtc.h24.Hour10 == 2 && rtc.h24.Hour > 3)
        rtc.h24.Hour10 = rtc.h24.Hour = 0;

}

