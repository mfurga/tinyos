#include <tinyos/kernel/irq.h>
#include <tinyos/kernel/panic.h>

#include "idt.h"
#include "../x86/pic.h"

extern void (*irq_entry_points[16])(void);
static irq_handler_t irq_handlers[16];

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

void irq_register_hander(u8 no, irq_handler_t handler) {
  assert(no < ARRAY_SIZE(irq_handlers));
  assert(irq_handlers[no] == NULL);

  irq_handlers[no] = handler;
}

void irq_handle(int_context_t *ctx) {
  assert(ctx->irq_no < ARRAY_SIZE(irq_handlers));

  if (irq_handlers[ctx->irq_no] == NULL) {
    return;
  }

  irq_handlers[ctx->irq_no](ctx);
}
