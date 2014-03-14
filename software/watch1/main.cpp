/*
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
#include "main.h"
#include "ds1302.h"
#include "buttons.h"

Watch::Watch()
{
    init();
}

Leds::Leds()
{
    handle = (volatile uint8_t *)LEDS_0_BASE;
}

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

AbstractMode::AbstractMode()
{
}

DisplayTimeMode::DisplayTimeMode()
{
}

DisplayTimeMode::DisplayTimeMode(Watch *watch)
{
    watch->getLeds()->write(~1);
    watch->getTimeDisplay()->setBlinkMask(0);
}

IncrementHoursMode::IncrementHoursMode()
{
    Watch *watch = Watch::getInstance();
    watch->getLeds()->write(~2);
    watch->getTimeDisplay()->setBlinkMask(0x0c);
}

IncrementMinutesMode::IncrementMinutesMode()
{
    Watch *watch = Watch::getInstance();
    watch->getLeds()->write(~4);
    watch->getTimeDisplay()->setBlinkMask(3);
}

TimeDisplay *Watch::getTimeDisplay()
{
    return segDisplay;
}

JtagUart::JtagUart()
{
    init();
}

void DisplayTimeMode::increase()
{
}

void IncrementHoursMode::increase()
{
}

void IncrementMinutesMode::increase()
{
}

void JtagUart::init()
{
    handle = (volatile uint32_t *)JTAG_UART_0_BASE;
}

void JtagUart::putc(const char c)
{
    while ((handle[2] & (1<<6)) == 0) {
    }
    
    handle[1] = c;
}

void JtagUart::puts(const char *s)
{
    while (*s)
        putc(*s++);
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
            mode2 = new IncrementHoursMode();
            break;
        case INCREMENT_MINUTES_MODE:
            mode2 = new IncrementMinutesMode();
            break;
        default:
            leds->write(0xff);
            break;
    }
}

void Watch::init()
{
    leds = new Leds();
    segDisplay = new TimeDisplay();
    mode = DISPLAY_TIME_MODE;
    mode2 = new DisplayTimeMode(this);
    Uart::getInstance()->puts("Initializing Digital Watch...\r\n");
    RTCFactory rtcFactory;
    rtc = rtcFactory.createRTC();
    buttons = Buttons::getInstance();
    buttons->addObserver(new ButtonS4Action(), 4);
    buttons->addObserver(new ButtonS5Action(), 5);
    Timer::getInstance()->addObserver(new TimerTick());
}

void Watch::increase()
{
    mode2->increase();
}

Watch *Watch::getInstance()
{
    static Watch instance;
    return &instance;
}

int main()
{
    Watch::getInstance();

    while (true)
    {
    }

    return 0;
}


