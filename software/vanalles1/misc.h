#ifndef _MISC_H_
#define _MISC_H_
#include <stdint.h>

class SegmentDisplay
{
private:
    volatile uint32_t *base;
public:
    SegmentDisplay(volatile uint32_t *base) { this->base = base; }
    void setBlinkMask(uint8_t mask) { *(base + 8) = mask; }
};
#endif

