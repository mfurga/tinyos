#include <tinyos/common/common.h>
#include <tinyos/common/string.h>
#include <tinyos/kernel/hal.h>

#include <tinyos/modules/vga.h>

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

void vga_init(u8 video_mode, u8 x, u8 y, u8 cols, u8 lines) {
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

  /* FIXME: memmove. */
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

void vga_putc(u8 ch) {
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

  draw_cursor();
}

