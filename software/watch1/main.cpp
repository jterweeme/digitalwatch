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
    volatile uint8_t * const handle;
public:
    Leds() : handle(0) { }
    Leds(volatile uint8_t * const base) : handle(base) { }
    void write(const uint8_t data) { *handle = data; }
};

class IWatch
{
public:
    virtual void timerTick() = 0;
    virtual void nextMode() = 0;
    virtual void increment() = 0;
    virtual Leds *getLeds() = 0;
    virtual Uart *getUart() = 0;
    virtual RTC *getRTC() = 0;
    virtual TimeDisplay *getTimeDisplay() = 0;
};

class AbstractMode
{
protected:
    IWatch *context;
public:
    AbstractMode(IWatch *context) : context(context) { }
    virtual void increase() {}
    virtual void timerTick() {}
};

class IncrementHoursMode : public AbstractMode
{
public:
    IncrementHoursMode(IWatch *);
    void increase();
};

class IncrementMinutesMode : public AbstractMode
{
public:
    IncrementMinutesMode(IWatch *);
    void increase();
};

class DisplayTimeMode : public AbstractMode
{
public:
    DisplayTimeMode(IWatch *);
    void timerTick();
};

class Watch : public IWatch
{
    Uart uart;
    JtagUart jtagUart;
    Leds leds;
    Buttons buttons;
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
};

class TimerTick : public Observer
{
    IWatch *watch;
public:
    TimerTick(IWatch *watch) : watch(watch) { }
    void update() { watch->timerTick(); }
};

class ButtonS4Action : public Observer
{
    IWatch *watch;
public:
    ButtonS4Action(IWatch *watch) : watch(watch) { }
    void update() { watch->nextMode(); }
};

class ButtonS5Action : public Observer
{
    IWatch *watch;
public:
    ButtonS5Action(IWatch *watch) : watch(watch) { }
    void update() { watch->increment(); }
};

Watch::Watch() :
#ifdef LEDS_BASE
    leds((uint8_t *)LEDS_BASE),
#endif
    buttons((void *)BUTTONS_BASE, BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID, BUTTONS_IRQ),
#ifdef SEGDISPLAY_BASE
    segDisplay((uint32_t *)SEGDISPLAY_BASE),
#endif
    uart((uint32_t *)UART_0_BASE),
    jtagUart((uint32_t *)JTAG_UART_0_BASE)
{
    debugger = JtagUart::getInstance();
    timer = Timer::getInstance();
    timer->init((void *)TIMER_0_BASE);
    mode = DISPLAY_TIME_MODE;
    mode2 = new DisplayTimeMode(this);
    debugger->puts("Initializing Digital Watch...\r\n");
    volatile uint32_t * const clk = (uint32_t *)DS1302_CLK_BASE;
    volatile uint32_t * const io = (uint32_t *)DS1302_IO_BASE;
    volatile uint32_t * const rst = (uint32_t *)DS1302_RESET_BASE;
    RTCFactory rtcFactory(clk, io, rst);
    rtc = rtcFactory.createRTC();
    buttons.setObserver(new ButtonS4Action(this), 4);
    buttons.setObserver(new ButtonS5Action(this), 5);
    timer->setObserver(new TimerTick(this));
}

DisplayTimeMode::DisplayTimeMode(IWatch *context) : AbstractMode(context)
{
    context->getLeds()->write(~1);
    context->getTimeDisplay()->setBlinkMask(0);
}

IncrementMinutesMode::IncrementMinutesMode(IWatch *context) : AbstractMode(context)
{
    context->getLeds()->write(~4);
    context->getTimeDisplay()->setBlinkMask(3);
}

IncrementHoursMode::IncrementHoursMode(IWatch *context) : AbstractMode(context)
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

Timer::Timer()
{
    alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, isr, 0, 0);
}

void Timer::init(volatile void *base)
{
    this->base = base;
    this->base32 = (volatile uint32_t *)base;
}

void JtagUart::putc(const char c)
{
    while (*ctl & 0xffff0000 == 0) { }
    base[0] = c;
}

Uart *Uart::instance;
JtagUart *JtagUart::instance;

void Uart::putc(const char c)
{
    while ((base[2] & (1<<6)) == 0) { }
    base[1] = c;
}

int main()
{
    Watch watch;

    while (true)
    {
        // wachten op interrupts
    }

    return 0;
}


