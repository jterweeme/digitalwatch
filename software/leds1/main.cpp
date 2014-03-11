#include <system.h>
#include <stdint.h>
#include <string.h>
int main()
{
    volatile uint8_t *leds = (volatile uint8_t *)LEDS_0_BASE;
    volatile uint32_t *segs = (volatile uint32_t *)SEGMENT_0_BASE;
    *leds = 0x14;
    segs[0] = 0x8e8c5e7c;
    segs[1] = 0x8e8c5e7c;


    uint8_t zegs[8] = {0x8e, 0x8c, 0x5e, 0x7c, 0x8e, 0x8c, 0x5e, 0x7c};
    memcpy((void *)segs, zegs, 8);

    return 0;
}


