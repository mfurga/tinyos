#include <tinyos/kernel/exception.h>
#include <tinyos/kernel/panic.h>

#include "idt.h"

extern void (*exception_entry_points[32])(void);
static exception_handler_t exception_handlers[16];

static void handle_breakpoint(int_context_t *ctx) {
  UNUSED(ctx);
  /* Do nothing */
}

void init_exception_handling(void) {
  /* Set the entries for the x86 exceptions */
  for (u8 i = 0; i < 32; i++) {
    idt_entry_set(i,
                  X86_KERNEL_CODE_SEL,
                  exception_entry_points[i],
                  IDT_GATE_INT32,
                  DPL_RING_0);
  }

  /* Allow `int3` to work from userspace */
  idt_entry_set(X86_EXP_BP,
                X86_KERNEL_CODE_SEL,
                exception_entry_points[X86_EXP_BP],
                IDT_GATE_INT32,
                DPL_RING_3);

  exception_handler_register(X86_EXP_BP, handle_breakpoint);

  load_idt();
}

void exception_handler_register(u8 no, exception_handler_t handler) {
  assert(no < ARRAY_SIZE(exception_handlers));
  assert(exception_handlers[no] == NULL);

  exception_handlers[no] = handler;
}

void exception_handle(int_context_t *ctx) {
  assert(ctx->int_no < ARRAY_SIZE(exception_handlers));

  if (unlikely(exception_handlers[ctx->int_no] == NULL)) {
    panic("Not registered handler for 0x%02x exception", ctx->int_no);
  }

  exception_handlers[ctx->int_no](ctx);
}
