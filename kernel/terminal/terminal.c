#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/panic.h>

static struct term terminals[8];
static size_t term_idx = 0;

void register_terminal(enum terminal_type type,
                       void (*init)(struct term *t),
                       void *term) {
  assert(term_idx < ARRAY_SIZE(terminals));

  terminals[term_idx].type = type;
  terminals[term_idx].init = init;
  terminals[term_idx].is_initialized = false;

  switch (type) {
    case TERMINAL_VIDEO:
      terminals[term_idx].write = &video_terminal_write;
      terminals[term_idx].vt = term;
    break;

    case TERMINAL_SERIAL:
      terminals[term_idx].write = &serial_terminal_write;
      terminals[term_idx].st = term;
    break;
  }

  term_idx++;
}

void early_init_serial_terminal(void) {
  for (size_t i = 0; i < term_idx; i++) {
    struct term *t = &terminals[i];

    if (t->type == TERMINAL_SERIAL) {
      t->init(t);
      t->is_initialized = true;
    }
  }
}

void init_video_terminal(void) {
  for (size_t i = 0; i < term_idx; i++) {
    struct term *t = &terminals[i];

    if (t->type == TERMINAL_VIDEO) {
      t->init(t);
      t->is_initialized = true;
    }
  }
}

void terminal_write(const char *buf, size_t size, u8 color) {
  for (size_t i = 0; i < term_idx; i++) {
    struct term *t = &terminals[i];

    if (t->is_initialized) {
      t->write(t, buf, size, color);
    }
  }
}
