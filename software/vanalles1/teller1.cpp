#include <system.h>
#include <stdio.h>
#include "misc.h"
class Teller1
{
public:
    Teller1(SegmentDisplay *);
    int tel();
private:
    SegmentDisplay *sd;
};

Teller1::Teller1(SegmentDisplay *sd)
{
    this->sd = sd;
}

int Teller1::tel()
{
    while (true)
    {
        printf("Bulawayo");

        for (volatile int i = 0; i < 0xffff; i++)
        {
        }
    }

    sd->setBlinkMask(0x88);
    return 0;
}

int main()
{
    SegmentDisplay sd((volatile uint32_t *)SEGDISPLAY_BASE);
    Teller1 teller1(&sd);
    return teller1.tel();
}


