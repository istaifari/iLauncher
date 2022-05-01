#include "../include/mouse.h"
#include "../include/interrupts.h"

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000010
#define PS2Rightbutton 0b00000100
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

uint8_t buffer_crs[3];
uint8_t offset_crs;
uint8_t buttons_crs;

uint8_t MousePointer[] = {
    0b11111111,
    0b11100000,
    0b11111111,
    0b10000000,
    0b11111110,
    0b00000000,
    0b11111100,
    0b00000000,
    0b11111000,
    0b00000000,
    0b11110000,
    0b00000000,
    0b11100000,
    0b00000000,
    0b11000000,
    0b00000000,
    0b11000000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
};

void MouseWait()
{
    uint64_t timeout = 100000;
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
    uint64_t timeout = 100000;
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
bool MousePacketReady = false;
long int MousePositionX = 80 / 2;
long int MousePositionY = 25 / 2;
long int MousePositionOldX = 80 / 2;
long int MousePositionOldY = 25 / 2;
int MouseSensitivity = 2;

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
    if (!MousePacketReady)
        return;

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
        MousePositionX += MousePacket[1];
        if (xOverflow)
        {
            MousePositionX += MouseSensitivity;
        }
    }
    else
    {
        MousePacket[1] = (MouseSensitivity + 1) - MousePacket[1];
        MousePositionX -= MousePacket[1];
        if (xOverflow)
        {
            MousePositionX -= MouseSensitivity;
        }
    }

    if (!yNegative)
    {
        MousePositionY -= MousePacket[2];
        if (yOverflow)
        {
            MousePositionY -= MouseSensitivity;
        }
    }
    else
    {
        MousePacket[2] = (MouseSensitivity + 1) - MousePacket[2];
        MousePositionY += MousePacket[2];
        if (yOverflow)
        {
            MousePositionY += MouseSensitivity;
        }
    }

    if (MousePositionX < 0)
        MousePositionX = 0;
    if (MousePositionX >= 80 - 1)
        MousePositionX = 79;
    if (MousePositionY < 0)
        MousePositionY = 0;
    if (MousePositionY >= 25 - 1)
        MousePositionY = 24;
    MousePacketReady = false;
    MousePositionOldX = MousePositionX;
    MousePositionOldY = MousePositionY;
}

void InitPS2Mouse()
{
    port_byte_write(0x64, 0xA8); // enabling the auxiliary device - mouse
    MouseWait();
    port_byte_write(0x64, 0x20); // tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
    uint8_t status = port_byte_read(0x60);
    status |= 0b10;
    MouseWait();
    port_byte_write(0x64, 0x60);
    MouseWait();
    port_byte_write(0x60, status); // setting the correct bit is the "compaq" status byte
    MouseWrite(0xF6);
    MouseRead();
    MouseWrite(0xF4);
    MouseRead();
}

int GetMousePos(int offset_pos)
{
    if (offset_pos == 0)
        return MousePositionX;
    else if (offset_pos == 1)
        return MousePositionY;
    else
        return 0;
}

void MouseSetup()
{
    InitPS2Mouse();
    vga_add(' ', 0x00, 0, 0);
    while (true)
    {
        /*if (KeyboardInterrupt() == 35)
        {
            _reboot();
        }*/
        ProcessMousePacket();
        if ((port_byte_read(0x64) & 0b10) == 0 && port_byte_read(0x64) & 0b1)
        {
            MouseInterrupt();
        }
        vga_addUpdate(0, '^', 0x0f, GetMousePos(0), GetMousePos(1));
        vga_update();
    }
}