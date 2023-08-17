#include <tinyos/common/printf.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/kernel/hal.h>

#include <tinyos/kernel/terminal.h>

static void printk_flush_no_tty(const char *buf, size_t len) {
  terminal_write(buf, len);
}

int vprintk(const char *fmt, va_list ap) {
  int cnt = 0;

  // vprintfmt((void *)putc, &cnt, fmt, ap);

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

