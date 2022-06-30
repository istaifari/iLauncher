#include "../include/keyboard.h"

#define LeftShift 0x2A
#define RightShift 0x36
#define CapsLock 0x3A

/* Alt keys */
#define LALT 0x38
#define RALT 0xE0
#define ALTGR 0x38

/* Enter, BackSpace, Spacebar */
#define Enter 0x1C
#define BackSpace 0x0E
#define Spacebar 0x39

/* ESC key */
#define ESC 0x01

/* Cursor */
#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4B
#define RIGHT 0x4D

/* F keys */
#define F1 0x3B
#define F2 0x3C
#define F3 0x3D
#define F4 0x3E
#define F5 0x3F
#define F6 0x40
#define F7 0x41
#define F8 0x42
#define F9 0x43
#define F10 0x44
#define F11 0x57
#define F12 0x58

/* Numbers */
#define ONE 0x02
#define TWO 0x03
#define THREE 0x04
#define FOUR 0x05
#define FIVE 0x06
#define SIX 0x07
#define SEVEN 0x08
#define EIGHT 0x09
#define NINE 0x0A
#define KEY_ZERO 0x0B

#define KEYPAD_ONE 0x4F
#define KEYPAD_TWO 0x50
#define KEYPAD_THREE 0x51
#define KEYPAD_FOUR 0x6B // scan code 2
#define KEYPAD_FIVE 0x4C
#define KEYPAD_SIX 0x74 // scan code 2
#define KEYPAD_SEVEN 0x47
#define KEYPAD_EIGHT 0x48
#define KEYPAD_NINE 0x49
#define KEYPAD_KEY_ZERO 0x52

/* KEYPAD +, -, *, / */
#define PLUS 0x4E
#define MINUS 0x4A
#define MULTIPLY 0x37
#define DIVIDE 0x35 // also it can be 0xE0

keyboard_t en_international = {
    .ScanCode = 0,
    .isEnterPressed = false,
    .isLeftShiftPressed = false,
    .isRightShiftPressed = false,
    .keymap_size = 90,
    .base =
        {
            0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
            '9', '0', '-', '=', '\b',                         /* Backspace */
            '\t',                                             /* Tab */
            'q', 'w', 'e', 'r',                               /* 19 */
            't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
            0,                                                /* 29   - Control */
            'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
            '\'', '`', 0,                                     /* Left shift */
            '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
            'm', ',', '.', '/', 0,                            /* Right shift */
            '*',
            0,   /* Alt */
            ' ', /* Space bar */
            0,   /* Caps lock */
            0,   /* 59 - F1 key ... > */
            0, 0, 0, 0, 0, 0, 0, 0,
            0, /* < ... F10 */
            0, /* 69 - Num lock*/
            0, /* Scroll Lock */
            0, /* Home key */
            0, /* Up Arrow */
            0, /* Page Up */
            '-',
            0, /* Left Arrow */
            0,
            0, /* Right Arrow */
            '+',
            0, /* 79 - End key*/
            0, /* Down Arrow */
            0, /* Page Down */
            0, /* Insert Key */
            0, /* Delete Key */
            0, 0, 0,
            0, /* F11 Key */
            0, /* F12 Key */
            0, /* All other keys are undefined */
        },
    .shift = {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*',    /* 9 */
        '(', ')', '_', '+', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'Q', 'W', 'E', 'R',                               /* 19 */
        'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
        '\"', '~', 0,                                     /* Left shift */
        '|', 'Z', 'X', 'C', 'V', 'B', 'N',                /* 49 */
        'M', '<', '>', '?', 0,                            /* Right shift */
        '*', 0,                                           /* Alt */
        ' ',                                              /* Space bar */
        0,                                                /* Caps lock */
        0,                                                /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0, 0,                        /* < ... F10 */
        0,                                                /* 69 - Num lock*/
        0,                                                /* Scroll Lock */
        0,                                                /* Home key */
        0,                                                /* Up Arrow */
        0,                                                /* Page Up */
        '-', 0,                                           /* Left Arrow */
        0, 0,                                             /* Right Arrow */
        '+', 0,                                           /* 79 - End key*/
        0,                                                /* Down Arrow */
        0,                                                /* Page Down */
        0,                                                /* Insert Key */
        0,                                                /* Delete Key */
        0, 0, 0, 0,                                       /* F11 Key */
        0,                                                /* F12 Key */
        0,                                                /* All other keys are undefined */
    },
};

