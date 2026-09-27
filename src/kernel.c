#include "kernel.h"

typedef unsigned char UINT8;
typedef unsigned int UINT32;

static UINT16 pos = 0;
static int cur_pos_x = 0;
static int cur_pos_y = 0;

void
scr_memsetw (UINT16 *dest, UINT16 val, UINT32 count)
{
	asm volatile ("cld\n\t"
				  "rep stosw"
				  : "+D"(dest), "+c"(count)
				  : "a"(val)
				  : "memory");
}

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

void
clear_screen (UINT8 color)
{
	UINT16 blank = (UINT16)' ' | ((UINT16)color << 8);
	scr_memsetw ((UINT16 *)VGA_ADDRESS, blank, 80 * 25);

	cur_pos_x = 0;
	cur_pos_y = 0;

	pos = 0;
	set_cursor (0, 0);
}

void
putchar (char ch, int color)
{
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
			if (cur_pos_x >= 80)
				{
					cur_pos_x = 0;
					cur_pos_y++;
				}
		}

	set_cursor (cur_pos_x, cur_pos_y);
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
	clear_screen (WHITE_COLOR);
	char *welcome_msg = "HELO SHARK FOOD IN NEXTLINE\nFOOD";

	print_text (welcome_msg, WHITE_COLOR);
}
