#include "../include/video.h"
#include "../include/interrupts.h"

char kbdus_keycode = 0;

unsigned char kbdus[128] =
    {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
        '9', '0', '-', '=', 8,                            /* Backspace */
        0,                                                /* Tab */
        'q', 'w', 'e', 'r',                               /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', 13,       /* Enter key */
        17,                                               /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`', 0,                                     /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
        'm', ',', '.', '/', 0,                            /* Right shift */
        '*',
        18,  /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0,  /* < ... F10 */
        0,  /* 69 - Num lock*/
        0,  /* Scroll Lock */
        0,  /* Home key */
        38, /* Up Arrow */
        0,  /* Page Up */
        '-',
        37, /* Left Arrow */
        0,
        39, /* Right Arrow */
        '+',
        35,  /* 79 - End key*/
        40, /* Down Arrow */
        0,  /* Page Down */
        0,  /* Insert Key */
        46, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

void SetKey(char key_c)
{
  kbdus_keycode = key_c;
  if (kbdus_keycode >= 128 || kbdus_keycode < 0)
    kbdus_keycode = 0;
}

char GetKey()
{
  return kbdus[kbdus_keycode];
}