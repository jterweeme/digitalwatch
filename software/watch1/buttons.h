/*
2014 Jasper ter Weeme
*/

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "observer.h"
#include <stdint.h>

class Buttons
{
public:
    static Buttons *getInstance();
    void setObserver(Observer *, int);
private:
    Buttons();
    void update();
    void init();
    static void isr(void *context);
    Observer *s4;
    Observer *s5;
    Observer *s6;
    static const uint8_t BUTTON_S4 = 3;
    static const uint8_t BUTTON_S5 = 5;
};

#endif


