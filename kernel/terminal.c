#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/panic.h>

static const struct terminal *terminals[8];
static size_t terminal_idx;

void register_terminal(const struct terminal *term) {
  assert(terminal_idx < ARRAY_SIZE(terminals));
  terminals[terminal_idx++] = term;
}

void early_init_terminals(void) {
  for (size_t i = 0; i < terminal_idx; i++) {
    const struct terminal *t = terminals[i];
    t->init();
  }
}

void terminal_write(const char *buf, size_t size) {
  /* Write to all available terminals */
  for (size_t i = 0; i < terminal_idx; i++) {
    const struct terminal *t = terminals[i];
    t->write(buf, size);
  }
}