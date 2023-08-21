#include "common.h"

NORETURN void _panic(const char *func,
                     const char *file,
                     int line,
                     const char *fmt,
                     ...) {
  va_list ap;
  va_start(ap, fmt);

  printf(
    "\n"
    "*** PANIC ***\n"
    "Location : %s(), %s:%u\n\n", func, file, line);
  vprintf(fmt, ap);
  printf("\n\n");

  va_end(ap);
  for (;;);
}
