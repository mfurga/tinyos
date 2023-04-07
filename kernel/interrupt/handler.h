#pragma once

#include <lib/common.h>

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

void int_handler_register(u8 excp_no, int_handler_t handler);

