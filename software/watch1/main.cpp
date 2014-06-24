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

#include "misc.h"
#include <system.h>
#include <stdint.h>
#include <sys/alt_irq.h>

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
    virtual ~IWatch() { }
};

class AbstractMode
{
protected:
    IWatch *context;
public:
    const uint8_t id;
    AbstractMode(IWatch *context, const uint8_t id) : context(context), id(id) { }
    virtual void init() { }
    virtual void increase() {}
    virtual void timerTick() {}
    virtual ~AbstractMode() { }
};

class DisplayTimeMode : public AbstractMode
{
public:
    static const uint8_t ID = 1;
    DisplayTimeMode(IWatch *context) : AbstractMode(context, ID) { }
    void init();
    void timerTick();
};

class IncrementHoursMode : public AbstractMode
{
public:
    static const uint8_t ID = 2;
    IncrementHoursMode(IWatch *context) : AbstractMode(context, ID) { }
    void init();
    void increase();
};

class IncrementMinutesMode : public AbstractMode
{
public:
    static const uint8_t ID = 3;
    IncrementMinutesMode(IWatch *context) : AbstractMode(context, ID) { }
    void init();
    void increase();
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

class Watch : public IWatch
{
    Uart uart;
    JtagUart jtagUart;
    Leds leds;
    Buttons buttons;
    TimeDisplay segDisplay;
    Timer timer;
    RTC *rtc;
    Terminal *debugger;
    DisplayTimeMode dtm;
    IncrementHoursMode ihm;
    IncrementMinutesMode imm;
    AbstractMode *mode2;
    TimerTick tt;
    ButtonS4Action a4;
    ButtonS5Action a5;
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

Watch::Watch()
  :
    uart((uint32_t *)UART_0_BASE),
    jtagUart((uint32_t *)JTAG_UART_0_BASE),
#ifdef LEDS_BASE
    leds((uint8_t *)LEDS_BASE),
#endif
    buttons((void *)BUTTONS_BASE, BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID, BUTTONS_IRQ),
#ifdef SEGDISPLAY_BASE
    segDisplay((uint32_t *)SEGDISPLAY_BASE),
#endif
    timer((void *)TIMER_0_BASE, TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ),
    debugger(&uart),
    dtm(this),
    ihm(this),
    imm(this),
    mode2(&dtm),
    tt(this),
    a4(this),
    a5(this)
{
    mode2->init();
    debugger->puts("Initializing Digital Watch...\r\n");
    RTCFactory rtcFactory((void *)DS1302_BASE);
    rtc = rtcFactory.createRTC();
    buttons.setObserver(&a4, 4);
    buttons.setObserver(&a5, 5);
    timer.setObserver(&tt);
}

void DisplayTimeMode::init()
{
    context->getLeds()->write(~1);
    context->getTimeDisplay()->setBlinkMask(0);
}

void IncrementMinutesMode::init()
{
    context->getLeds()->write(~4);
    context->getTimeDisplay()->setBlinkMask(3);
}

void IncrementHoursMode::init()
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
    TimeStamp ts = rtc->getTimeStamp();
    context->getUart()->puts(ts.toString());
    context->getTimeDisplay()->setTime(ts);
}

void Watch::nextMode()
{
    switch (mode2->id)
    {
        case DisplayTimeMode::ID:
            mode2 = &ihm;
            mode2->init();
            break;
        case IncrementHoursMode::ID:
            mode2 = &imm;
            mode2->init();
            break;
        case IncrementMinutesMode::ID:
            mode2 = &dtm;
            mode2->init();
            break;
        default:
            leds.write(0xff);
            break;
    }
}

Timer::Timer(volatile void * const base, const unsigned ctl, const unsigned irq)
  :
    base(base),
    base32((uint32_t *)base)
{
    instance = this;
    alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, isr, 0, 0);
}

Timer *Timer::instance;

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

uint8_t TimeDisplay::lookup[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

void TimeDisplay::setTime(const uint8_t uur, const uint8_t min)
{
    const uint8_t d = lookup[uur / 10];
    uint8_t c = lookup[uur % 10];
    c &= ~0x80;     // dot
    const uint8_t b = lookup[min / 10];
    const uint8_t a = lookup[min % 10];
    write(a | b << 8 | c << 16 | d << 24);
}

void TimeDisplay::setTime(TimeStamp ts)
{
    const uint8_t d = lookup[ts.getHour10()];
    uint8_t c = lookup[ts.getHour()];
    c &= ~0x80;
    const uint8_t b = lookup[ts.getMinutes10()];
    const uint8_t a = lookup[ts.getMinutes()];
    write(a | b << 8 | c << 16 | d << 24);
}

void SegDisplay::write(const uint32_t data)
{
    *handle = data;
    handle[1] = 0xffffffff;
}

Buttons::Buttons(volatile void * const base, const unsigned ctl, const unsigned irq)
  :
    base(base),
    base32((uint32_t *)base)
{
    instance = this;
    base32[2] = 0xf;
#ifdef BUTTONS_IRQ
    alt_ic_isr_register(ctl, irq, isr, 0, 0);
#endif
}

Buttons *Buttons::instance;

void Buttons::update()
{
    switch (base32[0])
    {
    case BUTTON_S4:
        if (s4)         // i.v.m. null-pointer
            s4->update();

        return;
    case BUTTON_S5:
        if (s5)         // i.v.m. null-pointer
            s5->update();

        return;
    }

    base32[3] = 0;
}

/*
Er zijn drie buttons beschikbaar
Elk van de drie buttons heeft een enkele Observer
*/
void Buttons::setObserver(Observer *obs, int n)
{
    switch (n)
    {
    case 4:
        s4 = obs;
        return;
    case 5:
        s5 = obs;
        return;
    case 6:
        s6 = obs;
        return;
    }
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


