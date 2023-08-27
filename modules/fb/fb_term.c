#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/panic.h>

#include "fb.h"

static void init(struct term *t) {
  assert(t->vt != NULL);

  u16 rows = fb_height() / fb_font_height();
  u16 cols = fb_width() / fb_font_width();

  video_terminal_init(t->vt, rows, cols);
}

static void vterm_write_char_at(u16 row, u16 col, u16 entry) {
  fb_draw_char(col * fb_font_width(), row * fb_font_height(), entry);
}

static void vterm_clear_row(u16 row, u8 bg) {
  fb_draw_line(row * fb_font_height(), fb_font_height(), fb_color(bg));
}

static void vterm_move_cursor(u16 row, u16 col) {
  UNUSED(row);
  UNUSED(col);
}

static struct vterm vterm = {
  .write_char_at = &vterm_write_char_at,
  .clear_row = &vterm_clear_row,
  .move_cursor = &vterm_move_cursor,
};

void register_fb_terminal(void) {
  register_terminal(TERMINAL_VIDEO, &init, &vterm);
}
