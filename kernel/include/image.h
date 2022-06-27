#pragma once
#include "../kernel.h"

typedef struct tga_header
{
    unsigned char magic1;
    unsigned char colormap;
    unsigned char encoding;
    unsigned short cmaporig, cmaplen;
    unsigned char cmapent;
    unsigned short x;
    unsigned short y;
    unsigned short h;
    unsigned short w;
    unsigned char bpp;
    unsigned char pixeltype;
} __attribute__((packed)) tga_header_t;

void *tga_load(void* filename);