#include "../include/interrupts.h"

void KeyboardInterrupt()
{
    PIC_EndMaster();
    if (port_byte_read(0x64) & 0x01 && !((port_byte_read(0x64) & 0b10) == 0) && !(port_byte_read(0x64) & 0b1))
    {
        SetKey(inb(0x60));
    }
    PIC_EndMaster();
}

void MouseInterrupt()
{
    PIC_EndSlave();
    if ((port_byte_read(0x64) & 0b10) == 0 && port_byte_read(0x64) & 0b1)
    {
        HandlePS2Mouse(inb(0x60));
    }
    PIC_EndSlave();

}

void PIC_EndMaster()
{
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave()
{
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void RemapPIC()
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
}