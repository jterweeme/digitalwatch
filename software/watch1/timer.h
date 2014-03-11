#ifndef _TIMER_H_
#define _TIMER_H_

#include "observer.h"

/*
Timer is Singleton en Observable
*/
class Timer
{
public:
    static Timer *getInstance();
    void addObserver(Observer *);
private:
    Timer();
    void update();
    static void isr(void *context);
    Observer *rh;
};

class TimerTick : public Observer
{
public:
    TimerTick();
    void update();
};

#endif

