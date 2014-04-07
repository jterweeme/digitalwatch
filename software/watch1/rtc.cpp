/*
2014 Jasper ter Weeme
*/

#define SYSTEM_BUS_WIDTH 32

#include "rtc.h"
#include "uart.h"
#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include <io.h>

DS1302::DS1302()
{
    init();
}

FallBackRTC::FallBackRTC()
{
    init();
}

uint8_t TimeStamp::getHour10()
{
    return ds.h24.Hour10;
}

uint8_t TimeStamp::getHour()
{
    return ds.h24.Hour;
}

uint8_t TimeStamp::getMinutes10()
{
    return ds.Minutes10;
}

uint8_t TimeStamp::getMinutes()
{
    return ds.Minutes;
}

void DS1302::update()
{
    start();
    toggleWrite(DS1302::CLOCK_BURST_READ, true);
    uint8_t *p = (uint8_t *)&rtc;

    for (int i = 0; i < 8; i++)
    {
        *p++ = toggleRead();
    }

    stop();

}

void FallBackRTC::init()
{
    rtc.h24.Hour10 = 1;
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

DS1302 *DS1302::getInstance()
{
    static DS1302 instance;
    return &instance;
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
    {
        toggleWrite(*p++, false);
    }

    stop();
}

void DS1302::increaseHours()
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

void DS1302::increaseMinutes()
{
    if (++rtc.Minutes > 9)
    {
        rtc.Minutes = 0;

        if (++rtc.Minutes10 > 5)
        {
            rtc.Minutes10 = 0;
        }
    }

    burstWrite((uint8_t *)&rtc);
}

void DS1302::toggleWrite(uint8_t data, uint8_t release)
{
    for (int i = 0; i <= 7; i++)
    {
        *io_handle = (data >> i) & 1;
        ::usleep(1);
        *clk_handle = 1;
        ::usleep(1);

        if (release && i == 7)
        {
            IOWR(DS1302_IO_BASE, 1, 0);
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
    DS1302 *test = DS1302::getInstance();
    test->update();
    TimeStamp *testStamp = test->getTimeStamp();
    Uart::getInstance()->puts(testStamp->toString());

    if (testStamp->getHour10() > 2)
    {
        return FallBackRTC::getInstance();
    }

    return DS1302::getInstance();
}

void DS1302::init()
{
    io_handle = (volatile bool *)DS1302_IO_BASE;
    clk_handle = (volatile bool *)DS1302_CLK_BASE;
    reset_handle = (volatile bool *)DS1302_RESET_BASE;

    write(DS1302::ENABLE, 0);
    write(DS1302::TRICKLE, 0);
}

void DS1302::start()
{
    *reset_handle = 0;
    *clk_handle = 0;
    IOWR(DS1302_IO_BASE, 1, 1);
    *reset_handle = 1;
    ::usleep(4);
}

int DS1302::digitalLees(int n)
{
    return 0;
}

FallBackRTC *FallBackRTC::getInstance()
{
    Uart::getInstance()->puts("Get FallBackRTC instance\r\n");
    static FallBackRTC instance;
    return &instance;
}

void FallBackRTC::update()
{
    if (rtc.Seconds++ >= 9)
    {   rtc.Seconds = 0;
        rtc.Seconds10++;
    }   else return;

    if (rtc.Seconds10 > 9)
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
    {
        rtc.h24.Hour10 = rtc.h24.Hour = 0;
    }

}
