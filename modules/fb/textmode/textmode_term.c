#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/common/colors.h>

#include "textmode_video.h"

static void init(struct term *t) {
  assert(t->vt != NULL);

  u16 rows = textmode_rows();
  u16 cols = textmode_cols();

  video_terminal_init(t->vt, rows, cols);
}

static void vterm_write_chat_at(u16 row, u16 col, u16 entry) {
  textmode_write_char_at(row, col, entry);
}

static void vterm_clear_row(u16 row, u8 bg) {
  textmode_clear_row(row, VGA_MAKE_COLOR(0 /* no used */, bg));
}

static void vterm_move_cursor(u16 row, u16 col) {
  textmode_move_cursor(row, col);
}

static void vterm_scroll_up(u8 bg) {
  textmode_scroll_up(VGA_MAKE_COLOR(0 /* no used */, bg));
}

static struct vterm vterm = {
  .write_char_at = &vterm_write_chat_at,
  .clear_row = &vterm_clear_row,
  .scroll_up = &vterm_scroll_up,
  .move_cursor = &vterm_move_cursor
};

void register_textmode_terminal(void) {
  register_terminal(TERMINAL_VIDEO, &init, &vterm);
}
