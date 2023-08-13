#pragma once

#include <stdarg.h>

int printk(const char *fmt, ...);

int vprintk(const char *fmt, va_list ap);

