#include "../kernel.h"
#include "../include/vga.h"
#include "../include/keyboard.h"

int vga_elem_length = 0;
int vga_update_stat = 1;
#define vga_buf_resolution 320 * 200

// arrays

char vga_ch[vga_buf_resolution];
char vga_color[vga_buf_resolution];
int vga_x[vga_buf_resolution];
int vga_y[vga_buf_resolution];

// read data from pin
uint8_t gfx_port_byte_read(uint16_t port)
{
	uint8_t result;
	__asm__ volatile("inb %1, %0"
					 : "=a"(result)
					 : "Nd"(port));
	return result;
}

// write data to pin
void gfx_port_byte_write(uint16_t port, uint8_t data)
{
	__asm__ volatile("outb %0, %1"
					 :
					 : "a"(data), "Nd"(port));
}

void vga_add(char ch, char color, int x, int y)
{
	int x_pos = x;
	if (x_pos < 0)
		x_pos = 0;
	if (x_pos >= 80)
		x_pos = 79;

	int y_pos = y;
	if (y_pos < 0)
		y_pos = 0;
	if (y_pos >= 25)
		y_pos = 24;
	vga_update_stat--;
	vga_ch[vga_elem_length] = ch;
	vga_color[vga_elem_length] = color;
	vga_x[vga_elem_length] = x_pos;
	vga_y[vga_elem_length] = y_pos;
	vga_elem_length++;
	vga_update_stat++;
}

void vga_addUpdate(int id, char ch, char color, int x, int y)
{
	vga_update_stat--;
	vga_ch[id] = ch;
	vga_color[id] = color;
	vga_x[id] = x;
	vga_y[id] = y;
	vga_update_stat++;
}

void vga_addRemove(int id)
{
	vga_update_stat--;
	char ch;
	char color = 0x00;
	int x = 0;
	int y = 0;

	vga_ch[id] = ch;
	vga_color[id] = color;
	vga_x[id] = x;
	vga_y[id] = y;
	vga_elem_length--;
	vga_update_stat++;
}

void vga_draw(char ch, char color, int x, int y)
{
	int x_pos = x;
	if (x_pos < -1)
		x_pos = -1;
	if (x_pos >= 80)
		x_pos = 80;

	int y_pos = y;
	if (y_pos < -1)
		y_pos = 0;
	if (y_pos >= 25)
		y_pos = 25;

	/* update screen and draw */
	char *screen = (char *)VIDEO_MEMORY;
	for (int i = 0; i < 80 * 25; i++)
	{
		char *temp = (char *)VIDEO_MEMORY;
		temp += +(80 * y_pos + x_pos) * 2;
		*temp++ = ch;
		*temp = color;
	}
}

void vga_update()
{
	for (int i = 0; i < vga_elem_length; i++)
	{
		vga_draw(vga_ch[i], vga_color[i], vga_x[i], vga_y[i]);
		vga_draw(' ', 0x00, vga_x[i] - 1, vga_y[i]);
		vga_draw(' ', 0x00, vga_x[i] + 1, vga_y[i]);
		vga_draw(' ', 0x00, vga_x[i], vga_y[i] - 1);
		vga_draw(' ', 0x00, vga_x[i], vga_y[i] + 1);
	}
}

void vga_start()
{
	gfx_mode = true;
	int icount = 0;
	int count_x = 0;
	int count_y = 0;
	for (int i = 0; i < 4000; i++)
	{
		vga_draw(' ', 0x00, i, 0);
		vga_draw(' ', 0x00, count_x, count_y++);
		icount++;
		if (icount > 25 - 1 && count_y > 25 - 1)
		{
			icount = 0;
			count_y = 0;
			count_x++;
			if (count_x > 80 - 1)
			{
				count_x = 0;
			}
		}
	}
	extern bool line_inf;
	line_inf = false;
}