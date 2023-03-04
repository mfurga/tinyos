#include <kernel/common.h>
#include <kernel/string.h>
#include <kernel/video.h>

#include <stdarg.h>

#define VRAM_BASE_ADDR 0xb8000

/* Foreground colors */
#define FG_BLACK     0x00
#define FG_BLUE      0x01
#define FG_GREEN     0x02
#define FG_CYAN      0x03
#define FG_RED       0x04
#define FG_PURPLE    0x05
#define FG_BROWN     0x06
#define FG_GRAY      0x07
#define FG_DARK_GRAY 0x08
/* ... */
#define FG_YELLOW    0x0e
#define FG_WHILE     0x0f

/* Background colors */
#define BG_BLACK     0x00
#define BG_BLUE      0x10
#define BG_GREEN     0x20
#define BG_CYAN      0x30
#define BG_RED       0x40
/* ... */
#define BG_YELLOW    0xe0
#define BG_WHITE     0xf0

static struct {
  u8 mode;
  u8 page;
  u8 x;
  u8 y;
  u8 cols;
  u8 lines;
  u8 attr;
} context;

void init_video(u8 video_mode, u8 x, u8 y, u8 cols, u8 lines) {
  context.mode = video_mode;
  context.x = x;
  context.y = y;
  context.cols = cols;
  context.lines = lines;
  context.attr = FG_GRAY | BG_BLACK;
}

static void draw_cursor(void) {
  u16 pos = (u16)context.y * (u16)context.cols + (u16)context.x;

  outb(0x3d4, 0x0e);
  outb(0x3d5, pos >> 8);
  outb(0x3d4, 0x0f);
  outb(0x3d5, pos & 0xff);
}

static void scroll(void) {
  u32 cols = context.cols;
  u32 lines = context.lines;

  void *dst = (void *)VRAM_BASE_ADDR;
  void *src = (void *)(VRAM_BASE_ADDR + cols * 2);
  unsigned n = cols * (lines - 1) * 2;

  /* Move. */
  memcpy(dst, src, n);

  void *s = (void *)(VRAM_BASE_ADDR + n);

  /* Clear last row. */
  memset(s, FG_BLACK | BG_BLACK, cols * 2);
}

static void fix_cursor(void) {
  if (context.x >= context.cols) {
    context.x = 0;
    context.y++;
  }

  if (context.y >= context.lines) {
    context.y = context.lines - 1;
    scroll();
  }
}

static void put_char(u8 ch) {
  u32 x = context.x;
  u32 y = context.y;
  u32 cols = context.cols;

  u32 addr = VRAM_BASE_ADDR + (y * cols + x) * 2;

  switch (ch) {
    case '\n':
      context.x = 0;
      context.y++;
    break;

    case '\r':
      context.x = 0;
    break;

    default:
      *(u8 *)addr = ch;
      *(u8 *)(addr + 1) = context.attr;
      context.x++;
    break;
  }

  fix_cursor();
}

static void printf_dec(int n) {
  char buff[11] = {0};
  int idx = 9;

  if (n == 0) {
    put_char('0');
    return;
  }

  if (n == -2147483648) {
    puts("-2147483648");
    return;
  }

  if (n < 0) {
    put_char('-');
    n = -n;
  }

  while (n > 0) {
    buff[idx--] = '0' + (n % 10);
    n /= 10;
  }

  puts(buff + idx + 1);
}

static void printf_hex(unsigned int n) {
  char buff[9] = {0};
  int idx = 7;

  put_char('0');
  put_char('x');

  if (n == 0) {
    put_char('0');
    return;
  }

  while (n > 0) {
    buff[idx--] = "0123456789abcdef"[n % 16];
    n /= 16;
  }

  puts(buff + idx + 1);
}

static void printf_lhex(long long unsigned int n) {
  char buff[16 + 1] = {0};
  int idx = 15;

  put_char('0');
  put_char('x');

  if (n == 0) {
    put_char('0');
    return;
  }

  while (n > 0) {
    buff[idx--] = "0123456789abcdef"[n % 16];
    n /= 16;
  }

  puts(buff + idx + 1);
}

void putc(u8 ch) {
  put_char(ch);
  draw_cursor();
}

void puts(const char *s) {
  while (*s != '\0') {
    put_char(*s++);
  }
  draw_cursor();
}

void printf(const char *fmt, ...) {
  va_list l;
  va_start(l, fmt);

  u32 long_mode = 0;

  const char *p = fmt;
  while (*p != '\0') {
    if (*p != '%') {
      put_char(*p++);
      continue;
    }

    p++;
    switch (*p) {
      case 'i':
      case 'd':
        printf_dec(va_arg(l, int));
      break;

      case 'l':
        long_mode = 1;
        p++;

        /* fall-through */

      case 'x':
        if (long_mode) {
          printf_lhex(va_arg(l, long long unsigned int));
          long_mode = 0;
        } else {
          printf_hex(va_arg(l, unsigned int));
        }
      break;

      case 's':
        puts(va_arg(l, const char *));
      break;

      case 'c':
        put_char((char)va_arg(l, unsigned));
      break;

      case '\0':
      continue;
    }
    p++;
  }

  va_end(l);
  draw_cursor();
}

