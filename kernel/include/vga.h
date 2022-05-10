#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../kernel.h"

void vga_clear();
void vga_start();
void vga_drawimage(int x, int y, int w, int h, unsigned pixels[]);
void vga_drawtext(char *text, unsigned color, unsigned x, unsigned y, unsigned char font[], unsigned font_w, unsigned font_h);
void vga_WriteResolutionRegister(unsigned char *regs);
void vga_fillRect(unsigned x, unsigned y, unsigned w, unsigned h, unsigned c);
unsigned vga_getresolution(int select);