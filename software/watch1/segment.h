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
    void init();
    volatile uint32_t *handle;
};

class TimeDisplay : public SegDisplay
{
public:
    void setMinutes(uint8_t);
    void setTime(uint8_t, uint8_t);
    void setTime(TimeStamp *);
    static uint8_t lookup[];
};

#endif

