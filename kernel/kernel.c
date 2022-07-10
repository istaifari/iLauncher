#include "kernel.h"

multiboot_info_t *mboot_ptr;
struct tar_header *initfs;
bool gfx_mode = true;
char *current_dir = "~";
char *user_name = "iLauncher";
char *pc_name = "iLauncher";
char *os_version;
char *mouse_keyboard = "Null";
int txt = 0;
int txt_max = 0;
int errr_cmd = 0;
char *txt_part;
char echo_color = 0x0f;

void poweroff()
{
    return;
}

void reboot()
{
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    asm("hlt");
}

void _kernel_errr_cmd(int type)
{
    if (type == 1)
    {
        printtext("Unknown Command!\n", 0x04, 0);
    }
    else if (type == 2)
    {
        if (!(txt >= txt_max && txt_part == "NULL"))
        {
            errr_cmd++;
            if (errr_cmd < 1)
            {
                _kernel_errr_cmd(1);
            }
        }
    }
    else
    {
        return;
    }
}

void _kernel_gfx_mode()
{
    vga_start();
    vga_update();
}

void _kernel_ramdisk_error()
{
    char *screen = (char *)VIDEO_MEMORY;
    long i;
    for (i = 0; i < 4000; i++)
    {
        *screen = 0x20;
        *(screen + 1) = 0x02;
        screen += 2;
    }
    set_cursor_pos(0, 0);
    screen = (char *)VIDEO_MEMORY;
    for (i = 0; i < 4000; i++)
    {
        *screen = 0x20;
        *(screen + 1) = 0x11;
        screen += 2;
    }
    set_cursor_pos(0, 0);
    printtext("Fatal Error: No Ramdisk\n", 0x1f, 0);
    char *s = "0";
    long icr = rand() % 10 + 1;
    if (icr < 0)
        icr = -icr;
    for (i = 0; i < 100; i += icr)
    {
        icr = rand() % 10 + 1;
        if (icr < 0)
            icr = -icr;
        set_cursor_pos(0, 1);
        itoa(i, s);
        printtext(s, 0x1f, 0);
        printchar('%', 0x1f, 0);
        if (i >= 90)
        {
            i = 100;
            continue;
        }
        sleep(100);
    }
    if (i >= 100)
    {
        i = 100;
        set_cursor_pos(0, 1);
        itoa(i, s);
        printtext(s, 0x1f, 0);
        printchar('%', 0x1f, 0);
        sleep(1000);
    }
    reboot();
}

void devices_install()
{
    InitTimer();
    InitPS2Keyboard();
    InitPS2Mouse();
    ResetPS2Keyboard();
}

void _kernel(multiboot_info_t *mboot)
{
    mboot_ptr = mboot;
    gdt_install();
    idt_install();
    tss_install(5, 0x10, 0);
    pmm_install(1096 * M);
    paging_install();
    kheap_install(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX_ADDRESS);
    devices_install();
    // if (mboot_ptr->mods_count > 0)
    //     initfs = tarfs_install();
    // else
    //     _kernel_ramdisk_error();
    os_version = tarfs_get_file(initfs, "version.txt");
    for (long volatile in = 0; in < 2; in++)
    {
        for (long volatile i = 0; i < 64000000; i++)
        {
            if (keyboard_ps2->isLeftShiftPressed | keyboard_ps2->isRightShiftPressed)
            {
                gfx_mode = false;
                break;
            }
        }
    }
    if (gfx_mode)
        _kernel_gfx_mode();
    clear_screen();
    printtext("\f", 0x0f, 0);
    keyboard_keybuffer_t *dos_buffer = keyboard_keybuffer_setup(32);
    while (1)
    {
        scan(dos_buffer->buffer, dos_buffer->size);
        char *clear = "clear";
        char *clean = "clean";
        char *color[32] =
            {
                "color",
                "color black",
                "color white",
                "color red",
                "color yellow",
                "color lime",
                "color aqua",
                "color blue",
                "color pink",
                "color orange"};
        char *colors = "WARNING: this only applies on \"echo\" command\n"
                       "color black\n"
                       "color white\n"
                       "color red\n"
                       "color yellow\n"
                       "color lime\n"
                       "color aqua\n"
                       "color blue\n"
                       "color pink\n"
                       "color orange\n";
        char *echo = "echo ";
        char *cat = "cat ";
        char *ist = "ist";
        char *gfx_interface = "ilfb";
        char *poweroff_options[2];
        poweroff_options[0] = "poweroff";
        poweroff_options[1] = "reboot";
        char *help = "help";
        if (strcmp(dos_buffer->buffer, clear) == 0 || strcmp(dos_buffer->buffer, clean) == 0)
        {
            clear_screen();
        }
        else if (strstr(dos_buffer->buffer, cat))
        {
            printtext(tarfs_get_file(initfs, dos_buffer->buffer + strlen(cat)), 0x0f, 0);
            printchar('\n', 0x0f, 0);
        }
        else if (strcmp(dos_buffer->buffer, gfx_interface) == 0)
        {
            _kernel_gfx_mode();
        }
        else if (strcmp(dos_buffer->buffer, color[0]) == 0)
        {
            printtext("Colors: \n{\n", 0x0f, 0);
            printtext(colors, 0x0f, 0);
            printtext("}\n", 0x0f, 0);
        }
        else if (strcmp(dos_buffer->buffer, color[1]) == 0)
        {
            echo_color = 0x0;
        }
        else if (strcmp(dos_buffer->buffer, color[2]) == 0)
        {
            echo_color = 0xf;
        }
        else if (strcmp(dos_buffer->buffer, color[3]) == 0)
        {
            echo_color = 0x4;
        }
        else if (strcmp(dos_buffer->buffer, color[4]) == 0)
        {
            echo_color = 0xe;
        }
        else if (strcmp(dos_buffer->buffer, color[5]) == 0)
        {
            echo_color = 0xa;
        }
        else if (strcmp(dos_buffer->buffer, color[6]) == 0)
        {
            echo_color = 0xb;
        }
        else if (strcmp(dos_buffer->buffer, color[7]) == 0)
        {
            echo_color = 0x1;
        }
        else if (strcmp(dos_buffer->buffer, color[8]) == 0)
        {
            echo_color = 0x5;
        }
        else if (strcmp(dos_buffer->buffer, color[9]) == 0)
        {
            echo_color = 0x6;
        }
        else if (strstr(dos_buffer->buffer, echo))
        {
            printtext(dos_buffer->buffer + strlen(echo), echo_color, 0);
            printtext("\n", 0x0f, 0);
        }
        else if (strcmp(dos_buffer->buffer, help) == 0)
        {
            printtext("Commands: {\nclear: Clear screen\nclean: Clear screen\necho: Print text to screen\ncolor: Set echo text color\npoweroff: Turn off computer\nreboot: Reboot computer\n}\n", 0x0f, 0);
        }
        else if (strcmp(dos_buffer->buffer, poweroff_options[0]) == 0)
        {
            poweroff();
        }
        else if (strcmp(dos_buffer->buffer, poweroff_options[1]) == 0)
        {
            reboot();
        }
        else if (strlen(dos_buffer->buffer) > 0)
        {
            _kernel_errr_cmd(1);
        }
        printtext("\f", 0x0f, 0);
    }
}

void _kernel_opendir_char(char dir)
{
    if (dir == 'c')
    {
        return;
    }
    current_dir = '\0';
    current_dir += dir;
}

void _kernel_opendir(char *cd_direct)
{
    char c_dir = 0;
    while ((c_dir = *cd_direct++) != '\0')
    {
        _kernel_opendir_char(c_dir);
    }
}