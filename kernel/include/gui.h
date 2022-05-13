#pragma once
#include "../kernel.h"

typedef struct Window
{
    bool isSetup;
    long x;
    long y;
    long width;
    long height;
    bool visible;
    bool OnMouseDown_Left;
    bool OnMouseDown_Middle;
    bool OnMouseDown_Right;
} __attribute__((packed)) Window;

void gui_window_paint(Window* obj);