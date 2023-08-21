#include "common.h"

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
  serial_putchar(c);
}

void puts(const char *s) {
  for (; *s != '\0'; s++) {
    putchar(*s);
  }
}

static void print_uint(unsigned int n) {
  char buff[11] = {0};
  int idx = 9;

  if (n == 0) {
    putchar('0');
    return;
  }

  while (n > 0) {
    buff[idx--] = '0' + (n % 10);
    n /= 10;
  }

  puts(buff + idx + 1);
}

static void print_int(int n) {
  char buff[11] = {0};
  int idx = 9;

  if (n == 0) {
    putchar('0');
    return;
  }

  /* Undefined behavior when calculating the absolute value of INT_MIN. */
  if (n == -2147483648) {
    puts("-2147483648");
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

  puts(buff + idx + 1);
}

static void print_hex(unsigned int n) {
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

  puts(buff + idx + 1);
}

void vprintf(const char *fmt, va_list ap) {
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
        print_int(va_arg(ap, int));
      break;

      case 'u':
        print_uint(va_arg(ap, unsigned int));
      break;

      case 'x':
        print_hex(va_arg(ap, unsigned int));
      break;

      case 's':
        puts(va_arg(ap, const char *));
      break;

      case 'c':
        putchar((char)va_arg(ap, unsigned));
      break;

      case '\0':
      continue;
    }
    p++;
  }

  va_end(ap);
}

void printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
}
