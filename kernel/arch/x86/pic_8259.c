/*
 * Based on:
 * http://www.brokenthorn.com/Resources/OSDevPic.html
 */

#include <tinyos/kernel/hal.h>
#include <tinyos/kernel/panic.h>

#define PIC_MASTER_COMMNAD  0x20
#define PIC_MASTER_DATA     0x21
#define PIC_SLAVE_COMMNAD   0xa0
#define PIC_SLAVE_DATA      0xa1

#define ICW1_ICW4           0x01  /* PIC expects to recieve ICW4 */
#define ICW1_SINGLE         0x02  /* Only one PIC in system. If not set ICW3 must
                                     be sent to controller */
#define ICW1_INTERVAL4      0x04  /* Call address interval is 4 (8) */
#define ICW1_LEVEL          0x08  /* Level (edge) triggered mode */
#define ICW1_INIT           0x10  /* Perform initialization */

#define ICW4_8086           0x01  /* Is 80x86 (MCS-80/86) mode */
#define ICW4_AUTO           0x02  /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE      0x08  /* Buffered mode/slave */
#define ICW4_BUF_MASTER     0x0C  /* Buffered mode/master */
#define ICW4_SFNM           0x10  /* Special fully nested (not) */

#define PIC_EOI             0x20

void init_pic_8259(u8 offset1, u8 offset2) {
  assert(!are_interrupts_enabled());

  assert(offset1 % 8 == 0);
  assert(offset2 % 8 == 0);

  /* Send ICW1 (master) */
  outb(PIC_MASTER_COMMNAD, ICW1_INIT | ICW1_ICW4);
  io_delay();

  /* Send ICW2 (master) */
  outb(PIC_MASTER_DATA, offset1);
  io_delay();

  /* Send ICW3 (master) */
  outb(PIC_MASTER_DATA, 1 << 2);
  io_delay();

  /* Send ICW4 (master) */
  outb(PIC_MASTER_DATA, ICW4_8086);
  io_delay();

  /* Send ICW1 (slave) */
  outb(PIC_SLAVE_COMMNAD, ICW1_INIT | ICW1_ICW4);
  io_delay();

  /* Send ICW2 (slave) */
  outb(PIC_SLAVE_DATA, offset2);
  io_delay();

  /* Send ICW3 (slave) */
  outb(PIC_SLAVE_DATA, 2);
  io_delay();

  /* Send ICW4 (master) */
  outb(PIC_SLAVE_DATA, ICW4_8086);
  io_delay();
}

void pic_8259_eoi(u8 irq_no) {
  assert(irq_no < 16);

  if (irq_no >= 8) {
    outb(PIC_SLAVE_COMMNAD, PIC_EOI);
  }

  outb(PIC_MASTER_COMMNAD, PIC_EOI);
}

static void pic_8259_change_imr(u8 irq_no, u8 value) {
  u8 port;
  u8 idx;

  if (irq_no < 8) {
    idx = irq_no;
    port = PIC_MASTER_DATA;
  } else {
    idx = irq_no - 8;
    port = PIC_SLAVE_DATA;
  }

  u8 t = inb(port);
  t ^= (-value ^ t) & (1 << idx);
  outb(port, t);
}

void pic_8259_mask(u8 irq_no) {
  assert(irq_no < 16);

  pic_8259_change_imr(irq_no, 1);
}

void pic_8259_unmask(u8 irq_no) {
  assert(irq_no < 16);

  pic_8259_change_imr(irq_no, 0);
  if (irq_no >= 8) {
    pic_8259_change_imr(2, 0);
  }
}
