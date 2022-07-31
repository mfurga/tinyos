#include "common.h"
#include "types.h"

void putchar(char c) {
  struct regs in;
  regsinit(&in);

  in.ah = 0x0e;
  in.al = c;
  in.bx = 0;
  biosint(0x10, &in, NULL);
}

void puts(const char *s) {
  for (int i = 0; s[i] != '\0'; i++) {
    putchar(s[i]);
  }
}

