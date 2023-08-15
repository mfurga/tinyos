#include <tinyos/kernel/hal.h>
#include <tinyos/drivers/serial.h>

void early_init_serial(void) {
  serial_init_port(COM1);
  serial_init_port(COM2);
  serial_init_port(COM3);
  serial_init_port(COM4);
}

