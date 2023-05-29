#include <kernel/interrupt/handler.h>
#include <kernel/interrupt/pic.h>
#include <kernel/stdio.h>
#include <kernel/panic.h>
#include <lib/x86.h>

static int_handler_t handlers[256];

void CDECL isr_handler(int_context_t *context) {
  int_handler_t handler = handlers[context->int_no];
  if (handler != 0) {
    handler(context);
  } else {
    panic(
      "Exception: %02xh  error_code: %02xh\n\n"
      "cs:     %02xh\n"
      "eip:    %08xh\n"
      "eflags: %08xh\n"
      "eax: %08xh  ebx: %08xh  ecx: %08xh  edx: %08xh\n"
      "esi: %08xh  edi: %08xh  ebp: %08xh  esp: %08xh\n"
      "\n",
      context->int_no, context->error_code,
      context->cs, context->eip, context->eflags,
      context->eax, context->ebx, context->ecx, context->edx,
      context->esi, context->edi, context->ebp, context->esp
    );
  }
}

void CDECL irq_handler(int_context_t *context) {
  if (context->irq_no != 0) {
    //printf("INT no: %x IRQ no: %x\n", context->int_no, context->irq_no);
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

