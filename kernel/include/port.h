#pragma once
#include <stddef.h>
#include <stdint.h>

unsigned char port_byte_read(unsigned short port);
void port_byte_write(unsigned short port, unsigned char data);
void port_byte_wait();
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
void io_wait();