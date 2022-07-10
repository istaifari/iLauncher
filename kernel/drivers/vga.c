#include <vga.h>
#include <vesa.h>

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

uint32_t curr_mode;
unsigned vga_width, vga_height, vga_bpp, vga_bufsize;

mode_info_t *vbe_screen;

int8_t *vga_buffer[2];
int8_t *vga_memory = 0x00000;
intptr_t vga_memory_address = 0x00000;
uint32_t *CursorImg = (uint32_t *)(2);

mode_info_t *vesa_get_info()
{
	mode_info_t *mode_info = kmalloc(sizeof(mode_info_t));
	vesa_get_mode((uint16_t)curr_mode, mode_info);
	return mode_info;
}

void vesa_memcpy24_32(uint24_t *dest, uint32_t *src, uint32_t count)
{
	uint24_t t;
	uint32_t i;
	for (i = 0; i < count; i++)
	{
		t.integer = src[i];
		dest[i] = t;
	}
}
void vesa_memset_rgb(uint8_t *dest, uint32_t rgb, uint32_t count)
{
	if (count % 3 != 0)
		count = count + 3 - (count % 3);
	uint8_t r = rgb & 0x00ff0000;
	uint8_t g = rgb & 0x0000ff00;
	uint8_t b = rgb & 0x000000ff;
	for (int i = 0; i < count; i++)
	{
		*dest++ = r;
		*dest++ = g;
		*dest++ = b;
	}
}

void vesa_get_mode(uint16_t mode, mode_info_t *mode_info)
{
	register16_t reg_in = {0};
	register16_t reg_out = {0};
	reg_in.ax = 0x4F01;
	reg_in.cx = mode;
	reg_in.di = 0x9000;
	bios32_service(BIOS_GRAPHICS_SERVICE, &reg_in, &reg_out);
	memcpy(mode_info, (void *)0x9000, sizeof(mode_info_t));
}

void vesa_set_mode(uint32_t mode)
{
	register16_t reg_in = {0};
	register16_t reg_out = {0};
	reg_in.ax = 0x4F02;
	reg_in.bx = mode;
	bios32_service(BIOS_GRAPHICS_SERVICE, &reg_in, &reg_out);
	curr_mode = mode;
}

uint32_t vesa_find_mode(uint32_t width, uint32_t height, uint32_t bpp)
{
	vbe_info_t *vbe_info;
	mode_info_t *mode_info;
	register16_t reg_in = {0};
	register16_t reg_out = {0};
	reg_in.ax = 0x4F00;
	reg_in.di = 0x9500;
	bios32_service(BIOS_GRAPHICS_SERVICE, &reg_in, &reg_out);
	memcpy(vbe_info, (void *)0x9500, sizeof(vbe_info_t));
	uint16_t *mode_list = vbe_info->video_modes;
	uint16_t mode_number = *mode_list++;
	while (mode_number != 0xffff)
	{
		vesa_get_mode(mode_number, mode_info);
		if (mode_info->resolutionX == 1024 && mode_info->resolutionY == 768 && mode_info->bpp == 32)
			return mode_number;
		mode_number = *mode_list++;
	}
	return 0;
}

void vesa_install()
{
	bios32_install();
	vesa_set_mode(0x144 | 0x4000);
	vbe_screen = vesa_get_info();
	vga_memory = (char *)(vbe_screen->physbase);
	allocate_region(kpage_dir, (uint32_t)vga_memory, (uint32_t)(vga_memory + 1024 * 768 * 4), 1, 1, 1);
	memset(vga_memory, 0xf0, 1024 * 768);
}

