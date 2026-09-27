#include "kernel.h"

typedef unsigned char UINT8;

static UINT16 pos = 0;
static int cur_pos_x = 0;
static int cur_pos_y = 0;

// set actual cursor position based on its expected position useing "_" for now
void
outb (UINT16 port, UINT8 val)
{
	asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// implement a cursor
void
set_cursor (int x, int y)
{
	cur_pos_x = x;
	cur_pos_y = y;
	UINT16 pos = y * 80 + x;

	outb (0x3D4, 0x0F);
	outb (0x3D5, (UINT8)(pos & 0xFF));

	outb (0x3D4, 0x0E);
	outb (0x3D5, (UINT8)((pos >> 8) & 0xFF));
}

// calculate cursor position
void
sync_cursor_with_buffer ()
{
	static int last_frame = -1;

	for (int i = (80 * 25) - 1; i >= 0; i--)
		{
			char character = (char)(TERMINAL_BUFFER[i] & 0xFF);

			if (character != ' ' && character != '\0')
				{
					last_frame = i;
					break;
				}
		}

	int next_index = last_frame + 1;
	if (next_index >= 80 * 25)
		{
			next_index = (80 * 25) - 1;
		}

	set_cursor (next_index % 80, next_index / 80);
}

void
putchar (char ch, int color)
{
	TERMINAL_BUFFER[pos] = (UINT16)ch | (UINT16)color << 8;
	if (ch == '\n')
		{
			cur_pos_x = 0;
			cur_pos_y++;
			pos = cur_pos_y * 80;
		}
	else
		{
			TERMINAL_BUFFER[pos] = (UINT16)ch | (UINT16)color << 8;
			cur_pos_x++;
			pos++;
		}
}

void
print_text (char *s, int color)
{
	while (*s)
		{
			putchar (*s, color);

			s++;
		}
}

void
KERNEL_MAIN (void)
{
	TERMINAL_BUFFER = (UINT16 *)VGA_ADDRESS;

	char *welcome_msg = "Hello Shark\nSEX";

	print_text (welcome_msg, WHITE_COLOR);
	sync_cursor_with_buffer ();
}
