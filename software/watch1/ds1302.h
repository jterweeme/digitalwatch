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
public:
    TimeStamp(ds1302_struct);
    const char *toString();
    uint8_t getHour10();
    uint8_t getHour();
    uint8_t getMinutes10();
    uint8_t getMinutes();
private:
    ds1302_struct ds;
};

class RTC
{
public:
    RTC() {};
    virtual void update() = 0;
    virtual TimeStamp *getTimeStamp() = 0;
};

class FallBackRTC : public RTC
{
public:
    static FallBackRTC *getInstance();
    void update();
    TimeStamp *getTimeStamp();
private:
    FallBackRTC();
    ds1302_struct rtc;
};

class DS1302 : public RTC
{
public:
    static DS1302 *getInstance();
    void update();
    TimeStamp *getTimeStamp();
private:
    void write(int, uint8_t);
    DS1302();
    void init();
    volatile bool *io_handle;
    volatile bool *reset_handle;
    volatile bool *clk_handle;
    static const uint8_t CLOCK_BURST_READ = 0xbf;
    static const uint8_t ENABLE = 0x8e;
    static const uint8_t TRICKLE = 0x90;
    static const uint8_t READBIT = 0;
    void stop();
    void start();
    int digitalLees(int n);
    uint8_t toggleRead();
    void toggleWrite(uint8_t data, uint8_t release);
    int bcd2bin(int hi, int lo);
    ds1302_struct rtc;
};

class RTCFactory
{
public:
    RTCFactory() {};
    RTC *createRTC();
};
#endif


