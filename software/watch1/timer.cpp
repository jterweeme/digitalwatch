/*
2014 Jasper ter Weeme
*/

#define SYSTEM_BUS_WIDTH 32

#include "timer.h"
#include <sys/alt_irq.h>
#include <priv/alt_legacy_irq.h>
#include "uart.h"
#include "main.h"
#include <io.h>
#include "rtc.h"

Timer::Timer()
{
    alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, isr, 0, 0);
}

TimerTick::TimerTick()
{
}

void TimerTick::update()
{
    Watch::getInstance()->timerTick();
}

/*
We houden het simpel: slechts een enkele observer
*/
void Timer::setObserver(Observer *obs)
{
    rh = obs;
}

void Timer::update()
{
    rh->update();
}

void Timer::isr(void *context)
{
    Timer::getInstance()->update();
    IOWR(TIMER_0_BASE, 0, 0);   // reset interrupt
}

/*
Quick and dirty implementatie van Singleton
*/
Timer *Timer::getInstance()
{
    static Timer instance;
    return &instance;
}


