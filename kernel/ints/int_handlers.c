#include <kernel/ints/int_handlers.h>
#include <kernel/ints/pic.h>

static int_handler_t handlers[256];

void CDECL isr_handler(int_context_t *context) {
  UNUSED(context);

  //printf("ISR interrupt %x %x\n", context->int_no, context->error_code);

  // TODO: Remove me.
  //kernel_panic(context);
}

void CDECL irq_handler(int_context_t *context) {
  if (context->irq_no != 0) {
    //printf("IRQ no: %x IRQ no: %x\n", context->int_no, context->irq_no);
  }

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

