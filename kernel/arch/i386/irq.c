#include <tinyos/kernel/irq.h>

#include "idt.h"
#include "../x86/pic.h"

extern void (*irq_entry_points[16])(void);

void init_irq_handling(void) {
  init_pic_8259(32, 32 + 8);

  for (u8 i = 0; i < 16; i++) {
    idt_entry_set(32 + i,
                  X86_KERNEL_CODE_SEL,
                  irq_entry_points[i],
                  IDT_GATE_INT32,
                  DPL_RING_0);
  }
}

void irq_hander_register(u8 no, irq_handler_t handler) {
  UNUSED(no);
  UNUSED(handler);
}
