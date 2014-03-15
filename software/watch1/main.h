#ifndef _MAIN_H_
#define _MAIN_H_

#include "uart.h"
#include "timer.h"
#include "segment.h"
#include "buttons.h"

class Watch;

class Leds
{
public:
    Leds();
    void write(uint8_t);
private:
    volatile uint8_t *handle;
};

class Alarm
{
public:
    Alarm();
};

class AbstractMode
{
public:
    AbstractMode();
    virtual void increase() = 0;
    virtual void timerTick() {};
};

class DisplayTimeMode : public AbstractMode
{
public:
    DisplayTimeMode();
    DisplayTimeMode(Watch *);
    void timerTick();
    void increase();
private:
    Watch *watch;
};

class IncrementHoursMode : public AbstractMode
{
public:
    IncrementHoursMode();
    void increase();
};

class IncrementMinutesMode : public AbstractMode
{
public:
    IncrementMinutesMode();
    void increase();
};

class JtagUart
{
public:
    JtagUart();
    void init();
    void puts(const char *);
    void putc(const char);
    volatile uint32_t *handle;
};

class Watch
{
public:
    static Watch *getInstance();
    TimeDisplay *getTimeDisplay();
    Leds *getLeds();
    RTC *getRTC();
    Uart *getUart();
    void nextMode();
    void increase();
    void timerTick();
private:
    Watch();
    void init();
    Uart *uart;
    JtagUart *jtagUart;
    Leds *leds;
    Buttons *buttons;
    TimeDisplay *segDisplay;
    Timer *timer;
    RTC *rtc;
    static const uint8_t DISPLAY_TIME_MODE = 1;
    static const uint8_t INCREMENT_HOURS_MODE = 2;
    static const uint8_t INCREMENT_MINUTES_MODE = 3;
    uint8_t mode;
    AbstractMode *mode2;
};

#endif

