#ifndef ARCH_INTS_ISR_H
#define ARCH_INTS_ISR_H

#include <arch/common.h>

typedef struct int_context {
  u32 ds;
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
  u32 int_no, error_code;
  u32 eip, cs, eflags, ret_esp, ret_ss;  /* Pushed by interrupt. */
} PACKED int_context_t;

CDECL void isr_handler(int_context_t *context);
CDECL void irq_handler(int_context_t *context);

#endif  // ARCH_INTS_ISR_H

