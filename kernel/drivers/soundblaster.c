#include <soundblaster.h>

void audio_start_beep(uint32_t fq)
{
    uint32_t Div;
    uint8_t tmp;
    Div = 1193180 / fq;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t)(Div));
    outb(0x42, (uint8_t)(Div >> 8));
    tmp = inb(0x61);
    if (tmp != (tmp | 3))
    {
        outb(0x61, tmp | 3);
    }
}

void audio_stop_beep()
{
    uint8_t tmp = inb(0x61) & 0xFC;
 	outb(0x61, tmp);
}

void InitSoundBlaster16()
{
    /* Coming Soon */
    return;
}