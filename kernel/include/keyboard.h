#pragma once
#include "../kernel.h"

typedef struct keyboard
{
  char ScanCode;
  long OnKeyDown;
  bool isEnterPressed;
  bool isLeftShiftPressed;
  bool isRightShiftPressed;
  unsigned long keymap_size;
  char base[512];
  char shift[512];
} keyboard_t;

extern keyboard_t *default_keyboard;

void InitPS2Keyboard();
char GetKey();