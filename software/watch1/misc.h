/*
2014 Jasper ter Weeme
*/

#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <stdint.h>

class Observer
{
public:
    virtual void update() = 0;
};

/*
Timer is Singleton en Observable
*/
class Timer
{

private:
    Timer();
    Observer *rh;
    void update();
    static void isr(void *context);
    volatile void *base;
    volatile uint32_t *base32;
public:
    static Timer *getInstance();
    void setObserver(Observer *obs) { rh = obs; }
    void init(volatile void *base);
};

class Buttons
{
public:
    static Buttons *getInstance();
    void setObserver(Observer *, int);
    void init(volatile void *base);
private:
    volatile void *base;
    volatile uint32_t *base32;
    void update();
    static void isr(void *context) { getInstance()->update(); }
    Observer *s4;
    Observer *s5;
    Observer *s6;
    static const uint8_t BUTTON_S4 = 3;
    static const uint8_t BUTTON_S5 = 5;
};

#endif

