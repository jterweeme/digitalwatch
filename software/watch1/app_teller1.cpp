#include <system.h>
#include <stdio.h>
#include "misc.h"
#include "mystl.h"

class Teller1
{
public:
    Teller1();
    int tel();
private:
    SegDisplayEx sd;
    mstd::vector<int> nummers;
};

Teller1::Teller1()
  :
    sd((uint32_t *)SEGDISPLAY_BASE),
    nummers(10)
{
    sd.blinkMask(0x00);
}

int Teller1::tel()
{
    nummers.push_back(1);
    nummers.push_back(2);
    nummers.push_back(3);
    int som = 0;

    for (mstd::vector<int>::iterator it = nummers.begin(); it < nummers.end(); it++)
        som += *it;

    sd.writeInt(som);
    return 0;
}

int main()
{
    Teller1 teller1;
    return teller1.tel();
}


