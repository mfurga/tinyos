#include "common.h"

#define VIDEO_ADDR ((u16 *)0xb8000)
#define VIDEO_COLS 80
#define VIDEO_ROWS 25

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
  u8 x;
  u8 y;
  u8 color;
} context;

static void text_mode_move_cursor(void) {
  u16 pos = (u16)context.y * VIDEO_COLS + (u16)context.x;

  outb(0x3d4, 0x0e);
  outb(0x3d5, pos >> 8);
  outb(0x3d4, 0x0f);
  outb(0x3d5, pos & 0xff);
}

static void textmode_clear_row(u16 row) {
  assert(row < VIDEO_ROWS);

  u16 entry = (context.color << 8) | ' ';
  memset16(VIDEO_ADDR + VIDEO_COLS * row,
           entry,
           VIDEO_COLS);
}

static void textmode_scroll_up(void) {
  /* FIXME: memmove. */
  memcpy16(VIDEO_ADDR,
           VIDEO_ADDR + VIDEO_COLS,
           (VIDEO_ROWS - 1) * VIDEO_COLS);

  textmode_clear_row(VIDEO_ROWS - 1);
}

static void textmode_fix_position(void) {
  if (context.x >= VIDEO_COLS) {
    context.x = 0;
    context.y++;
  }

  if (context.y >= VIDEO_ROWS) {
    context.y = VIDEO_ROWS - 1;
    textmode_scroll_up();
  }
}

void console_clear_screen(void) {
  u16 entry = (context.color << 8) | ' ';
  memset16(VIDEO_ADDR, entry, VIDEO_ROWS * VIDEO_COLS);
  init_console();
}

void init_console(void) {
  context.x = 0;
  context.y = 0;
  context.color = FG_GRAY | BG_BLACK;
  text_mode_move_cursor();
}

void console_set_color(u8 color) {
  context.color = color & 0xf;
  text_mode_move_cursor();  /* change cursor color */
}

void console_putc(u8 ch) {
  u16 *video = VIDEO_ADDR;

  switch (ch) {
    case '\n':
      context.x = 0;
      context.y++;
    break;

    case '\r':
      context.x = 0;
    break;

    default:
      u16 entry = (context.color << 8) | ch;
      video[context.y * VIDEO_COLS + context.x] = entry;
      context.x++;
    break;
  }

  textmode_fix_position();
  text_mode_move_cursor();
}
