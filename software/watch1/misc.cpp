/*
2014 Jasper ter Weeme
*/

#include "misc.h"
#include <stdio.h>
#include <unistd.h>

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

    ::sprintf(buffer, "Time = %c%c:%c%c:%c%c\r\n", ds.h24.Hour10 + 48,
        ds.h24.Hour + 48, ds.Minutes10 + 48, ds.Minutes + 48,
        ds.Seconds10 + 48, ds.Seconds + 48);

    return buffer;
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
    sda((uint8_t *)((uint8_t *)base + 1)),
    scl((uint8_t *)((uint8_t *)base + 1))
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
            *io_direction = 0;
        }
        else
        {
            *clk_handle = 0;
            ::usleep(1);
        }
    }
}

RTC *RTCFactory::createRTC()
{
    Uart::getInstance()->puts("RTC Factory\r\n");
    static DS1302 test(ds1302_base);
    test.update();
    TimeStamp testStamp = test.getTimeStamp();
    Uart::getInstance()->puts(testStamp.toString());

    if (testStamp.getHour10() > 2)
        return FallBackRTC::getInstance();

    return &test;
}

void DS1302::start()
{
    *reset_handle = 0;
    *clk_handle = 0;
    *io_direction = 1;
    *reset_handle = 1;
    ::usleep(4);
}

RTCFactory::RTCFactory(volatile void * const ds1302_base)
  :
    ds1302_base(ds1302_base)
{
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

