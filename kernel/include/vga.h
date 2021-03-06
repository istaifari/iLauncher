#pragma once
#include <kernel.h>

static unsigned char g_640x480x2[] =
	{
		2,
		/* MISC */
		0xE3,
		/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x06,
		/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
		0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
		0xFF,
		/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F,
		0xFF,
		/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
		0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		0x01, 0x00, 0x0F, 0x00, 0x00};
/*****************************************************************************
*** NOTE: the mode described by g_320x200x4[]
is different from BIOS mode 05h in two ways:
- Framebuffer is at A000:0000 instead of B800:0000
- Framebuffer is linear (no screwy line-by-line CGA addressing)
*****************************************************************************/
static unsigned char g_320x200x4[] =
	{
		4,
		/* MISC */
		0x63,
		/* SEQ */
		0x03, 0x09, 0x03, 0x00, 0x02,
		/* CRTC */
		0x2D, 0x27, 0x28, 0x90, 0x2B, 0x80, 0xBF, 0x1F,
		0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x0E, 0x8F, 0x14, 0x00, 0x96, 0xB9, 0xA3,
		0xFF,
		/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x02, 0x00,
		0xFF,
		/* AC */
		0x00, 0x13, 0x15, 0x17, 0x02, 0x04, 0x06, 0x07,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x01, 0x00, 0x03, 0x00, 0x00};

static unsigned char g_640x480x16[] =
	{
		16,
		/* MISC */
		0xE3,
		/* SEQ */
		0x03, 0x01, 0x08, 0x00, 0x06,
		/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
		0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
		0xFF,
		/* GC */
		0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
		0xFF,
		/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
		0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		0x01, 0x00, 0x0F, 0x00, 0x00};

static unsigned char g_720x480x16[] =
	{
		16,
		/* MISC */
		0xE7,
		/* SEQ */
		0x03, 0x01, 0x08, 0x00, 0x06,
		/* CRTC */
		0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
		0x00, 0x40, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
		0xEA, 0x0C, 0xDF, 0x2D, 0x08, 0xE8, 0x05, 0xE3,
		0xFF,
		/* GC */
		0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
		0xFF,
		/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x01, 0x00, 0x0F, 0x00, 0x00
};

static unsigned char g_320x200x8[] =
	{
		8,
		/* MISC */
		0x63,
		/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x0E,
		/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
		0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
		0xFF,
		/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
		0xFF,
		/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x41, 0x00, 0x0F, 0x00, 0x00};

extern uint32_t vga_width, vga_height, vga_bpp, vga_depth, vga_pitch, vga_bufsize;

void vga_clear(long c);
void vga_swapbuffers();
uint32_t vga_getresolution(int select);
void vga_start();