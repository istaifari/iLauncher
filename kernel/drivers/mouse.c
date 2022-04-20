#include "../kernel.h"
#include "../include/vga.h"
#include "../include/mouse.h"

uint8_t buffer_crs[3];
uint8_t offset_crs;
uint8_t buttons_crs;

void mouse_start()
{
    uint8_t status = gfx_port_byte_read(0x20);
    int8_t x = 40;
    int8_t y = 12;
    offset_crs = (offset_crs + 1) % 3;
    buffer_crs[offset_crs] = gfx_port_byte_read(0x20);
    if (offset_crs == 0)
    {
        x += buffer_crs[1];
        if (x < 0)
            x = 0;
        if (x >= 80)
            x = 79;
        y -= buffer_crs[2];
        if (y < 0)
            y = 0;
        if (x >= 25)
            x = 24;
    }
}

void mouse_setup()
{
    offset_crs = 0;
    buttons_crs = 0;
    gfx_port_byte_write(0xA8, 0xA8);
    gfx_port_byte_write(0x20, 0x20);
    uint8_t status = gfx_port_byte_read(0x20) | 2;
    gfx_port_byte_write(0x60, 0x60);
    gfx_port_byte_write(status, status);
    gfx_port_byte_write(0xD4, 0xD4);
    gfx_port_byte_write(0xF4, 0xF4);
    gfx_port_byte_read(0xF4);

    int i_x = 80 / 2;
    int i_y = 25 / 2;
    vga_add(' ', 0xff, i_x, i_y);
    vga_add(' ', 0xff, 0, 0);
    /*vga_add(' ', 0xff, 1, 0);
    vga_add(' ', 0xff, 0, 1);
    vga_add(' ', 0xff, 1, 1);*/
    vga_add(' ', 0xff, 80, 25);
    for (;;)
    {
        char keyI1 = get_key();
        if (keyI1 == 37)
        {
            if (i_x > 0)
                i_x--;
        }
        if (keyI1 == 39)
        {
            if (i_x < 79)
                i_x++;
        }
        if (keyI1 == 38)
        {
            if (i_y > 0)
                i_y--;
        }
        if (keyI1 == 40)
        {
            if (i_y < 24)
                i_y++;
        }
        if (keyI1 == 35)
        {
            _reboot();
        }
        vga_addUpdate(0, ' ', 0xff, i_x, i_y);
        vga_update();
        /*if (gfx_port_byte_read(0x20))
            mouse_start();*/
    }
}