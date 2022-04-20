#include "../include/keyboard.h"
#include "../include/video.h"
#include "../kernel.h"

void scan(char *buf, int n)
{
	char key = 0;
	int i = 0;
	char stat = 1;
	int startpos = get_cursor_pos() % 80;

	while (stat) // '\n is enter press key code'
	{
		key = get_key();
		if (key == 0)
			continue;
		switch (key)
		{
		case 13: // if enter press
			if (gfx_mode == false)
			{
				printtext("\n", 0x0f, 0);
				stat = 0;
			}
			break;
		case 8:
			if (i > 0 && gfx_mode == false)
			{
				int x = get_cursor_pos();
				rm_char_in_pos(x - 1);
				i--;
				buf--;
			}
			break;
		case 37:
			break;
		case 39:
			break;
		case 38:
			break;
		case 40:
			break;
		case 17:
			break;
		case 18:
			break;
		case 46:
			break;
		case 35:
			break;
		default:
			if (i < n - 1 && gfx_mode == false)
			{
				*buf++ = key;
				printchar(key, 0x0f, 0);
				i++;
			}
			break;
		}
	}
	*(buf++) = '\0';
}
