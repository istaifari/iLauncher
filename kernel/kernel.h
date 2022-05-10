#pragma once
#define VIDEO_MEMORY 0xb8000
#define VIDEO_CONTROL_PIN 0x3D4
#define VIDEO_DATA_PIN 0x3D5
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include "include/fonts.h"
#include "include/interrupts.h"
#include "include/port.h"
#include "include/vga.h"
#include "include/mouse.h"
#include "include/stdlib.h"
#include "include/string.h"
#include "include/video.h"
#include "include/stdio.h"
#include "include/keyboard.h"

extern void _poweroff();
extern void _reboot();
extern int _krand(int,int);
extern void _ksleep(int);
extern char echo_color;
extern bool gfx_mode;
extern char *current_dir;
extern char *user_name;
extern char *os_name;
extern char *os_version;