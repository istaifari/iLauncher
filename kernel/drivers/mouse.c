#include "../include/mouse.h"

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
	if (!a_type) {
		while (--timeout) {
			if ((inb(0x64) & 0x01) == 1) {
				return;
			}
		}
		return;
	} else {
		while (--timeout) {
			if (!((inb(0x64) & 0x02))) {
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

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
MouseInfo Mouse;
bool MousePacketReady = false;

void ProcessMousePacket();

void PS2MouseData(unsigned char data)
{
    switch (MouseCycle)
    {
    case 0:
        if (MousePacketReady)
            break;
        if (data & 0b00001000 == 0)
            break;
        MousePacket[0] = data;
        MouseCycle++;
        break;
    case 1:
        if (MousePacketReady)
            break;
        MousePacket[1] = data;
        MouseCycle++;
        break;
    case 2:
        if (MousePacketReady)
            break;
        MousePacket[2] = data;
        MousePacketReady = true;
        MouseCycle = 0;
        break;
    }

    if (MousePacket[0] & PS2Leftbutton)
    {
        Mouse.OnMouseDown_Left = true;
    }
    else
    {
        Mouse.OnMouseDown_Left = false;
    }

    if (MousePacket[0] & PS2Middlebutton)
    {
        Mouse.OnMouseDown_Middle = true;
    }
    else
    {
        Mouse.OnMouseDown_Middle = false;
    }

    if (MousePacket[0] & PS2Rightbutton)
    {
        Mouse.OnMouseDown_Right = true;
    }
    else
    {
        Mouse.OnMouseDown_Right = false;
    }

    if (!MousePacketReady)
        return;

    Mouse.x_old = Mouse.x;
    Mouse.y_old = Mouse.y;
    if (!Mouse.Sensitivity)
        Mouse.Sensitivity = 2;

    bool xNegative, yNegative, xOverflow, yOverflow;

    if (MousePacket[0] & PS2XSign)
    {
        xNegative = true;
    }
    else
    {
        xNegative = false;
    }

    if (MousePacket[0] & PS2YSign)
    {
        yNegative = true;
    }
    else
    {
        yNegative = false;
    }

    if (MousePacket[0] & PS2XOverflow)
    {
        xOverflow = true;
    }
    else
    {
        xOverflow = false;
    }

    if (MousePacket[0] & PS2YOverflow)
    {
        yOverflow = true;
    }
    else
    {
        yOverflow = false;
    }

    if (!xNegative)
    {
        Mouse.x += MousePacket[1] * Mouse.Sensitivity;
        if (xOverflow)
            Mouse.x += 255;
        Mouse.OnMouseMove = true;
    }
    else
    {
        MousePacket[1] = (255 + 1) - MousePacket[1];
        Mouse.x -= MousePacket[1] * Mouse.Sensitivity;
        if (xOverflow)
            Mouse.x -= 255;
        Mouse.OnMouseMove = true;
    }

    if (!yNegative)
    {
        Mouse.y -= MousePacket[2] * Mouse.Sensitivity;
        if (yOverflow)
            Mouse.y -= 255;
        Mouse.OnMouseMove = true;
    }
    else
    {
        MousePacket[2] = (255 + 1) - MousePacket[2];
        Mouse.y += MousePacket[2] * Mouse.Sensitivity;
        if (yOverflow)
            Mouse.y += 255;
        Mouse.OnMouseMove = true;
    }

    /*if (Mouse.x != Mouse.x_old || Mouse.y != Mouse.y_old || Mouse.x != Mouse.x || Mouse.y != Mouse.y)
    {
        Mouse.OnMouseMove = true;
    }
    else
    {
        Mouse.OnMouseMove = false;
    }*/
    MouseCheckPos();
    MousePacketReady = false;
    Mouse.OnMouseMove = false;
}

__attribute__((interrupt)) void HandlePS2Mouse(int_frame_t *r)
{
    unsigned char mousedata = inb(0x60);
    PS2MouseData(mousedata);
    mouse_keyboard = "mouse";
    PIC_End();
}

void MouseCheckPos()
{
    if (Mouse.x < -1)
        Mouse.x = -1;
    if (Mouse.x >= (long)vga_getresolution(1))
        Mouse.x = (long)vga_getresolution(1) - 1;
    if (Mouse.y < -1)
        Mouse.y = -1;
    if (Mouse.y >= (long)vga_getresolution(2))
        Mouse.y = (long)vga_getresolution(2) - 1;
}

void InitPS2Mouse()
{
    unsigned char status;
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
}