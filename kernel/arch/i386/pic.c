#include <tinyos/kernel/hal.h>

#include "pic.h"

void pic_remap(u8 master_offset, u8 slave_offset) {
  u8 master_mask = inb(PIC_MASTER_DATA);
  u8 slave_mask = inb(PIC_SLAVE_DATA);

  outb(PIC_MASTER_COMMNAD, PIC_ICW1_INIT | PIC_ICW4_8086);
  io_delay();
  outb(PIC_SLAVE_COMMNAD, PIC_ICW1_INIT | PIC_ICW4_8086);
  io_delay();

  outb(PIC_MASTER_DATA, master_offset);
  io_delay();
  outb(PIC_SLAVE_DATA, slave_offset);
  io_delay();

  outb(PIC_MASTER_DATA, 4);  /* Tell master PIC that a slave is at IRQ2 (0100). */
  io_delay();
  outb(PIC_SLAVE_DATA, 2);  /* Tell slave PIC its cascade identity. */
  io_delay();

  outb(PIC_MASTER_DATA, PIC_ICW4_8086);
  io_delay();
  outb(PIC_SLAVE_DATA, PIC_ICW4_8086);
  io_delay();

  /* Restore saved masks. */
  outb(PIC_MASTER_DATA, master_mask);
  io_delay();
  outb(PIC_SLAVE_DATA, slave_mask);
  io_delay();
}

