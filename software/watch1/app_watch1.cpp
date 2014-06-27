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

class RTCFactory
{
    volatile void * const ds1302_base;
    I2CBus * const i2cBus;
public:
    RTCFactory();
    RTCFactory(volatile void * const ds1302_base);
    RTCFactory(volatile void * const ds1302_base, I2CBus * const i2cBus);
    RTC *createRTC();
};

class Watch : public IWatch
{
    Uart uart;
    JtagUart jtagUart;
    Leds leds;
    Buttons buttons;
    I2CBus i2cBus;
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
    i2cBus((void *)I2CBUS_0_BASE),
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

RTCFactory::RTCFactory()
  :
    ds1302_base(0),
    i2cBus(0)
{
}

RTCFactory::RTCFactory(volatile void * const ds1302_base)
  :
    ds1302_base(ds1302_base),
    i2cBus(0)
{
}

RTCFactory::RTCFactory(volatile void * const ds1302_base,
    I2CBus * const i2cBus)
  :
    ds1302_base(ds1302_base),
    i2cBus(i2cBus)
{
}

RTC *RTCFactory::createRTC()
{
    Uart *uart = Uart::getInstance();
    uart->puts("RTC Factory\r\n");
    static DS1302 test(ds1302_base);
    test.update();
    TimeStamp testStamp = test.getTimeStamp();
    uart->puts(testStamp.toString());
    PCF8563 pcf(i2cBus);
    TimeStamp testStamp2 = pcf.getTimeStamp();
    uart->puts(testStamp2.toString());

    if (testStamp.getHour10() > 2)
        return FallBackRTC::getInstance();

    return &test;
}

void DisplayTimeMode::init()
{
    context->getLeds()->write(~1);
    context->getTimeDisplay()->blinkMask(0);
}

void IncrementMinutesMode::init()
{
    context->getLeds()->write(~4);
    context->getTimeDisplay()->blinkMask(3);
}

void IncrementHoursMode::init()
{
    context->getLeds()->write(~2);
    context->getTimeDisplay()->blinkMask(0x0c);
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

int main()
{
    Watch watch;

    while (true)
    {
        // wachten op interrupts
    }

    return 0;
}


