#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../include/vga.h"
#include "../include/keyboard.h"
#include "../include/interrupts.h"

void vga_clear();
void vga_start();
void vga_drawimage(int x, int y, int w, int h, unsigned pixels[]);
void vga_drawtext(char *text, unsigned color, unsigned x, unsigned y);
void vga_fillRect(unsigned x, unsigned y, unsigned w, unsigned h, unsigned c);
unsigned vga_getresolution(int select);