#include <stdio.h>

void *scan(char *buf, long n)
{
	char key = 0;
	int i = 0;
	int i_buf = 0;
	char stat = 1;
	int startpos = get_cursor_pos() % 80;
	while (stat)
	{
		key = keyboard_get_key();
		if (key == 0)
			continue;
		switch (key)
		{
		case '\n':
			printtext("\n", 0x0f, 0);
			stat = 0;
			break;
		case '\b':
			if (i > 0)
			{
				int x = get_cursor_pos();
				rm_char_in_pos(x - 1);
				i--;
				i_buf--;
				buf--;
			}
			break;
		default:
			if (i < n - 1)
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
