#pragma once
#include <kernel.h>

#define CHAR_PER_LINE80
#define VIDEO_MEMORY LOAD_MEMORY_ADDRESS+0xb8000
#define VIDEO_CONTROL_PIN 0x3D4
#define VIDEO_DATA_PIN 0x3D5

int get_cursor_pos();
void set_cursor_pos(int x,int y);
void printtext(char*,char,char);
void printchar(char,char,char);
void scroll();
void rm_char_in_pos(int);
void clear_screen();
void next_line();  