#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/common/colors.h>

void video_terminal_init(struct vterm *vt, u16 rows, u16 cols) {
  assert(vt != NULL);

  vt->cols = cols;
  vt->rows = rows;
  vt->c = 0;
  vt->r = 0;
  vt->bg = COLOR_BLACK;

  /* clear screen */
  for (u8 r = 0; r < vt->rows; r++) {
    vt->clear_row(r, vt->bg);
  }
}

void video_terminal_write(struct term *t, const char *buf, size_t size, u8 color) {
  assert(t->vt != NULL);

  struct vterm *vt = t->vt;
  for (size_t i = 0; i < size; i++) {

    if (unlikely(buf[i] == '\n')) {
      vt->c = 0;
      vt->r++;
    } else if (unlikely(buf[i] == '\r')) {
      vt->c = 0;
    } else {
      vt->write_char_at(vt->r, vt->c, VGA_ENTRY_MAKE(buf[i], color));
      vt->c++;
    }

    if (vt->c >= vt->cols) {
      vt->c = 0;
      vt->r++;
    }

    if (vt->r >= vt->rows) {
      vt->r--;
      vt->scroll_up(vt->bg);
    }
  }

  vt->move_cursor(vt->r, vt->c);
}