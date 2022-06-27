#pragma once
#include "../kernel.h"

#define SOUNDBLASTER16_DSP_MIXER_PORT 0x224
#define SOUNDBLASTER16_DSP_MIXER_DATA_PORT 0x225
#define SOUNDBLASTER16_DSP_RESET 0x226
#define SOUNDBLASTER16_DSP_READ 0x22A
#define SOUNDBLASTER16_DSP_WRITE 0x22C
#define SOUNDBLASTER16_DSP_READ_STATUS 0x22E 
#define SOUNDBLASTER16_DSP_16BACK 0x22F

static void audio_start()
{
    return;
    unsigned char targetinterrupt = 5;
	outportb(SOUNDBLASTER16_DSP_MIXER_PORT,0x80);
	outportb(SOUNDBLASTER16_DSP_MIXER_PORT,0x02);
}

static void audio_start_beep(uint32_t nFrequence)
{
    uint32_t Div;
    uint8_t tmp;
    Div = 1193180 / nFrequence;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t)(Div));
    outb(0x42, (uint8_t)(Div >> 8));
    tmp = inb(0x61);
    if (tmp != (tmp | 3))
    {
        outb(0x61, tmp | 3);
    }
}

static void audio_stop_beep()
{
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}