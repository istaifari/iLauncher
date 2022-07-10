#include <io.h>

void outb(uint16_t port, uint8_t val)
{
    asm("outb %1, %0"
        :
        : "dN"(port), "a"(val));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm("inb %1, %0"
        : "=a"(ret)
        : "Nd"(port));
    return ret;
}

uint16_t inw(uint16_t port)
{
    uint16_t rv;
    asm("inw %1, %0"
        : "=a"(rv)
        : "dN"(port));
    return rv;
}

void outw(uint16_t port, uint16_t data)
{
    asm("outw %1, %0"
        :
        : "dN"(port), "a"(data));
}

uint32_t inl(uint16_t port)
{
    uint32_t rv;
    asm("inl %%dx, %%eax"
        : "=a"(rv)
        : "dN"(port));
    return rv;
}

void outl(uint16_t port, uint32_t data)
{
    asm("outl %%eax, %%dx"
        :
        : "dN"(port), "a"(data));
}

void io_wait()
{
    asm("outb %%al, $0x80"
        :
        : "a"(0));
}