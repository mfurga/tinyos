#include <kernel/interrupt/nmi.h>
#include <lib/common.h>
#include <lib/x86.h>

/* CMOS */
#define NMI_ADDRESS_PORT 0x70
#define NMI_DATA_PORT    0x71

u8 nmi_disabled = 0;

void nmi_enable(void) {
  outb(NMI_ADDRESS_PORT, inb(NMI_ADDRESS_PORT) & 0x7f);
  io_delay();
  (void)inb(NMI_DATA_PORT);

  nmi_disabled = 0;
}

void nmi_disable(void) {
  outb(NMI_ADDRESS_PORT, inb(NMI_ADDRESS_PORT) | 0x80);
  io_delay();
  (void)inb(NMI_DATA_PORT);

  nmi_disabled = 1;
}

