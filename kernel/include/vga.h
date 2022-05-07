#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../include/vga.h"
#include "../include/keyboard.h"
#include "../include/interrupts.h"

uint8_t port_byte_read(uint16_t port);
void port_byte_write(uint16_t port, uint8_t data);
void vga_start();
void vga_drawimage(int x, int y, int w, int h, unsigned pixels[]);
void vga_fillRect(unsigned x, unsigned y, unsigned w, unsigned h, unsigned c);
long int vga_getresolution(int select);
void vga_add(char ch, char color, long x, long y);
void vga_addUpdate(int id, char ch, char color, long x, long y);
void vga_addRemove(int id);
void vga_update();