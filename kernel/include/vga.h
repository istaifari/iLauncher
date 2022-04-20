#pragma once
#include <stddef.h>
#include <stdint.h>

uint8_t gfx_port_byte_read(uint16_t port);
void gfx_port_byte_write(uint16_t port, uint8_t data);
void vga_start();
void vga_draw(char ch, char color, int x, int y);
void vga_add(char ch, char color, int x, int y);
void vga_addUpdate(int id, char ch, char color, int x, int y);
void vga_addRemove(int id);
void vga_update();