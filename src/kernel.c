#include "kernel.h"

static UINT16
VGA_DefaultEntry (unsigned char to_print)
{
  return (UINT16)to_print | (UINT16)WHITE_COLOR << 8;
}

int cursor_x = 0;
int cursor_y = 0;

void
putchar (char c, int color)
{
  if (c == '\n')
    {
      cursor_x = 0;
      cursor_y++;
    }
  else
    {
      int pos = cursor_y * 80 + cursor_x;
      TERMINAL_BUFFER[pos] = (UINT16)c | (UINT16)color << 8;
      cursor_x++;

      if (cursor_x >= 80)
        {
          cursor_x = 0;
          cursor_y++;
        }
    }

  if (cursor_y >= 25)
    {
      for (int y = 1; y < 25; y++)
        for (int x = 0; x < 80; x++)
          TERMINAL_BUFFER[(y - 1) * 80 + x] = TERMINAL_BUFFER[y * 80 + x];
      for (int x = 0; x < 80; x++)
        TERMINAL_BUFFER[24 * 80 + x] = (UINT16)' ' | (UINT16)color << 8;

      cursor_y = 24;
      cursor_x = 0;
    }
}

void
print (char *s, int color)
{
  while (*s)
    putchar (*s++, color);
}

void
KERNEL_MAIN ()
{
  TERMINAL_BUFFER = (UINT16 *)VGA_ADDRESS;
  print ("Welcome,\n", WHITE_COLOR);
  print ("THE SHARK IS ", RAND);
  print ("HUNGRY!", RED);
}
