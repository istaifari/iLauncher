#pragma once
#include <ctype.h>
#include <math.h>
#include <io.h>
#include <gdt.h>
#include <tss.h>
#include <idt.h>
#include <bios32.h>
#include <pic.h>
#include <fonts.h>
#include <soundblaster.h>
#include <clock.h>
#include <acpi.h>
#include <vga.h>
#include <mouse.h>
#include <stdlib.h>
#include <string.h>
#include <video.h>
#include <stdio.h>
#include <keyboard.h>
#include <gui.h>
#include <multiboot.h>
#include <pmm.h>
#include <kheap.h>
#include <path.h>
#include <tarfs.h>
#include <ordered_array.h>
#include <page.h>
#include <fs.h>
#include <list.h>
#include <image.h>
#include <link.h>

#define LOAD_MEMORY_ADDRESS 0xC0000000
#define K 1024
#define M (1024*K)
#define G (1024*M)

static void panic_assert(const char *file, uint32_t line, const char *desc)
{
	return;
	asm("cli");
	/*monitor_write("ASSERTION-FAILED(");
	monitor_write(desc);
	monitor_write(") at ");
	monitor_write(file);
	monitor_write(":");
	monitor_write_dec(line);
	monitor_write("\n");*/
	asm("sti");
}

static inline uintptr_t alloc_address(uintptr_t add)
{
	return LOAD_MEMORY_ADDRESS + add;
}

#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

void poweroff();
void reboot();

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