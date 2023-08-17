#include <tinyos/kernel/terminal.h>

static const struct terminal *terminals[8];
static unsigned terminal_idx;

void register_terminal(const struct terminal *term) {
  ASSERT(terminal_idx < ARRAY_SIZE(terminals));
  terminals[terminal_idx++] = term;
}

void early_init_terminals(void) {
  for (unsigned i = 0; i < terminal_idx; i++) {
    const struct terminal *t = terminals[i];
    t->init();
  }
}

void terminal_write(const char *buf, size_t len) {
  /* Write to all available terminals */

  for (unsigned i = 0; i < terminal_idx; i++) {
    const struct terminal *t = terminals[i];
    t->write(buf, len);
  }
}
