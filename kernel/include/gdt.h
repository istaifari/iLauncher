#pragma once
#include "../kernel.h"
typedef struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed)) gdt_entry;

/* Special pointer which includes the limit: The max bytes
 *  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
typedef struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) gdt_ptr;
