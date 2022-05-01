#pragma once
#include <stddef.h>
#include <stdint.h>

uint8_t port_byte_read(uint16_t port);
void port_byte_write(uint16_t port, uint8_t data);
void vga_start();
void vga_draw(char ch, char color, long x, long y);
void vga_add(char ch, char color, long x, long y);
void vga_addUpdate(int id, char ch, char color, long x, long y);
void vga_addRemove(int id);
void vga_update();