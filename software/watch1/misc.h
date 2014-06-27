/*
2014 Jasper ter Weeme
*/

#ifndef _MISC_H_
#define _MISC_H_
#include <stdint.h>

class Leds
{
    volatile uint8_t * const handle;
public:
    Leds() : handle(0) { }
    Leds(volatile uint8_t * const base) : handle(base) { }
    void write(const uint8_t data) { *handle = data; }
};

class Terminal
{
public:
    virtual void puts(const char *) = 0;
    virtual ~Terminal() { }
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
    virtual ~Observer() { }
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

class I2CBus
{
    volatile void * const base;
    volatile uint8_t * const sda;
    volatile uint8_t * const sda_dir;
    volatile uint8_t * const scl;
    static const uint8_t INPUT = 0;
    static const uint8_t OUTPUT = 1;
public:
    I2CBus(volatile void * const base);
    void start();
    void stop();
    bool private_write(uint8_t data);
    void private_read(uint8_t *data, bool ack);
    void write(uint8_t devAddr, uint8_t ctlAddr, uint8_t ctlData);
    uint8_t read(uint8_t devAddr, uint8_t ctlAddr);
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
    volatile uint8_t * const p_blinkMask;
public:
    void write(const uint32_t data);
    void blinkMask(const uint8_t mask) { *p_blinkMask = mask; }
    uint8_t blinkMask() { return *p_blinkMask; }
    SegDisplay() : handle(0), p_blinkMask(0) { }

    SegDisplay(volatile uint32_t * const base) :
        handle(base),
        p_blinkMask((volatile uint8_t * const)base + 8)
    { }
};

class SegDisplayEx : public SegDisplay
{
protected:
    static uint8_t lookup[];
public:
    SegDisplayEx() : SegDisplay() { }
    SegDisplayEx(volatile uint32_t * const addr) : SegDisplay(addr) { }
    void writeInt(int n) { write(lookup[n % 10]); }
};

class TimeDisplay : public SegDisplayEx
{
    static const uint8_t DOT = 0x80;
public:
    TimeDisplay() : SegDisplayEx() { }
    TimeDisplay(volatile uint32_t * const addr) : SegDisplayEx(addr) { }
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
    virtual ~RTC() { }
};

class PCF8563 : public RTC
{
    static const uint8_t ADDR = 0xa3;
    I2CBus * const i2cBus;
    ds1302_struct rtc;
public:
    PCF8563(I2CBus * const i2cBus) : i2cBus(i2cBus) { }
    TimeStamp getTimeStamp() { return TimeStamp(rtc); }
    void update();
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
    volatile void * const base;
    volatile uint8_t * const io_handle;
    volatile uint8_t * const io_direction;
    volatile uint8_t * const clk_handle;
    volatile uint8_t * const reset_handle;
    static const uint8_t INPUT = 0;
    static const uint8_t OUTPUT = 1;
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
    DS1302(volatile void * const io);
    TimeStamp getTimeStamp() { return TimeStamp(rtc); }
    void incrementMinutes();
    void incrementHours();
    void update();
};

#endif

