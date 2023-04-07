#include <drivers/cmos.h>
#include <lib/x86.h>

#define CMOS_ADDRESS_PORT 0x70
#define CMOS_DATA_PORT    0x71

extern u8 nmi_disabled;

u8 cmos_read(u8 reg) {
  reg &= 0x7f;

  outb(CMOS_ADDRESS_PORT, reg | 0x80 * nmi_disabled);
  io_delay();

  return inb(CMOS_DATA_PORT);
}

void cmos_write(u8 reg, u8 data) {
  reg &= 0x7f;

  outb(CMOS_ADDRESS_PORT, reg | 0x80 * nmi_disabled);
  io_delay();
  outb(CMOS_DATA_PORT, data);
}