keyboard_t ptbr_abnt2 = {
    .ScanCode = 0,
    .isEnterPressed = false,
    .isLeftShiftPressed = false,
    .isRightShiftPressed = false,
    .keymap_size = 90,
    .base =
        {
            0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
            '9', '0', '-', '=', '\b',                         /* Backspace */
            '\t',                                             /* Tab */
            'q', 'w', 'e', 'r',                               /* 19 */
            't', 'y', 'u', 'i', 'o', 'p', '´', '[', '\n',     /* Enter key */
            0,                                                /* 29   - Control */
            'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'ç', /* 39 */
            '~', '\'', 0,                                     /* Left shift */
            ']', 'z', 'x', 'c', 'v', 'b', 'n',                /* 49 */
            'm', ',', '.', ';', 0,                            /* Right shift */
            '*',
            0,   /* Alt */
            ' ', /* Space bar */
            0,   /* Caps lock */
            0,   /* 59 - F1 key ... > */
            0, 0, 0, 0, 0, 0, 0, 0,
            0, /* < ... F10 */
            0, /* 69 - Num lock*/
            0, /* Scroll Lock */
            0, /* Home key */
            0, /* Up Arrow */
            0, /* Page Up */
            '-',
            0, /* Left Arrow */
            0,
            0, /* Right Arrow */
            '+',
            0, /* 79 - End key*/
            0, /* Down Arrow */
            0, /* Page Down */
            0, /* Insert Key */
            0, /* Delete Key */
            0, 0, 0,
            0, /* F11 Key */
            0, /* F12 Key */
            0, /* All other keys are undefined */
        },
    .shift = {
        0, 27, '!', '@', '#', '$', '%', '¨', '&', '*',    /* 9 */
        '(', ')', '_', '+', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'Q', 'W', 'E', 'R',                               /* 19 */
        'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Ç', /* 39 */
        '^', '\"', 0,                                     /* Left shift */
        '}', 'Z', 'X', 'C', 'V', 'B', 'N',                /* 49 */
        'M', '<', '>', ':', 0,                            /* Right shift */
        '*', 0,                                           /* Alt */
        ' ',                                              /* Space bar */
        0,                                                /* Caps lock */
        0,                                                /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0, 0,                        /* < ... F10 */
        0,                                                /* 69 - Num lock*/
        0,                                                /* Scroll Lock */
        0,                                                /* Home key */
        0,                                                /* Up Arrow */
        0,                                                /* Page Up */
        '-', 0,                                           /* Left Arrow */
        0, 0,                                             /* Right Arrow */
        '+', 0,                                           /* 79 - End key*/
        0,                                                /* Down Arrow */
        0,                                                /* Page Down */
        0,                                                /* Insert Key */
        0,                                                /* Delete Key */
        0, 0, 0, 0,                                       /* F11 Key */
        0,                                                /* F12 Key */
        0,                                                /* All other keys are undefined */
    },
};

keyboard_t *default_keyboard = (keyboard_t *)&en_international;

keyboard_keybuffer_t *keyboard_keybuffer_setup(size_t size)
{
  keyboard_keybuffer_t *tmp = kmalloc(sizeof(keyboard_keybuffer_t) * (size+1));
  tmp->buffer = (char *)(kmalloc(size + 1));
  tmp->size = size + 1;
  memset(tmp->buffer, '\0', tmp->size);
  return tmp;
}

void *keyboard_keybuffer_free(keyboard_keybuffer_t *keybuffer)
{
  kfree(keybuffer);
}

void *keyboard_keybuffer_read(keyboard_keybuffer_t *keybuffer)
{
  if (!keybuffer)
    return "errn: keybuffer is empty/invalid";
  return keybuffer->buffer;
}

void *keyboard_keybuffer_scan(keyboard_keybuffer_t *keybuffer)
{
  if (!keybuffer)
    return false;
  char keycode = GetKey();
  long offset = strlen(keybuffer->buffer);
  switch (keycode)
  {
  case 0:
    return false;
  case '\n':
    return true;
  case '\t':
    for (long i = 0; i < 4; i++)
    {
      if (offset + (i + 1) < keybuffer->size - 1)
      {
        keybuffer->buffer[offset] = ' ';
      }
    }
    return false;
  case '\b':
    if (offset > 0)
    {
      keybuffer->buffer[offset - 1] = '\0';
    }
    return false;
  default:
    if (offset < keybuffer->size - 1)
    {
      keybuffer->buffer[offset] = keycode;
    }
    return false;
  }
}

char SetKey(char key_c, bool uppercase)
{
  if (key_c >= default_keyboard->keymap_size || key_c < 0)
    return 0;
  if (uppercase)
  {
    default_keyboard->ScanCode = (char)default_keyboard->shift[key_c];
    default_keyboard->OnKeyDown = 0;
  }
  else
  {
    default_keyboard->ScanCode = (char)default_keyboard->base[key_c];
    default_keyboard->OnKeyDown = 0;
  }
}

void *GetCharPos(const char *ss, char cc)
{
  for (long i = 0; i < i + 1; i++)
  {
    if (ss[i] != '\0')
      if (ss[i] == cc)
        return i;
  }
}

char GetKey()
{
  if (default_keyboard->OnKeyDown == 1)
  {
    if (default_keyboard->OnKeyDown == 1)
      default_keyboard->ScanCode = 0;
    default_keyboard->OnKeyDown = 2;
    return 0;
  }
  else if (default_keyboard->OnKeyDown == 2)
  {
    return 0;
  }
  else if (default_keyboard->OnKeyDown == 0)
  {
    default_keyboard->OnKeyDown = 1;
    return default_keyboard->ScanCode;
  }
}

void ScanKey(unsigned char scancode)
{
  switch (scancode)
  {
  /*case 0x1C:
    default_keyboard->isEnterPressed = true;
    return;
  case 0x1C + 0x80:
    default_keyboard->isEnterPressed = false;
    return;*/
  case 0x2A:
    default_keyboard->isLeftShiftPressed = true;
    return;
  case 0x2A + 0x80:
    default_keyboard->isLeftShiftPressed = false;
    return;
  case 0x36:
    default_keyboard->isRightShiftPressed = true;
    return;
  case 0x36 + 0x80:
    default_keyboard->isRightShiftPressed = false;
    return;
  default:
    SetKey(scancode, default_keyboard->isLeftShiftPressed | default_keyboard->isRightShiftPressed);
  }
}

__attribute__((interrupt)) void HandlePS2Keyboard(int_frame_t *r)
{
  unsigned char scancode = inb(0x60);
  ScanKey(scancode);
  mouse_keyboard = "keyboard";
  PIC_End();
}