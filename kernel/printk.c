#include <tinyos/common/printf.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/kernel/hal.h>

#include <tinyos/modules/serial.h>
#include <tinyos/modules/vga.h>

static void putc(int ch, int *cnt) {
  vga_putc(ch);
  serial_write(COM1, ch);
  (*cnt)++;
}

int vprintk(const char *fmt, va_list ap) {
  int cnt = 0;

  vprintfmt((void *)putc, &cnt, fmt, ap);

  return cnt;
}

int printk(const char *fmt, ...) {
  va_list ap;
  int cnt;

  va_start(ap, fmt);
  cnt = vprintk(fmt, ap);
  va_end(ap);

  return cnt;
}

