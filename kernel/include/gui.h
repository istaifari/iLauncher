#pragma once
#include <kernel.h>

typedef struct Window
{
    bool isSetup;
    long x;
    long y;
    long width;
    long height;
    long top;
    long left_right;
    long bottom;
    long buttons_width;
    bool visible;
    bool OnMouseDown_Left;
    bool OnMouseDown_Middle;
    bool OnMouseDown_Right;
} __attribute__((packed)) Window;

typedef struct WindowInfo
{
    char key;
    long x;
    long y;
    long width;
    long height;
    bool OnMouseDown_Left;
    bool OnMouseDown_Middle;
    bool OnMouseDown_Right;
} __attribute__((packed)) WindowInfo;

typedef void (*key_handle_t)(WindowInfo);

void gui_window_scancode(Window *obj, int8_t *keys, key_handle_t kfn, char keycode);
void gui_window_paint(Window *obj);
void gui_window_draw(Window *obj, long x, long y, long c);