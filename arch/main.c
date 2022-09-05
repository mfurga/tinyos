#include "stdio.h"
#include "types.h"
#include "a20.h"
#include "common.h"

#include <arch/ints/idt.h>

#include <kernel/kernel.h>

void NORETURN setup(void) {
  enable_a20();

  idt_setup();

  sti();

  __asm__ __volatile__("int3;");

  kernel_main();
}

