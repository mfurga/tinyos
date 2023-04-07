#pragma once

#include <stdarg.h>

void vprintfmt(void (*putc)(int, void *),
               void *context,
               const char *fmt,
               va_list ap);

