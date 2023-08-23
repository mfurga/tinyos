#include "common.h"

// static void bios_putchar(char c) {
//   struct regs in;
//   regsinit(&in);
//   in.ah = 0x0e;
//   in.al = c;
//   biosint(0x10, &in, NULL);
// }

char getchar(void) {
  struct regs in, out;
  regsinit(&in);
  regsinit(&out);

  in.ah = 0;
  biosint(0x16, &in, &out);

  return out.al;
}

void getline(char *buf, size_t sz) {
  size_t idx = 0;
  char c;
  while ((c = getchar()) != '\r' && idx < sz - 1) {
    buf[idx++] = c;
    putc(c);
  }
  putc('\n');
  buf[idx] = '\0';
}

void putc(char c) {
  if (c & IS_COLOR) {
    console_set_color(c);
    return;
  }
  console_putc(c);
  serial_putchar(c);
}

void puts(const char *s) {
  for (; *s != '\0'; s++) {
    putc(*s);
  }
}

static void print_uint(unsigned int n) {
  char buff[11] = {0};
  int idx = 9;

  if (n == 0) {
    putc('0');
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
    putc('0');
    return;
  }

  /* Undefined behavior when calculating the absolute value of INT_MIN. */
  if (n == -2147483648) {
    puts("-2147483648");
    return;
  }

  if (n < 0) {
    putc('-');
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

  putc('0');
  putc('x');

  if (n == 0) {
    putc('0');
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
      putc(*p);
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
        putc((char)va_arg(ap, unsigned));
      break;

      case '\0':
      continue;
    }
    p++;
  }

  console_set_color(C_GRAY[0]);
  va_end(ap);
}

void printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
}
