#include <arch/a20.h>
#include <arch/common.h>
#include <arch/ints/idt.h>
#include <arch/stdio.h>

#include <kernel/kernel.h>

void NORETURN setup(void) {
  enable_a20();
  idt_setup();

  __asm__ __volatile__("int3;");

  kernel_main();
}

