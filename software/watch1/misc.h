/*
2014 Jasper ter Weeme
*/

#ifndef _MISC_H_
#define _MISC_H_
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

#endif

