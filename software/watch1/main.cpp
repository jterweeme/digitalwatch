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
#include <unistd.h>
#include <sys/alt_irq.h>
#include "uart.h"
#include "misc.h"
#include "rtc.h"

class Leds
{
private:
    volatile uint8_t * const handle;
public:
    Leds(volatile uint8_t * const base) : handle(base) { }
    void write(const uint8_t data) { *handle = data; }
};

class Watch;

class IWatch
{
public:
    virtual void timerTick() = 0;
};

class AbstractMode
{
protected:
    Watch *context;
public:
    AbstractMode(Watch *context) : context(context) { }
    virtual void increase() {}
    virtual void timerTick() {}
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

class DisplayTimeMode : public AbstractMode
{
public:
    DisplayTimeMode(Watch *);
    void timerTick();
};

class Watch : public IWatch
{
    Uart uart;
    JtagUart jtagUart;
    Leds leds;
    Buttons *buttons;
    TimeDisplay segDisplay;
    Timer *timer;
    RTC *rtc;
    Terminal *debugger;
    static const uint8_t DISPLAY_TIME_MODE = 1;
    static const uint8_t INCREMENT_HOURS_MODE = 2;
    static const uint8_t INCREMENT_MINUTES_MODE = 3;
    uint8_t mode;
    AbstractMode *mode2;
public:
    Watch();
    TimeDisplay *getTimeDisplay() { return &segDisplay; }
    Leds *getLeds() { return &leds; }
    RTC *getRTC() { return rtc; }
    Uart *getUart() { return &uart; }
    Terminal *getDebugger() { return debugger; }
    void nextMode();
    void increment() { mode2->increase(); }
    void timerTick() { mode2->timerTick(); }
    void init();
};

class TimerTick : public Observer
{
private:
    IWatch *watch;
public:
    TimerTick(IWatch *watch) : watch(watch) { }
    void update() { watch->timerTick(); }
};

class ButtonS4Action : public Observer
{
private:
    Watch *watch;
public:
    ButtonS4Action(Watch *watch) : watch(watch) { }
    void update() { watch->nextMode(); }
};

class ButtonS5Action : public Observer
{
private:
    Watch *watch;
public:
    ButtonS5Action(Watch *watch) : watch(watch) { }
    void update() { watch->increment(); }
};

Watch::Watch() :
#ifdef LEDS_BASE
    leds((volatile uint8_t * const)LEDS_BASE),
#else
    leds((volatile uint8_t * const)0),
#endif

#ifdef SEGDISPLAY_BASE
    segDisplay((volatile uint32_t *)SEGDISPLAY_BASE),
#endif
    uart((volatile uint32_t * const)UART_0_BASE),
    jtagUart((volatile uint32_t * const)JTAG_UART_0_BASE)
{
}

DisplayTimeMode::DisplayTimeMode(Watch *context) : AbstractMode(context)
{
    context->getLeds()->write(~1);
    context->getTimeDisplay()->setBlinkMask(0);
}

IncrementMinutesMode::IncrementMinutesMode(Watch *context) : AbstractMode(context)
{
    context->getLeds()->write(~4);
    context->getTimeDisplay()->setBlinkMask(3);
}

IncrementHoursMode::IncrementHoursMode(Watch *context) : AbstractMode(context)
{
    context->getLeds()->write(~2);
    context->getTimeDisplay()->setBlinkMask(0x0c);
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

void DisplayTimeMode::timerTick()
{
    RTC *rtc = context->getRTC();
    rtc->update();
    TimeStamp *ts = rtc->getTimeStamp();
    context->getUart()->puts(ts->toString());
    context->getTimeDisplay()->setTime(ts);
}

Timer *Timer::getInstance()
{
    static Timer instance;
    return &instance;
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
            leds.write(0xff);
            break;
    }
}

void Watch::init()
{
    debugger = JtagUart::getInstance();
    timer = Timer::getInstance();
    timer->init((volatile void *)TIMER_0_BASE);
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


