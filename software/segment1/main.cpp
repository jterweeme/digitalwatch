#include <stdint.h>
#include <system.h>

class SegDisplay
{
public:
    SegDisplay();
    void write(uint32_t);
    void setLedBlinkMask(uint8_t);
private:
    void init();
    volatile uint8_t *handle;
};

SegDisplay::SegDisplay()
{
    init();
}

void SegDisplay::init()
{
    handle = (volatile uint8_t *)SLAVE_TEMPLATE_0_BASE;
}

void SegDisplay::write(uint32_t data)
{
    *handle = data;
}

int main()
{
    SegDisplay segDisplay;
    return 0;
}


