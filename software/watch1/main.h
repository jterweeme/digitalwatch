/*
2014 Jasper ter Weeme
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#include "uart.h"
#include "timer.h"
#include "segment.h"
#include "buttons.h"
#include "rtc.h"

class Leds
{
public:
    Leds();
    void write(uint8_t);
private:
    volatile uint8_t *handle;
};

/* State Pattern */
class AbstractMode
{
public:
    virtual void increase() {};
    virtual void timerTick() {};
};

class Watch
{
public:
    TimeDisplay *getTimeDisplay();
    Leds *getLeds();
    RTC *getRTC();
    Uart *getUart();
    Terminal *getDebugger();
    void nextMode();
    void increment();
    void timerTick();
    void init();
private:
    Uart *uart;
    Leds *leds;
    Buttons *buttons;
    TimeDisplay *segDisplay;
    Timer *timer;
    RTC *rtc;
    Terminal *debugger;
    static const uint8_t DISPLAY_TIME_MODE = 1;
    static const uint8_t INCREMENT_HOURS_MODE = 2;
    static const uint8_t INCREMENT_MINUTES_MODE = 3;
    uint8_t mode;
    AbstractMode *mode2;
};

#endif

