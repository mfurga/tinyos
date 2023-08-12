#include <lib/common.h>
#include <lib/x86.h>

#include <drivers/serial/serial.h>

void early_init_serial(void) {
  serial_init_port(COM1);
  serial_init_port(COM2);
  serial_init_port(COM3);
  serial_init_port(COM4);
}

