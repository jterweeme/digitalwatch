#include <system.h>
#include "misc.h"
#include "mystl.h"

class MyVector : public mstd::vector<int>
{
public:
    MyVector(size_t capacity) : mstd::vector<int>(capacity) { }
    int sum();
};

class Teller1
{
public:
    Teller1();
    int tel();
private:
    SegDisplayEx sd;
    MyVector nummers;
};

Teller1::Teller1()
  :
    sd((uint32_t *)SEGDISPLAY_BASE),
    nummers(10)
{
    sd.blinkMask(0x00);
}

int MyVector::sum()
{
    int som = 0;

    for (iterator it = begin(); it < end(); it++)
        som += *it;

    return som;
}

int Teller1::tel()
{
    nummers.push_back(1);
    nummers.push_back(2);
    nummers.push_back(3);
    sd.writeInt(nummers.sum());
    return 0;
}

int main()
{
    Teller1 teller1;
    return teller1.tel();
}


