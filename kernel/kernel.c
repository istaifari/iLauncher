#include "kernel.h"
bool gfx_mode = false;
bool line_inf = true;
char *current_dir = "~";
char *user_name = "Medium";
char *os_name = "iLauncher";
char *os_version = "1.02";
int txt = 0;
int txt_max = 0;
int errr_cmd = 0;
char *txt_part;
char echo_color = 0x0f;

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

void _kernel_echo_char(char echo_char, bool use_c_par)
{
    if (use_c_par)
    {
        txt_max = 8;
    }
    else
    {
        txt_max = 5;
    }
    if (!(txt >= txt_max))
    {
        txt++;
        if (echo_char == 'e' && txt_part == "e")
        {
            txt_part = "c";
            return;
            txt++;
        }
        if (echo_char == 'c' && txt_part == "c")
        {
            txt_part = "h";
            return;
            txt++;
        }
        if (echo_char == 'h' && txt_part == "h")
        {
            txt_part = "o";
            return;
            txt++;
        }
        if (echo_char == 'o' && txt_part == "o")
        {
            txt_part = " ";
            return;
            txt++;
        }
        if (use_c_par)
        {
            if (echo_char == ' ' && txt_part == " ")
            {
                txt_part = "-";
                return;
                txt++;
            }
            if (echo_char == '-' && txt_part == "-")
            {
                txt_part = "c2";
                return;
                txt++;
            }
            if (echo_char == 'c' && txt_part == "c2")
            {
                txt_part = " 2";
                return;
                txt++;
            }
            if (echo_char == ' ' && txt_part == " 2")
            {
                txt_part = "NULL";
                return;
                txt++;
            }
        }
        else
        {
            if (echo_char == ' ' && txt_part == " ")
            {
                txt_part = "NULL";
                return;
                txt++;
            }
        }
    }
    else if (txt >= txt_max && txt_part == "NULL")
    {
        printchar(echo_char, echo_color, 0);
    }
}

void _kernel_echo(char *echo_direct, bool use_c_par)
{
    txt = 0;
    txt_part = "e";
    errr_cmd = 0;
    char c_echo = 0;
    while ((c_echo = *echo_direct++) != '\0')
    {
        _kernel_echo_char(c_echo, use_c_par);
    }
}

void _kernel_gfx_mode()
{
    vga_start();
    mouse_setup();
}

void _kernel()
{
    /*_kernel_gfx_mode();*/
    clear_screen();
    if (line_inf)
        printtext("\f", 0x0f, 0);
    int buf_size = 80 * 25;
    char buf[buf_size];
    while (1)
    {
        buf[buf_size];
        scan(buf, buf_size);
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
                "color orange"
            };
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
        char *ist = "ist";
        char *gfx_interface = "ilframework";
        char *poweroff_options[2];
        poweroff_options[0] = "poweroff";
        poweroff_options[1] = "reboot";
        char *help = "help";
        if (strcmp(buf, clear) == 0 || strcmp(buf, clean) == 0)
        {
            clear_screen();
        }
        else if (strcmp(buf, gfx_interface) == 0)
        {
            _kernel_gfx_mode();
        }
        else if (strcmp(buf, color[0]) == 0)
        {
            printtext("Colors: \n{\n", 0x0f, 0);
            printtext(colors, 0x0f, 0);
            printtext("}\n", 0x0f, 0);
        }
        else if (strcmp(buf, color[1]) == 0)
        {
            echo_color = 0x0;
        }
        else if (strcmp(buf, color[2]) == 0)
        {
            echo_color = 0xf;
        }
        else if (strcmp(buf, color[3]) == 0)
        {
            echo_color = 0x4;
        }
        else if (strcmp(buf, color[4]) == 0)
        {
            echo_color = 0xe;
        }
        else if (strcmp(buf, color[5]) == 0)
        {
            echo_color = 0xa;
        }
        else if (strcmp(buf, color[6]) == 0)
        {
            echo_color = 0xb;
        }
        else if (strcmp(buf, color[7]) == 0)
        {
            echo_color = 0x1;
        }
        else if (strcmp(buf, color[8]) == 0)
        {
            echo_color = 0x5;
        }
        else if (strcmp(buf, color[9]) == 0)
        {
            echo_color = 0x6;
        }
        else if (strstr(buf, echo))
        {
            _kernel_echo(buf, false);
            printtext("\n", 0x0f, 0);
        }
        else if (strcmp(buf, help) == 0)
        {
            printtext("Commands: {\nclear: Clear screen\nclean: Clear screen\necho: Print text to screen\ncolor: Set echo text color\npoweroff: Turn off computer\nreboot: Reboot computer\n}\n", 0x0f, 0);
        }
        else if (strcmp(buf, poweroff_options[0]) == 0)
        {
            _poweroff();
        }
        else if (strcmp(buf, poweroff_options[1]) == 0)
        {
            _reboot();
        }
        else if (strlen(buf) > 0)
        {
            _kernel_errr_cmd(1);
        }
        if (line_inf)
            printtext("\f", 0x0f, 0);
    }
    return;
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