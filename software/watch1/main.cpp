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

#include <system.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/alt_irq.h>
#include "uart.h"
#include "misc.h"
#include "rtc.h"

class Leds
{
public:
    Leds(volatile uint8_t * const base) { this->handle = base; }
    void write(uint8_t);
private:
    volatile uint8_t *handle;
};

class Watch;

/* State Pattern */
class AbstractMode
{
public:
    virtual void increase() {}
    virtual void timerTick() {}
protected:
    Watch *context;
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
};

class IncrementHoursMode : public AbstractMode
{
public:
    IncrementHoursMode(Watch *);
    void increase();
};

class IncrementMinutesMode : public AbstractMode
{
public:
    IncrementMinutesMode(Watch *);
    void increase();
};

class TimerTick : public Observer
{
private:
    Watch *watch;
public:
    TimerTick(Watch *watch) { this->watch = watch; }
    void update() { watch->timerTick(); }
};

class ButtonS4Action : public Observer
{
private:
    Watch *watch;
public:
    ButtonS4Action(Watch *watch) { this->watch = watch; }
    void update() { watch->nextMode(); }
};

class ButtonS5Action : public Observer
{
private:
    Watch *watch;
public:
    ButtonS5Action(Watch *watch) { this->watch = watch; }
    void update() { watch->increment(); }
};

void Leds::write(uint8_t data)
{
    *handle = data;
}

Leds *Watch::getLeds()
{
    return leds;
}

RTC *Watch::getRTC()
{
    return rtc;
}

DisplayTimeMode::DisplayTimeMode(Watch *context)
{
    this->context = context;
    context->getLeds()->write(~1);
    context->getTimeDisplay()->setBlinkMask(0);
}

IncrementMinutesMode::IncrementMinutesMode(Watch *context)
{
    this->context = context;
    context->getLeds()->write(~4);
    context->getTimeDisplay()->setBlinkMask(3);
}

IncrementHoursMode::IncrementHoursMode(Watch *context)
{
    this->context = context;
    context->getLeds()->write(~2);
    context->getTimeDisplay()->setBlinkMask(0x0c);
}

TimeDisplay *Watch::getTimeDisplay()
{
    return segDisplay;
}

void IncrementHoursMode::increase()
{
    RTC *rtc = context->getRTC();
    rtc->incrementHours();
    context->getTimeDisplay()->setTime(rtc->getTimeStamp());
}

void IncrementMinutesMode::increase()
{
    RTC *rtc = context->getRTC();
    rtc->incrementMinutes();
    context->getTimeDisplay()->setTime(rtc->getTimeStamp());
}

Uart *Watch::getUart()
{
    return uart;
}

void DisplayTimeMode::timerTick()
{
    RTC *rtc = context->getRTC();
    rtc->update();
    TimeStamp *ts = rtc->getTimeStamp();
    context->getUart()->puts(ts->toString());
    context->getTimeDisplay()->setTime(ts);
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
        mode = DISPLAY_TIME_MODE;

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
    timer->init((volatile void *)TIMER_0_BASE);
#ifdef LEDS_0_BASE
    leds = new Leds((volatile uint8_t * const)LEDS_0_BASE);
#endif

#ifdef SEGDISPLAY_BASE
    segDisplay = new TimeDisplay((volatile uint32_t *)SEGDISPLAY_BASE);
#endif
    mode = DISPLAY_TIME_MODE;
    mode2 = new DisplayTimeMode(this);
    debugger->puts("Initializing Digital Watch...\r\n");
    RTCFactory rtcFactory;
    rtc = rtcFactory.createRTC();
    buttons = Buttons::getInstance();
#ifdef BUTTONS_BASE
    buttons->init((volatile void *)BUTTONS_BASE);
#endif
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


