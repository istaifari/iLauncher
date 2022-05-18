#pragma once
#include "../kernel.h"

typedef struct Window
{
    bool isSetup;
    int id;
    long x;
    long y;
    long width;
    long height;
    long top;
    long left_right;
    long bottom;
    long buttons_width;
    bool visible;
    int* z_order;
    int* tree;
    bool OnMouseDown_Left;
    bool OnMouseDown_Middle;
    bool OnMouseDown_Right;
} __attribute__((packed)) Window;

void gui_window_paint(Window* obj);