#include <kernel.h>

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
    if (mouse_ps2->OnMouseDown_Left)
    {
        /* mouse_ps2 Left */
        if (obj->visible)
        {
            if (gui_window_colide(obj->x - 15 + obj->width + obj->left_right, obj->y + obj->top - 12, 15, 11, mouse_ps2->x, mouse_ps2->y, 1, 1))
            {
                mouse_ps2->OnMouseDown_Left = !mouse_ps2->OnMouseDown_Left;
                poweroff();
                reboot();
            }
            if (gui_window_colide(obj->x + obj->left_right, obj->y + obj->bottom, obj->width - obj->buttons_width, obj->top - 6, mouse_ps2->x, mouse_ps2->y, 1, 1))
            {
                obj->x += -(posx - mouse_ps2->x);
                obj->y += -(posy - mouse_ps2->y);
            }
            else
            {
                if (gui_window_colide(obj->x + obj->left_right, obj->y, obj->width, 6, mouse_ps2->x, mouse_ps2->y, 1, 1))
                {
                    obj->y += -(posy - mouse_ps2->y);
                    obj->height -= -(posy - mouse_ps2->y);
                }
                else if (gui_window_colide(obj->x, obj->y + obj->top, obj->left_right, obj->height, mouse_ps2->x, mouse_ps2->y, 1, 1))
                {
                    obj->x += -(posx - mouse_ps2->x);
                    obj->width -= -(posx - mouse_ps2->x);
                }
                else if (gui_window_colide(obj->x + obj->left_right, obj->y + obj->height + obj->top, obj->width, 6, mouse_ps2->x, mouse_ps2->y, 1, 1))
                {
                    obj->height += -(posy - mouse_ps2->y);
                }
                else if (gui_window_colide(obj->x + obj->width + obj->left_right, obj->y + obj->top, obj->left_right, obj->height, mouse_ps2->x, mouse_ps2->y, 1, 1))
                {
                    obj->width += -(posx - mouse_ps2->x);
                }
            }
        }
    }
    else if (mouse_ps2->OnMouseDown_Middle)
    {
        /* mouse_ps2 Middle */
    }
    else if (mouse_ps2->OnMouseDown_Right)
    {
        /* mouse_ps2 Right */
        mouse_ps2->OnMouseDown_Right = false;
        if (obj->visible)
            obj->visible = false;
        else
            obj->visible = true;
    }
    posx = mouse_ps2->x;
    posy = mouse_ps2->y;
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