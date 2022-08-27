#include "common.h"
#include "stdio.h"

static void bios_putchar(char c) {
  struct regs in;
  regsinit(&in);
  in.ah = 0x0e;
  in.al = c;
  biosint(0x10, &in, NULL);
}

void putchar(char c) {
  bios_putchar(c);
}

void kprint(const char *s) {
  for (; *s != '\0'; s++) {
    putchar(*s);
  }
}
