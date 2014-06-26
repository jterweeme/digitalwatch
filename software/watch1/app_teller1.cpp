#include <system.h>
#include <stdio.h>
#include "misc.h"

class Teller1
{
public:
    Teller1();
    int tel();
private:
    SegDisplay sd;
};

Teller1::Teller1()
  :
    sd((uint32_t *)SEGDISPLAY_BASE)
{
}

int Teller1::tel()
{
    while (true)
    {
        for (volatile int i = 0; i < 0xffff; i++)
        {
        }
    }

    sd.setBlinkMask(0x88);
    return 0;
}

int main()
{
    Teller1 teller1;
    return teller1.tel();
}


