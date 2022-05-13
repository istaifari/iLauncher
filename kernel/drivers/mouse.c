#include "../include/mouse.h"

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000100
#define PS2Rightbutton 0b00000010
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

void MouseWait()
{
    long timeout = 100000;
    while (timeout--)
    {
        if ((port_byte_read(0x64) & 0b10) == 0)
        {
            return;
        }
    }
}

void MouseWaitInput()
{
    long timeout = 100000;
    while (timeout--)
    {
        if (port_byte_read(0x64) & 0b1)
        {
            return;
        }
    }
}

void MouseWrite(uint8_t value)
{
    MouseWait();
    port_byte_write(0x64, 0xD4);
    MouseWait();
    port_byte_write(0x60, value);
}

uint8_t MouseRead()
{
    MouseWaitInput();
    return port_byte_read(0x60);
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
MouseInfo Mouse;
bool MousePacketReady = false;

void ProcessMousePacket();

void HandlePS2Mouse(uint8_t data)
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
}

void ProcessMousePacket()
{
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
    Mouse.Sensitivity = 255;

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
        Mouse.x += MousePacket[1];
        if (xOverflow)
            Mouse.x += Mouse.Sensitivity;
    }
    else
    {
        MousePacket[1] = (Mouse.Sensitivity + 1) - MousePacket[1];
        Mouse.x -= MousePacket[1];
        if (xOverflow)
            Mouse.x -= Mouse.Sensitivity;
    }

    if (!yNegative)
    {
        Mouse.y -= MousePacket[2];
        if (yOverflow)
            Mouse.y -= Mouse.Sensitivity;
    }
    else
    {
        MousePacket[2] = (Mouse.Sensitivity + 1) - MousePacket[2];
        Mouse.y += MousePacket[2];
        if (yOverflow)
            Mouse.y += Mouse.Sensitivity;
    }

    /*if (Mouse.x != Mouse.x_old || Mouse.y != Mouse.y_old || Mouse.x != Mouse.x || Mouse.y != Mouse.y)
    {
        Mouse.OnMouseMove = true;
    }
    else
    {
        Mouse.OnMouseMove = false;
    }*/
    if (Mouse.x < 0)
        Mouse.x = 0;
    if (Mouse.x >= vga_getresolution(1))
        Mouse.x = vga_getresolution(1) - 1;
    if (Mouse.y < 0)
        Mouse.y = 0;
    if (Mouse.y >= vga_getresolution(2))
        Mouse.y = vga_getresolution(2) - 1;
    MousePacketReady = false;
}

void InitPS2Mouse()
{
    port_byte_write(0x64, 0xA8);
    MouseWait();
    port_byte_write(0x64, 0x20);
    MouseWaitInput();
    uint8_t status = port_byte_read(0x60);
    status |= 0b10;
    MouseWait();
    port_byte_write(0x64, 0x60);
    MouseWait();
    port_byte_write(0x60, status);
    MouseWrite(0xF6);
    MouseRead();
    MouseWrite(0xF4);
    MouseRead();
    /*MouseWait();
    outportb( 0x60, 0xF4 );
    CheckPort();
    CheckMouse();*/
}

long int GetMouseInfo(int offset_pos)
{
    if (offset_pos == 0)
        return Mouse.x;
    else if (offset_pos == 1)
        return Mouse.y;
    else
        return 0;
}

void MouseSetup()
{
    InitPS2Mouse();
}