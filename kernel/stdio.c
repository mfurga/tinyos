#include <kernel/stdio.h>
#include <lib/printf.h>

#include <drivers/serial.h>
#include <drivers/vga.h>

static void putc(int ch, int *cnt) {
  vga_putc(ch);
  serial_write(ch);
  (*cnt)++;
}

int vprintf(const char *fmt, va_list ap) {
  int cnt = 0;

  vprintfmt((void *)putc, &cnt, fmt, ap);

  return cnt;
}

int printf(const char *fmt, ...) {
  va_list ap;
  int cnt;

  va_start(ap, fmt);
  cnt = vprintf(fmt, ap);
  va_end(ap);

  return cnt;
}

