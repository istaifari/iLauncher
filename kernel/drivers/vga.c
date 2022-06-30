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

unsigned vga_width, vga_vm_width, vga_height, vga_vm_height, vga_bpp, vga_bufsize;

char *vga_oldbackbuffer;
char *vga_backbuffer;
char *vga_memory;
uint32_t *CursorImg = (uint32_t *)(2);

static unsigned GetFramebuffer(void)
{
	unsigned seg;
	outb(VGA_GC_INDEX, 0x06);
	seg = ((inb(VGA_GC_DATA) >> 2) & 3);
	switch (seg)
	{
	case 1:
		return 0xA0000;
	case 2:
		return 0xB0000;
	case 3:
		return 0xB8000;
	default:
		return 0x00000;
	}
}

void set_plane(uint64_t p)
{
	outb(VGA_GC_INDEX, 4);
	outb(VGA_GC_DATA, p);
	outb(VGA_SEQ_INDEX, 2);
	outb(VGA_SEQ_DATA, 1 << p);
}

void vga_putpixel_memory(long xy, long c)
{
	if (xy < 0 || xy >= vga_width * vga_height)
		return;
	long pos = 0;
	if (vga_bpp == 4)
	{
		pos = xy / 4;
		vga_memory[pos] = (vga_memory[pos] & ~(0xC0 >> ((xy & 3) * 2))) | (((c & 3) * 0x55) & (0xC0 >> ((xy & 3) * 2)));
	}
	if (vga_bpp == 8)
	{
		pos = xy;
		vga_memory[pos] = c;
	}
	if (vga_bpp == 16)
	{
		pos = xy / 8;
		for (long p = 0; p < 4; p++)
		{
			set_plane(p);
			if ((1 << p) & c)
				vga_memory[pos] |= (0x80 >> ((xy & 7) * 1));
			else
				vga_memory[pos] &= ~(0x80 >> ((xy & 7) * 1));
		}
	}
}

void vga_putpixel(long x, long y, long c)
{
	if (x < 0 || x >= vga_width || y < 0 || y >= vga_height)
		return;
	else
		vga_backbuffer[y * vga_width + x] = c;
}

void vga_drawimage(const uint32_t *pixels, long x, long y, long w, long h)
{
	long i = 0, l = 0, j = 0;
	for (l = j = 0; l < h; l++)
	{
		for (i = 0; i < w; i++, j++)
		{
			// if (pixels[j] & 0xFF000000)
			vga_putpixel(x + i, y + l, pixels[j + 2]);
		}
	}
}

void vga_fillRect(long x, long y, long w, long h, long c)
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

void vga_drawchar(char chr, long x, long y, long c, unsigned char *font, long font_w, long font_h, bool flip)
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

void vga_drawtext(char *text, long x, long y, long c, unsigned char *font, long font_w, long font_h)
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
		vga_drawchar(chr, ((col - 1) * font_width) + x, (row * font_height) + y, c, font, font_w, font_h, false);
	}
}

unsigned vga_getresolution(int select)
{
	if (select == 1)
	{
		return vga_vm_width;
	}
	else if (select == 2)
	{
		return vga_vm_height;
	}
	else
	{
		return 0;
	}
}

void vga_buffercopy(char *b1, char *b2, size_t bsz)
{
	for (size_t i = 0; i < vga_bufsize; i++)
	{
		if (b1[i] != b2[i])
			vga_putpixel_memory(i, b1[i]);
	}
}

void vga_setupFramebuffer(unsigned VMWidth, unsigned VMHeight, unsigned ScreenWidth, unsigned ScreenHeight, unsigned BitsPerPixel)
{
	vga_width = ScreenWidth;
	vga_height = ScreenHeight;
	vga_vm_width = VMWidth;
	vga_vm_height = VMHeight;
	vga_bpp = BitsPerPixel;
	vga_bufsize = vga_vm_width * vga_vm_height;
	kfree(vga_backbuffer);
	kfree(vga_oldbackbuffer);
	vga_backbuffer = (char *)(kmalloc(vga_bufsize));
	vga_oldbackbuffer = (char *)(kmalloc(vga_bufsize));
	vga_memory = (char *)(GetFramebuffer());
	memset(vga_oldbackbuffer, 0x00, vga_bufsize);
	memset(vga_backbuffer, 0x00, vga_bufsize);
	for (size_t i = 0; i < vga_bufsize; i++)
		vga_putpixel_memory(i, 0x00);
	Mouse.width = MousePointer_24x32[0];
	Mouse.height = MousePointer_24x32[1];
	Mouse.x = (vga_getresolution(1) - Mouse.width) / 2;
	Mouse.y = (vga_getresolution(2) - Mouse.height) / 2;
	Mouse.x_old = Mouse.x;
	Mouse.y_old = Mouse.y;
}

void vga_WriteResolutionRegister(uint8_t *rgs, unsigned ow, unsigned oh, unsigned w, unsigned h)
{
	uint8_t bpp = rgs[0];
	uint8_t *regs = kmalloc(62);
	memcpy(regs, rgs, 62);
	unsigned i;
	outb(VGA_MISC_WRITE, regs[1]);
	for (i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, regs[i + 2]);
	}
	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
	regs[0x03 + (2 + VGA_NUM_SEQ_REGS)] |= 0x80;
	regs[0x11 + (2 + VGA_NUM_SEQ_REGS)] &= ~0x80;
	for (i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, regs[i + (2 + VGA_NUM_SEQ_REGS)]);
	}
	for (i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, regs[i + (2 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS)]);
	}
	for (i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, regs[i + (2 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS)]);
	}
	(void)inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
	vga_setupFramebuffer(ow, oh, w, h, bpp);
	kfree(regs);
}

