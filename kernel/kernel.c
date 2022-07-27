#include "types.h"
#include "io.h"

void kernel_main(void) {

  outb(0x3d4, 14);
  int p = inb(0x3d5);
  p <<= 8;

  outb(0x3d4, 15);
  p += inb(0x3d5);

  int offset = p * 2;

  char *vga = (char *)0xb8000;
  vga[offset] = 'X';
  vga[offset + 1] = 0x0f;
}

