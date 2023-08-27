#pragma once

#include <tinyos/common/colors.h>
#include <stdarg.h>

/* Printk control character */
#define PRINTK_CTRL_CHAR '\x01'

/* Controls */
#define PRINTK_CTRL_NO_PREFIX '\x00'
#define PRINTK_NO_PREFIX "\x01\x00"

#define PRINTK_COLOR VGA_MAKE_COLOR(COLOR_GRAY, COLOR_BLACK)

int printk(const char *fmt, ...);

int vprintk(const char *fmt, va_list ap);

