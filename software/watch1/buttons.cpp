#define SYSTEM_BUS_WIDTH 32

#include "buttons.h"
#include <system.h>
#include <stdio.h>
#include <io.h>
#include <sys/alt_irq.h>
#include <stdint.h>
#include "main.h"

Buttons::Buttons()
{
    init();
}

ButtonS4Action::ButtonS4Action()
{
}

ButtonS5Action::ButtonS5Action()
{
}

void ButtonS4Action::update()
{
    Uart::getInstance()->puts("Button S4 prezzed\r\n");
    Watch::getInstance()->nextMode();
}

void ButtonS5Action::update()
{
    Uart::getInstance()->puts("Button S5 prezzed\r\n");
    Watch::getInstance()->increase();
}

void Buttons::init()
{
    IOWR(BUTTONS_BASE, 2, 0xf);
    alt_ic_isr_register(BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID, BUTTONS_IRQ, isr, 0, 0);
    Uart::getInstance()->puts("Init buttons\r\n");

}

void Buttons::update()
{
    uint8_t btn = IORD(BUTTONS_BASE, 0);
    
    switch (btn)
    {
    case BUTTON_S4:
        if (s4)
            s4->update();
        return;
    case BUTTON_S5:
        if (s5)
            s5->update();
        return;
    }
}

void Buttons::addObserver(Observer *obs, int n)
{
    Uart::getInstance()->puts("Add button observer\r\n");

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

