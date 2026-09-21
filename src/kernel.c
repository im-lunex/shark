#include "kernel.h"

void
KERNEL_MAIN (void)
{
	TERMINAL_BUFFER = (UINT16 *)VGA_ADDRESS;

	const char *welcome_msg = "Hello Shark";

	static int color = WHITE_COLOR;

	for (int i = 0; welcome_msg[i] != '\0'; i++)
		{

			if (welcome_msg[i] == ' ')
				{
					color = RED;
				}

			TERMINAL_BUFFER[i] = (UINT16)welcome_msg[i] | ((UINT16)color << 8);
		}
}
