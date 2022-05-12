#include "../include/interrupts.h"

#define ENABLE_PS2 0xA8
#define KBD_STAT 0x64
#define MOUSE 0xD4
#define MOUSE_STREAM 0xF4
#define MOUSE_DISABLE 0xF5
#define KBD_CMD 0x60
#define DISABLE_KBD 0xAD
#define ENABLE_KBD 0xAE

void CheckPort()
{
    unsigned char temp;
    while (true)
    {
        temp = inb(0x64);
        if ((temp & 2) == 0)
            return;
    }
}

char CheckMouse()
{
    unsigned char temp;
    temp = inb(0x64);

    if (temp & 1)
        return 0;
    else
        return 1;
}

char GetMouseByte()
{
    outb(0x64, 0xAD);
    CheckPort();
    char ret = 1;
    while (ret)
        ret = CheckMouse();
    MouseWait();
    ret = inb(0x60);
    outb(0x64, 0xAE);
    CheckPort();
    PIC_EndMaster();
    return ret;
}

void MouseAndKeyboard_FixUpdate()
{
    uint8_t MouseData = GetMouseByte();
    HandlePS2Mouse(MouseData);
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