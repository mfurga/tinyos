#include <stdarg.h>
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
  if (c == '\n') {
    bios_putchar('\r');
  }
  bios_putchar(c);
}

void kprint(const char *s) {
  for (; *s != '\0'; s++) {
    putchar(*s);
  }
}

static void kprint_dec(int n) {
  char buff[11] = {0};
  int idx = 9;

  if (n == 0) {
    putchar('0');
    return;
  }

  /* Undefined behavior when calculating the absolute value of INT_MIN. */
  if (n == -2147483648) {
    kprint("-2147483648");
    return;
  }

  if (n < 0) {
    putchar('-');
    n = -n;
  }

  while (n > 0) {
    buff[idx--] = '0' + (n % 10);
    n /= 10;
  }

  kprint(buff + idx + 1);
}

static void kprint_hex(unsigned int n) {
  char buff[9] = {0};
  int idx = 7;

  putchar('0');
  putchar('x');

  if (n == 0) {
    putchar('0');
    return;
  }

  while (n > 0) {
    buff[idx--] = "0123456789abcdef"[n % 16];
    n /= 16;
  }

  kprint(buff + idx + 1);
}

void kprintf(const char *fmt, ...) {
  va_list l;
  va_start(l, fmt);

  const char *p = fmt;
  while (*p != '\0') {
    if (*p != '%') {
      putchar(*p);
      p++;
      continue;
    }

    p++;
    switch (*p) {
      case 'i':
      case 'd':
        kprint_dec(va_arg(l, int));
      break;

      case 'x':
        kprint_hex(va_arg(l, unsigned int));
      break;

      case 's':
        kprint(va_arg(l, const char *));
      break;

      case 'c':
        putchar((char)va_arg(l, unsigned));
      break;

      case '\0':
      continue;
    }
    p++;
  }

  va_end(l);
}

