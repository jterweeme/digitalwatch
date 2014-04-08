/*
2014 Jasper ter Weeme

A simple digital watch displays the time of day. Setting of the time is
achieved by two buttons. Button #1 will switch from displaying time mode
to increment hours mode. In this mode the hours flash and pressing
button #2 will increment the hours each time it is pressed. Further
pressing of button #1 switches to increment minutes
mode. Button #2 functions in the same way as before. If button #1 is
pressed again the watch returns to display time mode. Time is not kept
whilst being set.
*/

#define SYSTEM_BUS_WIDTH 32

#include <system.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <io.h>
#include <sys/alt_irq.h>
#include "buttons.h"
#include "uart.h"
#include "timer.h"
#include "segment.h"
#include "rtc.h"

class Leds
{
public:
    Leds(volatile uint8_t * const);
    void write(uint8_t);
private:
    volatile uint8_t * handle;
};

/* State Pattern */
class AbstractMode
{
public:
    virtual void increase() {}
    virtual void timerTick() {}
};

/*
Hoofdklasse, nu niet meer Singleton
*/
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

class DisplayTimeMode : public AbstractMode
{
public:
    DisplayTimeMode(Watch *);
    void timerTick();
private:
    Watch *watch;
};

class IncrementHoursMode : public AbstractMode
{
public:
    IncrementHoursMode(Watch *);
    void increase();
private:
    Watch *watch;
};

class IncrementMinutesMode : public AbstractMode
{
public:
    IncrementMinutesMode(Watch *);
    void increase();
private:
    Watch *watch;
};

class TimerTick : public Observer
{
private:
    Watch *watch;
public:
    TimerTick(Watch *watch) { this->watch = watch; }
    void update();
};

class ButtonS4Action : public Observer
{
private:
    Watch *watch;
public:
    ButtonS4Action(Watch *);
    void update();
};

class ButtonS5Action : public Observer
{
private:
    Watch *watch;
public:
    ButtonS5Action(Watch *);
    void update();
};

ButtonS4Action::ButtonS4Action(Watch *watch)
{
    this->watch = watch;
}

ButtonS5Action::ButtonS5Action(Watch *watch)
{
    this->watch = watch;
}

Leds::Leds(volatile uint8_t * const base)
{
    handle = base;
}

void Leds::write(uint8_t data)
{
    *handle = data;
}

void ButtonS4Action::update()
{
    watch->nextMode();
}

void ButtonS5Action::update()
{
    watch->increment();
}

Leds *Watch::getLeds()
{
    return leds;
}

RTC *Watch::getRTC()
{
    return rtc;
}

DisplayTimeMode::DisplayTimeMode(Watch *watch)
{
    this->watch = watch;
    watch->getLeds()->write(~1);
    watch->getTimeDisplay()->setBlinkMask(0);
}

IncrementMinutesMode::IncrementMinutesMode(Watch *watch)
{
    this->watch = watch;
    watch->getLeds()->write(~4);
    watch->getTimeDisplay()->setBlinkMask(3);
}

IncrementHoursMode::IncrementHoursMode(Watch *watch)
{
    this->watch = watch;
    watch->getLeds()->write(~2);
    watch->getTimeDisplay()->setBlinkMask(0x0c);
}

TimeDisplay *Watch::getTimeDisplay()
{
    return segDisplay;
}

void TimerTick::update()
{
    watch->timerTick();
}

void IncrementHoursMode::increase()
{
    RTC *rtc = watch->getRTC();
    rtc->incrementHours();
    watch->getTimeDisplay()->setTime(rtc->getTimeStamp());
}

void IncrementMinutesMode::increase()
{
    RTC *rtc = watch->getRTC();
    rtc->incrementMinutes();
    watch->getTimeDisplay()->setTime(rtc->getTimeStamp());
}

Uart *Watch::getUart()
{
    return uart;
}

void DisplayTimeMode::timerTick()
{
    RTC *rtc = watch->getRTC();
    rtc->update();
    TimeStamp *ts = rtc->getTimeStamp();
    watch->getUart()->puts(ts->toString());
    watch->getTimeDisplay()->setTime(ts);
}

void Watch::timerTick()
{
    mode2->timerTick();
}

/*
Quick and dirty implementatie van Singleton
*/
Timer *Timer::getInstance()
{
    static Timer instance;
    return &instance;
}


Terminal *Watch::getDebugger()
{
    return debugger;
}

void Watch::nextMode()
{
    delete mode2;

    if (++mode > INCREMENT_MINUTES_MODE)
    {
        mode = DISPLAY_TIME_MODE;
    }

    switch (mode)
    {
        case DISPLAY_TIME_MODE:
            mode2 = new DisplayTimeMode(this);
            break;
        case INCREMENT_HOURS_MODE:
            mode2 = new IncrementHoursMode(this);
            break;
        case INCREMENT_MINUTES_MODE:
            mode2 = new IncrementMinutesMode(this);
            break;
        default:
            leds->write(0xff);
            break;
    }
}

void Watch::init()
{
    uart = Uart::getInstance();
    debugger = Uart::getInstance();
    timer = Timer::getInstance();
    leds = new Leds((volatile uint8_t * const)LEDS_0_BASE);
    segDisplay = new TimeDisplay((volatile uint32_t *)SEGDISPLAY_BASE);
    mode = DISPLAY_TIME_MODE;
    mode2 = new DisplayTimeMode(this);
    debugger->puts("Initializing Digital Watch...\r\n");
    RTCFactory rtcFactory;
    rtc = rtcFactory.createRTC();
    buttons = Buttons::getInstance();
    buttons->setObserver(new ButtonS4Action(this), 4);
    buttons->setObserver(new ButtonS5Action(this), 5);
    timer->setObserver(new TimerTick(this));
}

void Watch::increment()
{
    mode2->increase();
}

int main()
{
    Watch watch;
    watch.init();

    while (true)
    {
        // wachten op interrupts
    }

    return 0;
}


