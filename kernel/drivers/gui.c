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
        obj->height = 128;
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
        Mouse.OnMouseDown_Right = false;
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
        vga_drawtext("x", (unsigned)obj->x + obj->width - 11 + obj->left_right, (unsigned)obj->y + obj->top - 11, 0x3f, font_8x8, 8, 8);
    }
}

void gui_window_scancode(Window *obj, int8_t *keys, key_handle_t kfn, char keycode)
{
    if (!obj->visible)
        return;
    WindowInfo info = {
        .key = 0,
        .x = obj->x,
        .y = obj->y,
        .width = obj->width,
        .height = obj->height,
        .OnMouseDown_Left = obj->OnMouseDown_Left,
        .OnMouseDown_Middle = obj->OnMouseDown_Middle,
        .OnMouseDown_Right = obj->OnMouseDown_Right,
    };
    for (long i = 0; i < strlen(keys); i++)
    {
        info.key = keys[i];
        if (keycode == keys[i])
            kfn(info);
    }
}

void gui_window_draw(Window *obj, long x, long y, long c)
{
    if (!obj->visible)
        return;
    long offsetX = obj->x + obj->left_right + x;
    long offsetY = obj->y + obj->top + y;
    if (offsetX < (obj->x + obj->left_right))
        return;
    if (offsetY < (obj->y + obj->top))
        return;
    if (offsetX > obj->width + (obj->x + obj->left_right) - 1)
        return;
    if (offsetY > obj->height + (obj->y + obj->top) - 1)
        return;
    vga_putpixel(offsetX, offsetY, c);
}