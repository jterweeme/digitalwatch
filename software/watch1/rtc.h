/*
2014 Jasper ter Weeme
*/

#ifndef _DS1302_H_
#define _DS1302_H_

#include <stdint.h>

struct ds1302_struct
{
  uint8_t Seconds:4;
  uint8_t Seconds10:3;
  uint8_t CH:1;
  uint8_t Minutes:4;
  uint8_t Minutes10:3;
  uint8_t reserved1:1;
  union
  {
    struct
    {
      uint8_t Hour:4;
      uint8_t Hour10:2;
      uint8_t reserved2:1;
      uint8_t hour_12_24:1;
    } h24;
    struct
    {
      uint8_t Hour:4;
      uint8_t Hour10:1;
      uint8_t AM_PM:1;
      uint8_t reserved2:1;
      uint8_t hour_12_24:1;
    } h12;
  };
  uint8_t Date:4;
  uint8_t Date10:2;
  uint8_t reserved3:2;
  uint8_t Month:4;
  uint8_t Month10:1;
  uint8_t reserved4:3;
  uint8_t Day:3;
  uint8_t reserved5:5;
  uint8_t Year:4;
  uint8_t Year10:4;
  uint8_t reserved6:7;
  uint8_t WP:1;
};

class TimeStamp
{
private:
    ds1302_struct ds;
public:
    TimeStamp(ds1302_struct ds) { this->ds = ds; }
    ds1302_struct *getDS() { return &ds; }
    const char *toString();
    uint8_t getHour10() { return ds.h24.Hour10; }
    uint8_t getHour() { return ds.h24.Hour; }
    uint8_t getMinutes10() { return ds.Minutes10; }
    uint8_t getMinutes() { return ds.Minutes; }
};

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
private:
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
private:
    void write(int, uint8_t);
    void init();
    DS1302() { init(); }
    volatile bool *io_handle;
    volatile bool *reset_handle;
    volatile bool *clk_handle;
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
    void incrementMinutes();
    void incrementHours();
    void update();
};

class RTCFactory
{
public:
    static RTC *createRTC();    // kan ook in statische context aangeroepen worden
};
#endif


