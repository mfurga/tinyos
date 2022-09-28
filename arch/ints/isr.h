#ifndef ARCH_INTS_ISR_H
#define ARCH_INTS_ISR_H

#include <arch/types.h>

typedef struct isrparams {
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
  u32 int_no, error_code;
} PACKED isrparams_t;

CDECL void int_handler(isrparams_t params);

#endif  // ARCH_INTS_ISR_H

