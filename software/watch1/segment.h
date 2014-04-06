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
    SegDisplay(volatile uint32_t *);
    void write(uint32_t);
    void setBlinkMask(uint8_t);
private:
    volatile uint32_t *handle;
    void init(volatile uint32_t *);
    volatile uint8_t *blinkMask;
};

class TimeDisplay : public SegDisplay
{
public:
    TimeDisplay(volatile uint32_t *);
    void setMinutes(uint8_t);
    void setTime(uint8_t, uint8_t);
    void setTime(TimeStamp *);
private:
    static uint8_t lookup[];
};

#endif

