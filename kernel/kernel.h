#pragma once
#define VIDEO_MEMORY 0xb8000
#define VIDEO_CONTROL_PIN 0x3D4
#define VIDEO_DATA_PIN 0x3D5
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include "include/io.h"
#include "include/gdt.h"
#include "include/tss.h"
#include "include/idt.h"
#include "include/pic.h"
#include "include/fonts.h"
#include "include/audio.h"
#include "include/clock.h"
#include "include/vga.h"
#include "include/mouse.h"
#include "include/stdlib.h"
#include "include/string.h"
#include "include/video.h"
#include "include/stdio.h"
#include "include/keyboard.h"
#include "include/gui.h"
#include "include/multiboot.h"
#include "include/kheap.h"
#include "include/path.h"
#include "include/tarfs.h"
#include "include/ordered_array.h"
#include "include/page.h"
#include "include/fs.h"
#include "include/list.h"
#include "include/image.h"
#include "include/link.h"

static void panic_assert(const char *file, uint32_t line, const char *desc)
{
	asm("cli");
	/*monitor_write("ASSERTION-FAILED(");
	monitor_write(desc);
	monitor_write(") at ");
	monitor_write(file);
	monitor_write(":");
	monitor_write_dec(line);
	monitor_write("\n");*/
	for (;;)
		_reboot();
}

typedef char byte;
typedef char *string;
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

static void _shutdown()
{
	outw(0x604, 0x2000);
	outw(0xB004, 0x2000);
	outw(0x4004, 0x3400);
	asm("hlt");
}

extern void _reboot();
extern uint32_t placement_address;
extern multiboot_info_t *mboot_ptr;
extern struct tar_header *initfs;
extern bool gfx_mode;
extern char echo_color;
extern char *current_dir;
extern char *user_name;
extern char *pc_name;
extern char *os_version;
extern char *mouse_keyboard;