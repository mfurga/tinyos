#include <tinyos/kernel/terminal.h>

#include "textmode_video.h"

void video_term_init(void) {
  textmode_init(0, 0);
  textmode_clear_screen();
}

void video_term_write(const char *buf, size_t size) {
  for (size_t i = 0; i < size; i++) {
    textmode_putc((u8)buf[i]);
  }
}

static const struct terminal video_term = {
  .type = TERMINAL_VIDEO_TEXT,
  .init = &video_term_init,
  .write = &video_term_write
};

REGISTER_TERMINAL(&video_term);
