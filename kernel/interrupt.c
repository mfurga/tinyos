#include <tinyos/kernel/irq.h>
#include <tinyos/kernel/exception.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/printk.h>

void syscall_entry(int_context_t *ctx) {
  UNUSED(ctx);
}

void irq_entry(int_context_t *ctx) {
  assert(!are_interrupts_enabled());

  /* Call the arch-dependent handler */
  irq_handle(ctx);

  assert(!are_interrupts_enabled());
}

void exception_entry(int_context_t *ctx) {
  /* Call the arch-dependent handler */
  exception_handle(ctx);
}
