#ifndef KERNEL_INTS_ISR_H
#define KERNEL_INTS_ISR_H

#include <kernel/common.h>

#define IRQ0 0x20
#define IRQ1 0x21
#define IRQ2 0x22
#define IRQ3 0x23
#define IRQ4 0x24
#define IRQ5 0x25
#define IRQ6 0x26
#define IRQ7 0x27
#define IRQ8 0x28
#define IRQ9 0x29
#define IRQ10 0x2a
#define IRQ11 0x2b
#define IRQ12 0x2c
#define IRQ13 0x2d
#define IRQ14 0x2e
#define IRQ15 0x2f

typedef struct int_context {
  u32 ds;
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
  u32 int_no;
  union {
    u32 error_code;
    u32 irq_no;
  };
  u32 eip, cs, eflags, ret_esp, ret_ss;  /* Pushed by interrupt. */
} PACKED int_context_t;

typedef void (*int_handler_t)(int_context_t *);

CDECL void isr_handler(int_context_t *context);
CDECL void irq_handler(int_context_t *context);

void int_handler_register(u8 int_no, int_handler_t handler);

#endif  // KERNEL_INTS_ISR_H

