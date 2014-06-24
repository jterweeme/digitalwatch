/*
2014 Jasper ter Weeme
*/

#ifndef _MISC_H_
#define _MISC_H_
#include <stdint.h>

class Terminal
{
public:
    virtual void puts(const char *) = 0;
};

class JtagUart : public Terminal
{
    static JtagUart *instance;
    volatile uint32_t * const base;
    volatile uint32_t * const ctl;
public:
    JtagUart(volatile uint32_t * const base) : base(base), ctl(base + 4) { instance = this; }
    static JtagUart *getInstance() { return instance; }
    void putc(const char);
    void puts(const char *s) { while (*s) putc(*s++); }
};

class Uart : public Terminal
{
    static Uart *instance;
    volatile uint32_t * const base;
public:
    Uart(volatile uint32_t * const base) : base(base) { instance = this; }
    static void isr(void *context) { }
    static Uart *getInstance() { return instance; }
    void putc(const char);
    void puts(const char *s) { while (*s) putc(*s++); }
};

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
    ds1302_struct ds;
public:
    TimeStamp(ds1302_struct ds) : ds(ds) { }
    ds1302_struct *getDS() { return &ds; }
    const char *toString();
    uint8_t getHour10() { return ds.h24.Hour10; }
    uint8_t getHour() { return ds.h24.Hour; }
    uint8_t getMinutes10() { return ds.Minutes10; }
    uint8_t getMinutes() { return ds.Minutes; }
};

class Observer
{
public:
    virtual void update() = 0;
};

class Timer
{
    Observer *rh;
    volatile void * const base;
    volatile uint32_t * const base32;
    void update() { rh->update(); base32[0] = 0; }
    static Timer *instance;
public:
    Timer(volatile void * const base, const unsigned ctl, const unsigned irq);
    static Timer *getInstance() { return instance; }
    void setObserver(Observer *obs) { rh = obs; }
private:
    static void isr(void *context) { getInstance()->update(); }
};

class Buttons
{
    static Buttons *instance;
    volatile void * const base;
    volatile uint32_t * const base32;
    void update();
    static void isr(void *context) { getInstance()->update(); }
    Observer *s4;
    Observer *s5;
    Observer *s6;
    static const uint8_t BUTTON_S4 = 3;
    static const uint8_t BUTTON_S5 = 5;
public:
    static Buttons *getInstance() { return instance; }
    void setObserver(Observer *, int);
    Buttons(volatile void * const base, const unsigned ctl, const unsigned irq);
};

class SegDisplay
{
    volatile uint32_t * const handle;
    volatile uint8_t * const blinkMask;
public:
    void write(const uint32_t data);
    void setBlinkMask(const uint8_t mask) { *blinkMask = mask; }
    SegDisplay() : handle(0), blinkMask(0) { }

    SegDisplay(volatile uint32_t * const base) :
        handle(base),
        blinkMask((volatile uint8_t * const)base + 8)
    { }
};

class TimeDisplay : public SegDisplay
{
    static uint8_t lookup[];
public:
    TimeDisplay() : SegDisplay() { }
    TimeDisplay(volatile uint32_t * const addr) : SegDisplay(addr) { }
    void setMinutes(const uint8_t min) { write(lookup[0] | (lookup[0] << 8)); }
    void setTime(uint8_t, uint8_t);
    void setTime(TimeStamp ts);
};

class RTC
{
public:
    virtual void update() = 0;
    virtual TimeStamp getTimeStamp() = 0;
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
    TimeStamp getTimeStamp() { return TimeStamp(rtc); }
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
    TimeStamp getTimeStamp() { return TimeStamp(rtc); }
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

