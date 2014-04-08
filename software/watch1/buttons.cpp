/*
2014 Jasper ter Weeme
*/

#define SYSTEM_BUS_WIDTH 32

#include "buttons.h"
#include <system.h>
#include <stdio.h>
#include <io.h>
#include <sys/alt_irq.h>
#include <stdint.h>

Buttons::Buttons()
{
    init();
}

void Buttons::init()
{
    IOWR(BUTTONS_BASE, 2, 0xf);
    alt_ic_isr_register(BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID, BUTTONS_IRQ, isr, 0, 0);
}

void Buttons::update()
{
    uint8_t btn = IORD(BUTTONS_BASE, 0);
    
    switch (btn)
    {
    case BUTTON_S4:
        if (s4)         // i.v.m. null-pointer
        {
            s4->update();
        }
        return;
    case BUTTON_S5:
        if (s5)         // i.v.m. null-pointer
        {
            s5->update();
        }
        return;
    }
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

Buttons *Buttons::getInstance()
{
    static Buttons instance;
    return &instance;
}

void Buttons::isr(void *context)
{
    Buttons::getInstance()->update();
    IOWR(BUTTONS_BASE, 3, 0);
}

