#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/panic.h>

static const struct terminal *terminals[8];
static size_t terminal_idx;

static const struct terminal *serial_terminal;
static const struct terminal *video_terminal;

void register_terminal(const struct terminal *term) {
  assert(terminal_idx < ARRAY_SIZE(terminals));
  terminals[terminal_idx++] = term;
}

void early_init_terminals(void) {
  for (size_t i = 0; i < terminal_idx; i++) {
    const struct terminal *t = terminals[i];
    t->init();

    switch (t->type) {
      case TERMINAL_SERIAL:
        serial_terminal = t;
      break;

      case TERMINAL_VIDEO_TEXT:
      case TERMINAL_VIDEO_GRAPHICAL:
        video_terminal = t;
      break;
    }
  }
}

void set_curr_video_terminal(const struct terminal *term) {
  video_terminal = term;
}

void terminal_write(const char *buf, size_t size) {
  if (serial_terminal) {
    serial_terminal->write(buf, size);
  }

  if (video_terminal) {
    video_terminal->write(buf, size);
  }
}
