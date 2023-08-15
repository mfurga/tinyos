#include <tinyos/common/printf.h>
#include <tinyos/common/common.h>
#include <tinyos/common/string.h>

static void printnum(void (*putc)(int, void *),
                     void *context,
                     unsigned long long num,
                     unsigned base,
                     int width,
                     char pad) {
  if (num >= base) {
    printnum(putc, context, num / base, base, width - 1, pad);
  } else {
    while (--width > 0) {
      putc(pad, context);
    }
  }

  putc("0123456789abcdef"[num % base], context);
}

static long long getint(va_list *ap, unsigned long_type) {
  if (long_type >= 2) {
    return va_arg(*ap, long long);
  } else if (long_type == 1) {
    return va_arg(*ap, long);
  } else {
    return va_arg(*ap, int);
  }
}

static unsigned long long getuint(va_list *ap, unsigned long_type) {
  if (long_type >= 2) {
    return va_arg(*ap, unsigned long long);
  } else if (long_type == 1) {
    return va_arg(*ap, unsigned long);
  } else {
    return va_arg(*ap, unsigned int);
  }
}

void vprintfmt(void (*putc)(int, void *),
               void *context,
               const char *fmt,
               va_list ap) {
  char ch;
  char *s;
  int base, width, long_type;
  unsigned long long num;
  char pad;

  while (1) {
    /* Put "normal" character */

    while ((ch = *fmt++) != '%') {
      if (ch == '\0') {
        return;
      }
      putc(ch, context);
    }

    /* Parse %-escape sequence */

    width = 0;
    long_type = 0;
    pad = ' ';

  next_char:
    switch (ch = *fmt++) {

      /* pad with 0's */
      case '0':
        pad = '0';
        goto next_char;

      /* width */
      case '1' ... '9':
        for (; ; fmt++) {
          width = width * 10 + ch - '0';

          ch = *fmt;
          if (ch < '0' || ch > '9') {
            goto next_char;
          }
        }
        break;

      /* long */
      case 'l':
        long_type++;
        goto next_char;

      /* decimal */
      case 'd':
        base = 10;
        num = getint(&ap, long_type);
        if ((long long) num < 0) {
          putc('-', context);
          num = -(long long) num;
        }
        printnum(putc, context, num, base, width, pad);
        break;

      /* unsigned */
      case 'u':
        base = 10;
        goto uint_get_print;

      /* octal (unsigned) */
      case 'o':
        base = 8;
        goto uint_get_print;

      /* pointer (unsigned) */
      case 'p':
        base = 16;
        putc('0', context);
        putc('x', context);
        num = (unsigned long) va_arg(ap, void *);
        printnum(putc, context, num, base, width, pad);
        break;

      /* hex (unsigned) */
      case 'x':
        base = 16;

      uint_get_print:
        num = getuint(&ap, long_type);
        printnum(putc, context, num, base, width, pad);
        break;

      /* string */
      case 's':
        s = va_arg(ap, char *);
        if (s == NULL) {
          s = "(null)";
        }

        for (width -= strlen(s); width > 0; width--) {
          putc(pad, context);
        }

        while ((ch = *s++) != '\0') {
          putc(ch, context);
        }
        break;

      /* char */
      case 'c':
        putc(va_arg(ap, int), context);
        break;

      /* '%' */
      case '%':
        putc(ch, context);
        break;

      /* unknown escape sequence */
      default:
        break;
    }
  }
}

struct snprintf_buf {
  char *str;
  unsigned size;
  unsigned cnt;
};

static void snprintf_putc(int ch, struct snprintf_buf *buf) {
  if (buf->cnt < buf->size) {
    *buf->str++ = ch;
    buf->cnt++;
  }
}

int vsnprintf(char *str, unsigned size, const char *fmt, va_list ap) {
  struct snprintf_buf buf = { .str = str, .size = size - 1, .cnt = 0 };

  if (str == NULL || size == 0) {
    return -1;
  }

  vprintfmt((void *)snprintf_putc, &buf, fmt, ap);

  *buf.str = '\0';
  return (int)buf.cnt;
}

int snprintf(char *str, unsigned size, const char *fmt, ...) {
  va_list ap;
  int cnt;

  va_start(ap, fmt);
  cnt = vsnprintf(str, size, fmt, ap);
  va_end(ap);

  return cnt;
}

