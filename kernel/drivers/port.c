#include "../include/port.h"

unsigned char port_byte_read(unsigned short port)
{
	unsigned char result;
	__asm__ volatile("inb %1, %0"
					 : "=a"(result)
					 : "Nd"(port));
	return result;
}

void port_byte_write(unsigned short port, unsigned char data)
{
	__asm__ volatile("outb %0, %1"
					 :
					 : "a"(data), "Nd"(port));
}

void port_byte_wait()
{
	__asm__ volatile("outb %%al, $0x80"
					 :
					 : "a"(0));
}

unsigned char inb(unsigned short port)
{
	unsigned char result;
	__asm__ volatile("inb %1, %0"
					 : "=a"(result)
					 : "Nd"(port));
	return result;
}

void outb(unsigned short port, unsigned char data)
{
	__asm__ volatile("outb %0, %1"
					 :
					 : "a"(data), "Nd"(port));
}

void io_wait()
{
	__asm__ volatile("outb %%al, $0x80"
					 :
					 : "a"(0));
}