void vga_clear(long c)
{
	memcpy(vga_oldbackbuffer, vga_backbuffer, vga_bufsize);
	memset(vga_backbuffer, c, vga_bufsize);
}

void vga_swapbuffers()
{
	vga_buffercopy(vga_backbuffer, vga_oldbackbuffer, vga_bufsize);
}

void vga_start()
{
	char *resolution = "320x200x8";
	if (resolution == "320x200x8")
	{
		vga_WriteResolutionRegister(g_320x200x8, 320, 200, 320, 200);
	}
	else if (resolution == "320x200x4")
	{
		vga_WriteResolutionRegister(g_320x200x4, 320, 200, 320, 200);
	}
	else if (resolution == "640x480x16")
	{
		vga_WriteResolutionRegister(g_640x480x16, 640, 480, 640, 480);
	}
	else if (resolution == "720x480x16")
	{
		vga_WriteResolutionRegister(g_720x480x16, 720, 480, 720, 480);
	}
}

long xo = (128 - 24) / 2;
long yo = (128 - 24) / 2;
long co = 0x02;

void vga_keys_handler(WindowInfo info)
{
	if (info.key == '!')
		vga_WriteResolutionRegister(g_320x200x8, 320, 200, 320, 200);
	if (info.key == '@')
		vga_WriteResolutionRegister(g_640x480x16, 640, 480, 640, 480);
	if (info.key == '#')
		vga_WriteResolutionRegister(g_720x480x16, 720, 480, 720, 480);
	if (info.key == 's')
		yo -= 2;
	if (info.key == 'x')
		yo += 2;
	if (info.key == 'z')
		xo -= 2;
	if (info.key == 'c')
		xo += 2;
	if (info.key == 'S')
		yo -= 4;
	if (info.key == 'X')
		yo += 4;
	if (info.key == 'Z')
		xo -= 4;
	if (info.key == 'C')
		xo += 4;
	if (info.key == '\n')
		co = 0x03;
	if (info.key == '\b')
		co = 0x04;
	if (info.key == 'b')
		co = 0x02;
	if (xo <= 0)
		xo = 0;
	if (yo <= 0)
		yo = 0;
	if (xo >= info.width - 24)
		xo = info.width - 24;
	if (yo >= info.height - 24)
		yo = info.height - 24;
}

void vga_update()
{
	CursorImg = tga_load("cursor.tga");
	Mouse.width = MousePointer_24x32[0];
	Mouse.height = MousePointer_24x32[1];
	Mouse.x = (vga_getresolution(1) - Mouse.width) / 2;
	Mouse.y = (vga_getresolution(2) - Mouse.height) / 2;
	Mouse.x_old = Mouse.x;
	Mouse.y_old = Mouse.y;
	Window window;
	char *strfile = tarfs_get_file(initfs, "file.txt");
	keyboard_keybuffer_t *keybuffer = keyboard_keybuffer_setup(16);
	// clock_settime(timer, "23:59:40"); // is not working
	while (1)
	{
		vga_clear(0x01);
		gui_window_update(&window);
		gui_window_paint(&window);
		if (keyboard_keybuffer_scan(keybuffer))
			vga_drawtext("keybuffer enter: true", 0, 8 * 5, 0x02, font_8x8, 8, 8);
		else
			vga_drawtext("keybuffer enter: false", 0, 8 * 5, 0x04, font_8x8, 8, 8);
		gui_window_scancode(&window, "\n\bb!@# sxzcSXZC", vga_keys_handler, GetKey());
		for (long l = 0; l < 24; l++)
		{
			for (long i = 0; i < 24; i++)
			{
				gui_window_draw(&window, l + xo, i + yo, co);
			}
		}
		vga_drawchar(GetKey(), 0, 0, 0x3f, font_8x8, 8, 8, false);
		vga_drawtext(mouse_keyboard, 0, 8, 0x3f, font_8x8, 8, 8);
		vga_drawtext(clock_read(timer, 3), (8 * 3) * 0, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(":", (8) * 2, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(clock_read(timer, 2), (8 * 3) * 1, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(":", (8) * 5, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(clock_read(timer, 1), (8 * 3) * 2, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(strfile, 0, 8 * 3, 0x3f, font_8x8, 8, 8);
		vga_drawtext(keyboard_keybuffer_read(keybuffer), 0, 8 * 4, 0x3f, font_8x8, 8, 8);
		vga_drawimage(MousePointer_24x32, Mouse.x, Mouse.y, Mouse.width, Mouse.height);
		/*if (i >= 250)
		{
			audio_start_beep(1000/2);
		}
		else
		{
			i++;
		}
		if (a >= 450)
		{
			audio_stop_beep();
			i = 0;
			a = 0;
		}
		else
		{
			a++;
		}*/
		vga_swapbuffers();
		/*sleep(1000);
		  sleep(1000);
		  sleep(1000);
		  sleep(1000);
		  sleep(1000);
		  sleep(1000);
		  sleep(1000);
		  sleep(1000);
		  _reboot();  */
	}
}