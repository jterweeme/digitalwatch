/*
2014 Jasper ter Weeme
*/

#include "misc.h"
#include <sys/alt_irq.h>

Timer::Timer()
{
    alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, isr, 0, 0);
}

void Timer::init(volatile void *base)
{
    this->base = base;
    this->base32 = (volatile uint32_t *)base;
}

void Timer::update()
{
    rh->update();
    base32[0] = 0;
}

void Timer::isr(void *context)
{
    Timer::getInstance()->update();
    //base32[0] = 0;  // reset interrupt
    //IOWR(TIMER_0_BASE, 0, 0);   // reset interrupt
}


