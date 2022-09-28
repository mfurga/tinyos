#include <arch/ints/isr.h>
#include <arch/stdio.h>
#include <arch/types.h>

CDECL void int_handler(isrparams_t params) {
  kprintf("Interrupt %x %x\n", params.int_no, params.error_code);
}

