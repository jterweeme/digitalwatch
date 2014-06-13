/*
2014 Jasper ter Weeme
*/

#include "misc.h"
#include <stdint.h>

uint8_t TimeDisplay::lookup[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

void TimeDisplay::setTime(const uint8_t uur, const uint8_t min)
{
    const uint8_t d = lookup[uur / 10];
    uint8_t c = lookup[uur % 10];
    c &= ~0x80;     // dot
    const uint8_t b = lookup[min / 10];
    const uint8_t a = lookup[min % 10];
    write(a | b << 8 | c << 16 | d << 24);
}

void TimeDisplay::setTime(TimeStamp *ts)
{
    const uint8_t d = lookup[ts->getHour10()];
    uint8_t c = lookup[ts->getHour()];
    c &= ~0x80;
    const uint8_t b = lookup[ts->getMinutes10()];
    const uint8_t a = lookup[ts->getMinutes()];
    write(a | b << 8 | c << 16 | d << 24);
}

void SegDisplay::write(const uint32_t data)
{
    *handle = data;
    handle[1] = 0xffffffff;
}


