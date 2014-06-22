/*
2014 Jasper ter Weeme
*/

#include "misc.h"
#include <system.h>         // moet nog weg
#include <sys/alt_irq.h>

void Buttons::init(volatile void *base)
{
    this->base = base;
    this->base32 = (uint32_t *)base;
    base32[2] = 0xf;
#ifdef BUTTONS_IRQ
    alt_ic_isr_register(BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID, BUTTONS_IRQ, isr, 0, 0);
#endif
}

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

Buttons *Buttons::getInstance()
{
    static Buttons instance;
    return &instance;
}


