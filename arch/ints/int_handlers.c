#include <arch/ints/int_handlers.h>
#include <arch/stdio.h>

CDECL void isr_handler(int_context_t *context) {
  kprintf("ISR interrupt %x %x\n", context->int_no, context->error_code);
}

CDECL void irq_handler(int_context_t *context) {
  kprintf("IRQ interrupt %x %x\n", context->int_no, context->error_code);
}