static uintptr_t GetFramebuffer(void)
{
	uintptr_t seg;
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

void vga_putpixel_memory(long x, long y, long c)
{
	if ((y * vga_width + x) < 0 || (y * vga_width + x) >= vga_width * vga_height)
		return;
	long pos = 0;
	if (vga_bpp == 4)
	{
		pos = (y * vga_width + x) / 4;
		vga_memory[pos] = (vga_memory[pos] & ~(0xC0 >> ((x & 3) * 2))) | (((c & 3) * 0x55) & (0xC0 >> ((x & 3) * 2)));
	}
	if (vga_bpp == 8)
	{
		pos = (y * vga_width + x);
		vga_memory[pos] = c;
	}
	if (vga_bpp == 16)
	{
		pos = (y * vga_width + x) / 8;
		for (long p = 0; p < 4; p++)
		{
			set_plane(p);
			if ((1 << p) & c)
				vga_memory[pos / 2] |= (0x80 >> ((x & 7) * 1));
			else
				vga_memory[pos / 2] &= ~(0x80 >> ((x & 7) * 1));
		}
	}
}

void vga_putpixel(long x, long y, long c)
{
	if (x < 0 || x >= vga_width || y < 0 || y >= vga_height)
		return;
	vga_buffer[0][y * vga_width + x] = c;
}

void vga_drawimage(const uint32_t *pixels, long x, long y, long w, long h)
{
	long i = 0, l = 0, j = 0;
	for (l = j = 0; l < h; l++)
	{
		for (i = 0; i < w; i++, j++)
		{
			// if (pixels[j] & 0xFF000000)
			if (pixels[j + 2] != 0x40)
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
	if (!text)
		return;
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

uint32_t vga_getresolution(int select)
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

uintptr_t uint(intptr_t i)
{
	if (i < 0)
		return -i;
	else
		return i;
}

void vga_buffercopy(char *b1, char *b2, char *mem, size_t bsz)
{
	uint64_t num_dwords = bsz / 8;
	uint64_t num_bytes = bsz % 8;
	uint64_t *dest64 = (uint64_t *)b1;
	uint64_t *src64 = (uint64_t *)b2;
	uint64_t *mem64 = (uint64_t *)mem;
	uint8_t *dest8 = ((uint8_t *)b1) + num_dwords * 8;
	uint8_t *src8 = ((uint8_t *)b2) + num_dwords * 8;
	uint8_t *mem8 = ((uint8_t *)mem) + num_dwords * 8;
	if (vga_bpp == 8)
		goto vga_bpp_8;
	if (vga_bpp == 16)
		goto vga_bpp_16;
vga_bpp_8:
	for (uint64_t i = 0; i < num_dwords; i++)
		if (dest64[i] != src64[i])
			mem64[i] = dest64[i];
	for (uint64_t i = 0; i < num_bytes; i++)
		if (dest8[i] != src8[i])
			mem8[i] = dest8[i];
	goto end;
vga_bpp_16:
	for (uint64_t i = 0; i < num_dwords; i++)
		if (dest64[i] != src64[i])
			for (long p = 0; p < 4; p++)
			{
				set_plane(p);
				if ((1 << p) & dest64[i])
					mem64[i / 8] |= uint(0x80 >> uint(i & 7));
				else
					mem64[i / 8] &= ~uint(0x80 >> uint(i & 7));
			}
	for (uint64_t i = 0; i < num_bytes; i++)
	{
		if (dest8[i] != src8[i])
			mem8[i / 8] = dest8[i];
	}
	goto end;
	/*for (size_t y = 0; y < vga_height; y++)
		for (size_t x = 0; x < vga_width; x++)
			if (b1[y * vga_width + x] != b2[y * vga_width + x])
				vga_putpixel_memory(x, y, b1[y * vga_width + x]);*/
end:
	return;
}

void vga_keys_handler(WindowInfo info);
long xo = (128 - 24) / 2;
long yo = (128 - 24) / 2;
long co = 0x02;

void vga_setupFramebuffer(unsigned ScreenWidth, unsigned ScreenHeight, unsigned BitsPerPixel)
{
	vga_width = ScreenWidth;
	vga_height = ScreenHeight;
	vga_bpp = BitsPerPixel;
	vga_bufsize = vga_width * vga_height;
	kfree(vga_buffer[0]);
	kfree(vga_buffer[1]);
	if (vga_memory_address != alloc_address(GetFramebuffer()))
	{
		vga_memory = (int8_t *)(alloc_address(GetFramebuffer()));
		vga_memory_address = alloc_address(GetFramebuffer());
	}
	vga_buffer[0] = (int8_t *)(kmalloc(vga_bufsize));
	vga_buffer[1] = (int8_t *)(kmalloc(vga_bufsize));
	memset(vga_buffer[0], 0x00, vga_bufsize);
	memset(vga_buffer[1], 0x00, vga_bufsize);
	for (size_t y = 0; y < vga_height; y++)
		for (size_t x = 0; x < vga_width; x++)
			vga_putpixel_memory(x, y, 0x00);
	mouse_ps2->width = MousePointer_24x32[0];
	mouse_ps2->height = MousePointer_24x32[1];
	mouse_ps2->x = (vga_getresolution(1) - mouse_ps2->width) / 2;
	mouse_ps2->y = (vga_getresolution(2) - mouse_ps2->height) / 2;
	mouse_ps2->x_old = mouse_ps2->x;
	mouse_ps2->y_old = mouse_ps2->y;
}

void vga_WriteResolutionRegister(uint8_t *rgs, unsigned w, unsigned h)
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
	vga_setupFramebuffer(w, h, bpp);
	kfree(regs);
}

void vga_clear(long c)
{
	memcpy(vga_buffer[1], vga_buffer[0], vga_bufsize);
	memset(vga_buffer[0], c, vga_bufsize);
}

void vga_swapbuffers()
{
	vga_buffercopy(vga_buffer[0], vga_buffer[1], vga_memory, vga_bufsize);
}

void vga_start()
{
	char *resolution = "320x200x8";
	if (resolution == "320x200x8")
	{
		vga_WriteResolutionRegister(g_320x200x8, 320, 200);
	}
	else if (resolution == "320x200x4")
	{
		vga_WriteResolutionRegister(g_320x200x4, 320, 200);
	}
	else if (resolution == "640x480x16")
	{
		vga_WriteResolutionRegister(g_640x480x16, 640, 480);
	}
	else if (resolution == "720x480x16")
	{
		vga_WriteResolutionRegister(g_720x480x16, 720, 480);
	}
}

void vga_keys_handler(WindowInfo info)
{
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

long charlen(char *buffer, char target)
{
	long in = 0;
	for (long i = 0; buffer[i] != '\0'; i++)
		if (buffer[i] == target)
			in++;
	return in;
}

long charstop(char *buffer, char target, intptr_t offset)
{
	long in = 0;
	for (long i = 0; buffer[i] != '\0'; i++)
		if (buffer[i + offset] != target)
			in++;
	return in;
}

void vga_update()
{
	Window *window = kmalloc(sizeof(Window));
	memset(window, 0, sizeof(Window));
	char *strfile = tarfs_get_file(initfs, "file.txt");
	keyboard_keybuffer_t *keybuffer = keyboard_keybuffer_setup(16);
	// clock_settime(timer, "23:59:40"); // is not working
	while (1)
	{
		vga_clear(0x01);
		gui_window_update(window);
		gui_window_paint(window);
		if (keyboard_keybuffer_scan(keybuffer))
			vga_drawtext("keybuffer enter: true", 0, 8 * 5, 0x02, font_8x8, 8, 8);
		else
			vga_drawtext("keybuffer enter: false", 0, 8 * 5, 0x04, font_8x8, 8, 8);
		gui_window_scancode(window, "\n\bb sxzcSXZC", vga_keys_handler, keyboard_get_key());
		for (long l = 0; l < 24; l++)
		{
			for (long i = 0; i < 24; i++)
			{
				gui_window_draw(window, l + xo, i + yo, co);
			}
		}
		vga_drawchar(keyboard_get_key(), 0, 0, 0x3f, font_8x8, 8, 8, false);
		vga_drawtext(mouse_keyboard, 0, 8, 0x3f, font_8x8, 8, 8);
		vga_drawtext(clock_read(timer, 3), (8 * 3) * 0, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(":", (8) * 2, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(clock_read(timer, 2), (8 * 3) * 1, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(":", (8) * 5, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(clock_read(timer, 1), (8 * 3) * 2, 8 * 2, 0x3f, font_8x8, 8, 8);
		vga_drawtext(strfile, 0, 8 * 3, 0x3f, font_8x8, 8, 8);
		vga_drawtext(keyboard_keybuffer_read(keybuffer), 0, 8 * 4, 0x3f, font_8x8, 8, 8);
		vga_drawimage(MousePointer_24x32, mouse_ps2->x, mouse_ps2->y, mouse_ps2->width, mouse_ps2->height);
		vga_swapbuffers();
	}
}