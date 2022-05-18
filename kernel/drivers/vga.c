#include "../include/vga.h"

#define VGA_AC_INDEX 0x3C0
#define VGA_AC_WRITE 0x3C0
#define VGA_AC_READ 0x3C1
#define VGA_MISC_WRITE 0x3C2
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5
#define VGA_DAC_READ_INDEX 0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA 0x3C9
#define VGA_MISC_READ 0x3CC
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF
#define VGA_CRTC_INDEX 0x3D4
#define VGA_CRTC_DATA 0x3D5
#define VGA_INSTAT_READ 0x3DA
#define VGA_NUM_SEQ_REGS 5
#define VGA_NUM_CRTC_REGS 25
#define VGA_NUM_GC_REGS 9
#define VGA_NUM_AC_REGS 21
#define VGA_NUM_REGS (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)
/*#define MAKE_VGA_MODE(w, h, depth) ((((depth & 0xff) << 24) | ((w & 0xfff) << 12) | (h & 0xfff)))
#define VGA_MODE_WIDTH(m) (((unsigned)m >> 12) & 0xfff)
#define VGA_MODE_HEIGHT(m) ((unsigned)m & 0xfff)
#define VGA_MODE_DEPTH(m) (((unsigned)m >> 24) & 0xff)*/

unsigned vga_width, vga_height, vga_bpp, vga_depth, vga_pitch, vga_bufsize;

unsigned char *vga_memory;
unsigned char *vga_backbuffer;

static void (*vga_putpixel)(long x, long y, unsigned c);

static unsigned get_fb_seg(void)
{
	unsigned seg;
	outb(VGA_GC_INDEX, 0x06);
	seg = ((inb(VGA_GC_DATA) >> 2) & 3);
	switch (seg)
	{
	case 1:
		return 0xa0000;
	case 2:
		return 0xb0000;
	case 3:
		return 0xb8000;
	default:
		return 0x00000;
	}
}

static void set_plane(long p)
{
	unsigned char pmask;
	p &= 3;
	pmask = 1 << p;
	outb(VGA_GC_INDEX, 4);
	outb(VGA_GC_DATA, p);
	outb(VGA_SEQ_INDEX, 2);
	outb(VGA_SEQ_DATA, pmask);
}

static void putpixel8(long x, long y, unsigned c)
{
	if (x < 0 || x >= vga_width || y < 0 || y >= vga_height)
		return;
	long pos, mask, pmask;
	if (vga_bpp == 8)
	{
		pos = y * vga_width + x;
		vga_backbuffer[pos] = c;
	}
	else if (vga_bpp == 16)
	{
		pos = y * (vga_width / 8) + x / 8;
		x = (x & 7) * 1;
		mask = 0x80 >> x;
		pmask = 1;
		vga_backbuffer[pos] = (c & mask) | vga_backbuffer[pos];
	}
}

void vga_drawimage(long x, long y, long w, long h, const unsigned *pixels)
{
	long i = 0, l = 0, j = 0;
	for (l = j = 0; l < h; l++)
	{
		for (i = 0; i < w; i++, j++)
		{
			if (pixels[j] != 0x40)
			{
				vga_putpixel(x + i, y + l, pixels[j]);
			}
		}
	}
}

void vga_fillRect(long x, long y, long w, long h, unsigned c)
{
	long i = 0, l = 0;
	for (l = 0; l < h; l++)
	{
		for (i = 0; i < w; i++)
		{
			vga_putpixel(x + i, y + l, c);
		}
	}
}

void vga_drawchar(char chr, long x, long y, unsigned c, unsigned char font[], long font_w, long font_h, bool flip)
{
	int cx, cy;
	int mask[8];
	if (flip)
	{
		mask[0] = 1;
		mask[1] = 2;
		mask[2] = 4;
		mask[3] = 8;
		mask[4] = 16;
		mask[5] = 32;
		mask[6] = 64;
		mask[7] = 128;
	}
	else
	{
		mask[7] = 1;
		mask[6] = 2;
		mask[5] = 4;
		mask[4] = 8;
		mask[3] = 16;
		mask[2] = 32;
		mask[1] = 64;
		mask[0] = 128;
	}
	unsigned char *gylph = font + (int)chr * font_h;

	for (cy = 0; cy < font_h; cy++)
	{
		for (cx = 0; cx < font_w; cx++)
		{
			if (gylph[cy] & mask[cx])
				vga_putpixel(x + cx, y + cy, c);
		}
	}
}

