/*
2014 Jasper ter Weeme
*/

#ifndef _SEGMENT_H_
#define _SEGMENT_H_

#include <stdint.h>
#include "ds1302.h"

class SegDisplay
{
public:
    SegDisplay();
    void write(uint32_t);
private:
    volatile uint32_t *handle;
protected:
    void init();
    volatile uint8_t *blinkMask;
};

class TimeDisplay : public SegDisplay
{
public:
    TimeDisplay();
    void init();
    void setMinutes(uint8_t);
    void setTime(uint8_t, uint8_t);
    void setTime(TimeStamp *);
    void setBlinkMask(uint8_t);
    static uint8_t lookup[];
};

#endif

