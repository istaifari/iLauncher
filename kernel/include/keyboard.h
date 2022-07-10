#pragma once
#include <kernel.h>

typedef struct keyboard
{
  char ScanCode;
  long OnKeyDown;
  bool isLeftShiftPressed;
  bool isRightShiftPressed;
  unsigned long keymap_size;
  char base[512];
  char shift[512];
} keyboard_t;

typedef struct keyboard_keybuffer
{
  char *buffer;
  size_t size;
} keyboard_keybuffer_t;

extern keyboard_t *keyboard_ps2;

keyboard_t *keyboard_info_setup_layout(keyboard_t keyboard);
keyboard_keybuffer_t *keyboard_keybuffer_setup(size_t size);
void *keyboard_keybuffer_free(keyboard_keybuffer_t *keybuffer);
void *keyboard_keybuffer_read(keyboard_keybuffer_t *keybuffer);
void *keyboard_keybuffer_scan(keyboard_keybuffer_t *keybuffer);
void *keyboard_keybuffer_clear_characters(keyboard_keybuffer_t *keybuffer);
void InitPS2Keyboard();
void ResetPS2Keyboard();
char keyboard_get_key();