void vga_drawtext(char *text, unsigned color, long x, long y, unsigned char font[], long font_w, long font_h)
{
	long i, x_pos, y_pos, col, row, stop;
	char chr;

	long len = strlen(text);
	long font_width = font_w, font_height = font_h;
	row = 0;
	col = 0;
	stop = 0;
	for (i = 0; i < len; i++)
	{
		switch (text[i])
		{
		case '\n':
			row++;
			col = 0;
			chr = 0;
			break;

		case '\r':
			chr = 0;
			break;

		case '\t':
			chr = 0;
			col += 4 - col % 4;
			break;

		case '\0':
			stop = 1;
			chr = 0;
			break;

		default:
			col++;
			chr = text[i];
			break;
		}

		if (stop)
		{
			break;
		}

		if (((col - 1) * font_width) >= vga_width)
		{
			row++;
			col = 0;
			chr = 0;
		}

		vga_drawchar(chr, ((col - 1) * font_width) + x, (row * font_height) + y, color, font, font_w, font_h, false);
	}
}

unsigned vga_getresolution(int select)
{
	if (select == 1)
	{
		return vga_width;
	}
	else if (select == 2)
	{
		return vga_height;
	}
	else
	{
		return 0;
	}
}

void vga_swapbuffers()
{
	memcpy(vga_memory, vga_backbuffer, vga_bufsize);
}

void vga_setupFramebuffer(unsigned ScreenWidth, unsigned ScreenHeight, unsigned BitsPerPixel)
{
	vga_width = ScreenWidth;
	vga_height = ScreenHeight;
	vga_bpp = BitsPerPixel;
	vga_depth = (vga_bpp | 7) >> 3;
	vga_pitch = vga_width * vga_depth;
	vga_bufsize = vga_pitch * vga_height;
	vga_memory = (unsigned char *)get_fb_seg();
	vga_backbuffer = (unsigned char *)vga_bufsize;
	return;
}

void vga_WriteResolutionRegister(unsigned char *regs, unsigned w, unsigned h, unsigned c)
{
	unsigned i;
	outb(VGA_MISC_WRITE, *regs);
	regs++;
	for (i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, *regs);
		regs++;
	}
	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
	for (i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, *regs);
		regs++;
	}
	for (i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, *regs);
		regs++;
	}
	for (i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, *regs);
		regs++;
	}
	(void)inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
	vga_setupFramebuffer(w, h, c);
	vga_clear(0x00);
}

void vga_clear(unsigned c)
{
	memset(vga_backbuffer, c, vga_bufsize);
	/*unsigned x, y;
	for (y = 0; y < vga_height; y++)
	{
		for (x = 0; x < vga_width; x++)
		{
			// memset(vga_backbuffer, 0x00, vga_height * vga_pitch);
			vga_putpixel(x, y, 0x00);
			// SwapBuffers();
		}
	}*/
}

void vga_start()
{
	char *resolution = "320x200x8";
	if (resolution == "320x200x8")
	{
		vga_putpixel = putpixel8;
		vga_WriteResolutionRegister(g_320x200x8, 320, 200, 8);
	}
	/*else if (resolution == "320x200x4")
	{
		vga_WriteResolutionRegister(g_320x200x4);
		vga_width = 320;
		vga_height = 200;
		vga_putpixel = putpixel2;
		vga_clear();
	}*/
	else if (resolution == "640x480x16")
	{
		vga_putpixel = putpixel8;
		vga_WriteResolutionRegister(g_640x480x16, 640, 480, 16);
	}
	/*else if (resolution == "720x480x16")
	{
		vga_putpixel = putpixel4p;
		vga_WriteResolutionRegister(g_720x480x16, 720, 480, 16);
	}*/
	vga_update();
	gfx_mode = true;
	/*int icount = 0;
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
	}*/
	extern bool line_inf;
	line_inf = false;
}

void vga_update()
{
	Mouse.width = 24;
	Mouse.height = 32;
	Mouse.x = (vga_getresolution(1) - Mouse.width) / 2;
	Mouse.y = (vga_getresolution(2) - Mouse.height) / 2;
	Mouse.x_old = Mouse.x;
	Mouse.y_old = Mouse.y;
	Window window;
	while (1)
	{
		vga_clear(0x01);
		gui_window_update(&window);
		gui_window_paint(&window);
		gui_window_draw(&window);
		vga_drawimage(Mouse.x, Mouse.y, Mouse.width, Mouse.height, MousePointer_24x32);
		vga_swapbuffers();

		/* Mouse Interrupt */
		ProcessMousePacket();
		MouseInterrupt();
	}
	while (1)
		;
}