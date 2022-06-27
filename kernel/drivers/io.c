#include "../include/io.h"

uint8_t inb(uint16_t port)
{
    uint8_t result;
    asm("inb %1, %0"
        : "=a"(result)
        : "Nd"(port));
    return result;
}

void outb(uint16_t port, uint8_t data)
{
    asm("outb %0, %1"
        :
        : "a"(data), "Nd"(port));
}

uint8_t inw(uint16_t port)
{
    uint8_t ret;
    asm("inw %%dx,%%ax"
        : "=a"(ret)
        : "d"(port));
    return ret;
}

void outw(uint16_t port, uint16_t data)
{
    asm("outw %%ax, %%dx"
        :
        : "d"(port), "a"(data));
}

void io_wait()
{
    asm("outb %%al, $0x80"
        :
        : "a"(0));
}