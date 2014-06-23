/*
2014 Jasper ter Weeme
*/

#ifndef _DS1302_H_
#define _DS1302_H_

#include <stdint.h>
#include "misc.h"

class RTC
{
public:
    virtual void update() = 0;
    virtual TimeStamp *getTimeStamp() = 0;
    virtual void incrementHours() {}
    virtual void incrementMinutes() {}
};

class FallBackRTC : public RTC
{
    ds1302_struct rtc;
public:
    static FallBackRTC *getInstance();
    void update();
    void incrementMinutes();
    void incrementHours();
    TimeStamp *getTimeStamp() { return new TimeStamp(rtc); }
};

class DS1302 : public RTC
{
    void write(int, uint8_t);
    volatile void *base;
    volatile uint32_t *io_handle;
    volatile uint32_t *reset_handle;
    volatile uint32_t *clk_handle;
    static const uint8_t SECONDS = 0x80;
    static const uint8_t MINUTES = 0x82;
    static const uint8_t HOURS = 0x84;
    static const uint8_t DATE = 0x86;
    static const uint8_t ENABLE = 0x8e;
    static const uint8_t TRICKLE = 0x90;
    static const uint8_t CLOCK_BURST_WRITE = 0xbe;
    static const uint8_t CLOCK_BURST_READ = 0xbf;
    void stop();
    void start();
    int digitalLees(int n) { return 0; }
    uint8_t toggleRead();
    void toggleWrite(uint8_t data, uint8_t release);
    ds1302_struct rtc;
    void burstWrite(uint8_t *);
public:
    TimeStamp *getTimeStamp() { return new TimeStamp(rtc); }
    static DS1302 *getInstance();
    void init(volatile uint32_t *io, volatile uint32_t *clk, volatile uint32_t *rst);
    void incrementMinutes();
    void incrementHours();
    void update();
};

class RTCFactory
{
    volatile uint32_t * const ds1302_clk;
    volatile uint32_t * const ds1302_io;
    volatile uint32_t * const ds1302_rst;
public:
    RTCFactory(volatile uint32_t * const ds1302_clk,
            volatile uint32_t * const ds1302_io,
            volatile uint32_t * const ds1302_rst);

    RTC *createRTC();
};
#endif


