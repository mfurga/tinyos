#include <tinyos/common/common.h>
#include <tinyos/common/string.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/hal.h>
#include <tinyos/common/colors.h>

#include "textmode_video.h"

static struct textmode tm;

void setup_textmode_from_multiboot(struct multiboot_info *mbi) {
  assert(mbi->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT);

  tm.paddr = mbi->framebuffer_addr;
  tm.cols = mbi->framebuffer_width;
  tm.rows = mbi->framebuffer_height;

  register_textmode_terminal();
}

void textmode_clear_row(u16 row, u8 bg) {
  assert(row < tm.rows);

  u16 entry = VGA_ENTRY_MAKE(' ', bg);
  memset16((u16 *)tm.paddr + tm.cols * row,
           entry,
           tm.cols);
}

void textmode_scroll_up(u8 bg) {
  /* FIXME: memmove. */
  memcpy16((u16 *)tm.paddr,
           (u16 *)tm.paddr + tm.cols,
           (tm.rows - 1) * tm.cols);

  textmode_clear_row(tm.rows - 1, bg);
}

void textmode_move_cursor(u16 row, u16 col) {
  assert(row < tm.rows);
  assert(col < tm.cols);

  u16 pos = row * tm.cols + col;

  outb(0x3d4, 0x0e);
  outb(0x3d5, pos >> 8);
  outb(0x3d4, 0x0f);
  outb(0x3d5, pos & 0xff);
}

void textmode_write_char_at(u16 row, u16 col, u16 entry) {
  assert(row < tm.rows);
  assert(col < tm.cols);

  u16 *video = (u16 *)tm.paddr;
  video[row * tm.cols + col] = entry;
}

u16 textmode_cols(void) {
  return tm.cols;
}

u16 textmode_rows(void) {
  return tm.rows;
}
