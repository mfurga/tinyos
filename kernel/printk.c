#include <tinyos/common/string.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/hal.h>
#include <tinyos/kernel/timer.h>
#include <tinyos/kernel/panic.h>

#define BUF_SIZE 256
#define PREFIX_BUF_SIZE 16

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

struct vsnprintk_buf {
  char *str;
  size_t size;
  size_t cnt;
};

static void vsnprintk_putc(int ch, struct vsnprintk_buf *buf) {
  if (buf->cnt < buf->size) {
    *buf->str++ = (char)ch;
    buf->cnt++;
  }
}

int vsnprintk(char *buf, size_t size, const char *fmt, va_list ap) {
  struct vsnprintk_buf b = {
    .str = buf,
    .size = size - 1,
    .cnt = 0
  };

  if (buf == NULL || size == 0) {
    return -1;
  }

  vprintfmt((void *)vsnprintk_putc, &b, fmt, ap);

  *b.str = '\0';
  return (int)b.cnt;
}

int snprintk(char *buf, size_t size, const char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  int cnt = vsnprintk(buf, size, fmt, ap);
  va_end(ap);

  return cnt;
}

int vprintk(const char *fmt, va_list ap) {
  static char buf[BUF_SIZE];
  static char prefix_buf[PREFIX_BUF_SIZE];
  int cnt;
  bool prefix = true;

  if (unlikely(fmt[0] == PRINTK_CTRL_CHAR)) {
    switch (fmt[1]) {
      case PRINTK_CTRL_NO_PREFIX:
        prefix = false;
        break;
      default:
        /* Do nothing */
        break;
    };
    fmt += 2;
  }

  if (unlikely(in_panic())) {
    prefix = false;
  }

  if (likely(prefix)) {
    u64 time = timer_get_time();
    cnt = snprintk(prefix_buf, sizeof(prefix_buf), "[%5llu.%06llu] ",
      time / TIMER_SCALE, (time % TIMER_SCALE) / 1000);
    /* TODO: Write to ring buf. */
    terminal_write(prefix_buf, cnt);
  }

  cnt = vsnprintk(buf, sizeof(buf), fmt, ap);
  /* TODO: Write to ring buf. */
  terminal_write(buf, cnt);

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
