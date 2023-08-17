#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/hal.h>

#include "serial.h"

void serial_term_init(void) {
  serial_init_port(COM1);
}

void serial_term_write(const char *buf, size_t len) {
  for (size_t i = 0; i < len; i++) {
    serial_write(COM1, (u8)buf[i]);
  }
}

static const struct terminal serial_term = {
  .type = TERMINAL_SERIAL,
  .init = &serial_term_init,
  .write = &serial_term_write
};

REGISTER_TERMINAL(&serial_term);
