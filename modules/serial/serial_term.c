#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/hal.h>

#include "serial.h"

static void init(struct term *t) {
  UNUSED(t);
  serial_init_port(COM1);
}

static void serial_write_char(char c) {
  serial_write(COM1, c);
}

static struct sterm sterm = {
  .write_char = &serial_write_char
};

REGISTER_SERIAL_TERMINAL(&init, &sterm);
