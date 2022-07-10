#include <pic.h>

void PIC_End()
{
    outb(0x20, 0x20);
    outb(0xA0, 0x20);
}

void RemapPIC()
{
    uint8_t a1, a2;

    a1 = inb(0x21);
    io_wait();
    a2 = inb(0xA1);
    io_wait();

    outb(0x20, 0x10 | 0x01);
    io_wait();
    outb(0xA0, 0x10 | 0x01);
    io_wait();

    outb(0x21, 0x20);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();

    outb(0x21, 4);
    io_wait();
    outb(0xA1, 2);
    io_wait();

    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    outb(0x21, a1);
    io_wait();
    outb(0xA1, a2);
}