#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/panic.h>

void serial_terminal_write(struct term *t, const char *buf, size_t size, u8 color) {
  UNUSED(color);
  assert(t->st != NULL);

  struct sterm *st = t->st;
  for (size_t i = 0; i < size; i++) {
    st->write_char(buf[i]);
  }
}
