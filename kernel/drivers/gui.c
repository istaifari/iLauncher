#include "../kernel.h"

bool gui_window_colide(long x1, long y1, long w1, long h1, long x2, long y2, long w2, long h2)
{
    if (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

long posx = 0, posy = 0;

void gui_window_update(Window *obj)
{
    if (!obj->isSetup)
    {
        obj->isSetup = true;
        obj->width = 128;
        obj->height = 96;
        obj->x = (vga_getresolution(1) - obj->width) / 2;
        obj->y = (vga_getresolution(2) - obj->height) / 2;
        obj->top = 22;
        obj->left_right = 8;
        obj->bottom = 8;
        obj->buttons_width = 18 * 1;
        obj->visible = true;
    }
    if (Mouse.OnMouseDown_Left)
    {
        /* Mouse Left */
        if (obj->visible)
        {
            if (gui_window_colide(obj->x - 15 + obj->width + obj->left_right, obj->y + obj->top - 12, 15, 11, Mouse.x, Mouse.y, 1, 1))
            {
                Mouse.OnMouseDown_Left = !Mouse.OnMouseDown_Left;
                _reboot();
            }
            if (gui_window_colide(obj->x + obj->left_right, obj->y + obj->bottom, obj->width - obj->buttons_width, obj->top - 6, Mouse.x, Mouse.y, 1, 1))
            {
                obj->x += -(posx - Mouse.x);
                obj->y += -(posy - Mouse.y);
            }
            else
            {
                if (gui_window_colide(obj->x + obj->left_right, obj->y, obj->width, 6, Mouse.x, Mouse.y, 1, 1))
                {
                    obj->y += -(posy - Mouse.y);
                    obj->height -= -(posy - Mouse.y);
                }
                else if (gui_window_colide(obj->x, obj->y + obj->top, obj->left_right, obj->height, Mouse.x, Mouse.y, 1, 1))
                {
                    obj->x += -(posx - Mouse.x);
                    obj->width -= -(posx - Mouse.x);
                }
                else if (gui_window_colide(obj->x + obj->left_right, obj->y + obj->height + obj->top, obj->width, 6, Mouse.x, Mouse.y, 1, 1))
                {
                    obj->height += -(posy - Mouse.y);
                }
                else if (gui_window_colide(obj->x + obj->width + obj->left_right, obj->y + obj->top, obj->left_right, obj->height, Mouse.x, Mouse.y, 1, 1))
                {
                    obj->width += -(posx - Mouse.x);
                }
            }
        }
    }
    else if (Mouse.OnMouseDown_Middle)
    {
        /* Mouse Middle */
    }
    else if (Mouse.OnMouseDown_Right)
    {
        /* Mouse Right */
        if (obj->visible)
            obj->visible = false;
        else
            obj->visible = true;
    }
    posx = Mouse.x;
    posy = Mouse.y;
}

void gui_window_paint(Window *obj)
{
    if (obj->visible)
    {
        vga_fillRect((unsigned)obj->x, (unsigned)obj->y, (unsigned)obj->width + obj->left_right + obj->left_right, (unsigned)obj->height + obj->top + obj->bottom, 0x3f);
        vga_fillRect((unsigned)obj->x + obj->left_right, (unsigned)obj->y + obj->top, (unsigned)obj->width, (unsigned)obj->height, 0x00);

        vga_fillRect((unsigned)obj->x - 15 + obj->width + obj->left_right, (unsigned)obj->y + obj->top - 12, 15, 11, 0x04);
        vga_drawtext("x", 0x3f, (unsigned)obj->x + obj->width - 11 + obj->left_right, (unsigned)obj->y + obj->top - 11, font_8x8, 8, 8);
    }
}

unsigned color = 0x3f;

void gui_window_draw(Window *obj)
{
    if (!obj->visible)
        return;
    long offsetX = obj->x + obj->left_right;
    long offsetY = obj->y + obj->top;
    vga_drawtext("Hello World!", color, offsetX + 2, offsetY + 2, font_8x8, 8, 8);
    color = rand(0x01,0x3f);
}