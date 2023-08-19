#include <tinyos/kernel/irq.h>
#include <tinyos/kernel/exception.h>

void irq_entry(int_context_t *ctx) {
  /* Call the arch-dependent handler */
  irq_handle(ctx);
}

void exception_entry(int_context_t *ctx) {
  /* Call the arch-dependent handler */
  exception_handle(ctx);
}
