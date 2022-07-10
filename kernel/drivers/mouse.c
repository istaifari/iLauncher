#include <mouse.h>

#define PS2Leftbutton 0x01
#define PS2Middlebutton 0x04
#define PS2Rightbutton 0x02
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0x80
#define PS2YOverflow 0x40

void MouseWait(int a_type)
{
    long timeout = 100000;
    if (!a_type)
    {
        while (--timeout)
        {
            if ((inb(0x64) & 0x01) == 1)
            {
                return;
            }
        }
        return;
    }
    else
    {
        while (--timeout)
        {
            if (!((inb(0x64) & 0x02)))
            {
                return;
            }
        }
        return;
    }
}

void MouseWrite(uint8_t value)
{
    MouseWait(1);
    outb(0x64, 0xD4);
    MouseWait(1);
    outb(0x60, value);
}

uint8_t MouseRead()
{
    MouseWait(0);
    return inb(0x60);
}

mouse_t *mouse_ps2;
uint8_t cm = 0;

void mouse_info_process_packet(mouse_t *mouse, uint8_t data)
{
    if (!mouse)
        return;

    switch (mouse->packet[3])
    {
    case 0:
        if (mouse->packetready)
            break;
        if (data & 0b00001000 == 0)
            break;
        mouse->packet[0] = data;
        break;
    case 1:
        if (mouse->packetready)
            break;
        mouse->packet[1] = data;
        break;
    case 2:
        if (mouse->packetready)
            break;
        mouse->packet[2] = data;
        mouse->packetready = true;
        break;
    }

    mouse->packet[3] = (mouse->packet[3] + 1) % 3;

    if (mouse->packet[0] & PS2Leftbutton)
    {
        mouse->OnMouseDown_Left = true;
        /*mouse->count++;*/
    }
    else
    {
        /*if (mouse->count >= 10)
            mouse->OnMouseClick_Left = true;
        else
        {
            mouse->count = 0;
            mouse->OnMouseClick_Left = false;
        }*/
        mouse->OnMouseDown_Left = false;
    }

    if (mouse->packet[0] & PS2Middlebutton)
    {
        mouse->OnMouseDown_Middle = true;
    }
    else
    {
        mouse->OnMouseDown_Middle = false;
    }

    if (mouse->packet[0] & PS2Rightbutton)
    {
        mouse->OnMouseDown_Right = true;
    }
    else
    {
        mouse->OnMouseDown_Right = false;
    }

    if (!mouse->packetready)
        return;

    mouse->x_old = mouse->x;
    mouse->y_old = mouse->y;
    if (!mouse->Sensitivity)
        mouse->Sensitivity = 2;

    bool xNegative, yNegative, xOverflow, yOverflow;

    if (mouse->packet[0] & PS2XSign)
    {
        xNegative = true;
    }
    else
    {
        xNegative = false;
    }

    if (mouse->packet[0] & PS2YSign)
    {
        yNegative = true;
    }
    else
    {
        yNegative = false;
    }

    if (mouse->packet[0] & PS2XOverflow)
    {
        xOverflow = true;
    }
    else
    {
        xOverflow = false;
    }

    if (mouse->packet[0] & PS2YOverflow)
    {
        yOverflow = true;
    }
    else
    {
        yOverflow = false;
    }

    if (!xNegative)
    {
        mouse->x += mouse->packet[1] * mouse->Sensitivity;
        if (xOverflow)
            mouse->x += 255;
    }
    else
    {
        mouse->packet[1] = (255 + 1) - mouse->packet[1];
        mouse->x -= mouse->packet[1] * mouse->Sensitivity;
        if (xOverflow)
            mouse->x -= 255;
    }

    if (!yNegative)
    {
        mouse->y -= mouse->packet[2] * mouse->Sensitivity;
        if (yOverflow)
            mouse->y -= 255;
    }
    else
    {
        mouse->packet[2] = (255 + 1) - mouse->packet[2];
        mouse->y += mouse->packet[2] * mouse->Sensitivity;
        if (yOverflow)
            mouse->y += 255;
    }
    if (mouse->x < -1)
        mouse->x = -1;
    if (mouse->x >= (long)vga_getresolution(1))
        mouse->x = (long)vga_getresolution(1) - 1;
    if (mouse->y < -1)
        mouse->y = -1;
    if (mouse->y >= (long)vga_getresolution(2))
        mouse->y = (long)vga_getresolution(2) - 1;
    mouse->packetready = false;
}

__attribute__((interrupt)) void MouseInt_Handler(int_frame_t *r)
{
    asm("cli");
    uint8_t mousedata = inb(0x60);
    mouse_info_process_packet(mouse_ps2, mousedata);
    mouse_keyboard = "mouse ps/2";
    PIC_End();
    asm("sti");
}

void MouseCheckPos(mouse_t *mouse)
{
    if (mouse->x < -1)
        mouse->x = -1;
    if (mouse->x >= (long)vga_getresolution(1))
        mouse->x = (long)vga_getresolution(1) - 1;
    if (mouse->y < -1)
        mouse->y = -1;
    if (mouse->y >= (long)vga_getresolution(2))
        mouse->y = (long)vga_getresolution(2) - 1;
}

mouse_t *mouse_info_setup()
{
    mouse_t *ret = kmalloc(sizeof(mouse_t));
    memset(ret, 0, sizeof(mouse_t));
    ret->Sensitivity = 0;
    ret->packet[3] = 0;
    ret->packet[4] = 0;
    return ret;
}

void InitPS2Mouse()
{
    unsigned char status;
    if (!mouse_ps2)
    {
        asm("cli");
        mouse_ps2 = mouse_info_setup();
        MouseWait(1);
        outb(0x64, 0xA8);
        MouseWait(1);
        outb(0x64, 0x20);
        MouseWait(0);
        status = inb(0x60) | 2;
        MouseWait(1);
        outb(0x64, 0x60);
        MouseWait(1);
        outb(0x60, status);
        MouseWrite(0xF6);
        MouseRead();
        MouseWrite(0xF4);
        MouseRead();
        asm("sti");
    }
    idt_set_gate(32 + 12, MouseInt_Handler, 0x08, 0x8E);
}