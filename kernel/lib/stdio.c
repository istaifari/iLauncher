#include "../include/keyboard.h"
#include "../include/video.h"
#include "../kernel.h"

void scan(char *buf, int n)
{
	if (!gfx_mode)
	{
		char key = 0;
		int i = 0;
		int i_buf = 0;
		char stat = 4;
		int startpos = get_cursor_pos() % 80;
		while (stat) // '\n is enter press key code'
		{
			key = GetKey();
			if (key == 0)
				continue;
			switch (key)
			{
			case 13: // if enter press
				if (!gfx_mode)
				{
					printtext("\n", 0x0f, 0);
					stat = 0;
				}
				break;
			case 8:
				if (i > 0 && !gfx_mode)
				{
					int x = get_cursor_pos();
					rm_char_in_pos(x - 1);
					i--;
					i_buf--;
					buf--;
				}
				break;
			case 37:
				/*if (i > 0 && !gfx_mode)
				{
					int y = get_cursor_pos() / 80;
					int x = get_cursor_pos() % 80;
					x--;
					i--;
					set_cursor_pos(x, y);
				}*/
				break;
			case 39:
				/*if (!(i > i_buf - 1) && !gfx_mode)
				{
					int y = get_cursor_pos() / 80;
					int x = get_cursor_pos() % 80;
					x++;
					i++;
					set_cursor_pos(x, y);
				}*/
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
				if (i < n - 1 && !gfx_mode)
				{
					*buf++ = key;
					printchar(key, 0x0f, 0);
					i++;
					i_buf++;
				}
				break;
			}
		}
		*(buf++) = '\0';
	}
}
