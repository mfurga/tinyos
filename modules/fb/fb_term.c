#include <tinyos/kernel/terminal.h>

#include "fb.h"

static void fb_term_write(const char *s, size_t size) {
  for (size_t i = 0; i < size; i++) {
    // TODO
  }
}

static struct terminal fb_terminal = {
  .type = TERMINAL_VIDEO_GRAPHICAL,
  .init = NULL,
  .write = &fb_term_write
};

void init_fb_terminal(void) {
  register_terminal(&fb_terminal);
  set_curr_video_terminal(&fb_terminal);
}
