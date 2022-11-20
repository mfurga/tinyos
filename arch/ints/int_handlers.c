#include <arch/ints/int_handlers.h>
#include <arch/ints/pic.h>
#include <arch/stdio.h>

static int_handler_t handlers[256];

CDECL void isr_handler(int_context_t *context) {
  kprintf("ISR interrupt %x %x\n", context->int_no, context->error_code);
}

CDECL void irq_handler(int_context_t *context) {
//  kprintf("IRQ interrupt %x %x\n", context->int_no, context->error_code);

  int_handler_t handler = handlers[context->int_no];
  if (handler != 0) {
    handler(context);
  }

  if (context->irq_no >= 8) {
    outb(PIC_SLAVE_COMMNAD, PIC_EOI);
  }
  outb(PIC_MASTER_COMMNAD, PIC_EOI);
}

void int_handler_register(u8 int_no, int_handler_t handler) {
  handlers[int_no] = handler;
}

