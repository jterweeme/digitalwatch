#include "segment.h"
#include <stdint.h>
#include <system.h>
#include "ds1302.h"

SegDisplay::SegDisplay()
{
    init();
}

TimeDisplay::TimeDisplay()
{
    init();
}

uint8_t TimeDisplay::lookup[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

void TimeDisplay::setMinutes(uint8_t min)
{
    write(lookup[0] | (lookup[0] << 8));
}

void TimeDisplay::setBlinkMask(uint8_t mask)
{
    *blinkMask = mask;
}

void TimeDisplay::init()
{
    SegDisplay::init();
    setBlinkMask(0);
}

void TimeDisplay::setTime(uint8_t uur, uint8_t min)
{
    uint8_t d = lookup[uur / 10];
    uint8_t c = lookup[uur % 10];
    c &= ~0x80;     // dot
    uint8_t b = lookup[min / 10];
    uint8_t a = lookup[min % 10];
    write(a | b << 8 | c << 16 | d << 24);
}

void TimeDisplay::setTime(TimeStamp *ts)
{
    uint8_t d = lookup[ts->getHour10()];
    uint8_t c = lookup[ts->getHour()];
    c &= ~0x80;
    uint8_t b = lookup[ts->getMinutes10()];
    uint8_t a = lookup[ts->getMinutes()];
    write(a | b << 8 | c << 16 | d << 24);
}

void SegDisplay::init()
{
    handle = (volatile uint32_t *)SLAVE_TEMPLATE_0_BASE;
    blinkMask = (volatile uint8_t *)SLAVE_TEMPLATE_0_BASE + 8;
}

void SegDisplay::write(uint32_t data)
{
    *handle = data;
}


