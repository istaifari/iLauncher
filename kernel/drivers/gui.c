#include "../kernel.h"

void gui_window_paint(Window* obj)
{
    if (!obj->isSetup)
    {
        obj->isSetup = true;
        obj->x = 0;
        obj->y = 0;
        obj->width = 128;
        obj->height = 96;
        obj->visible = true;
    }
    if (obj->visible)
    {
        vga_fillRect((unsigned)obj->x, (unsigned)obj->y, (unsigned)obj->width, (unsigned)obj->height+13, 0x3f);
        vga_fillRect((unsigned)obj->x, (unsigned)obj->y+13, (unsigned)obj->width, (unsigned)obj->height, 0x02);

        vga_fillRect((unsigned)obj->x-16+obj->width, (unsigned)obj->y+1, 15, 11, 0x04);
        vga_drawtext("x", 0x3f, (unsigned)obj->x+obj->width-12, (unsigned)obj->y+2, font_8x8, 8, 8);

        vga_drawtext("Hello World", 0x3f, (unsigned)obj->x+2, (unsigned)obj->y+13+2, font_8x8, 8, 8);
    }
}