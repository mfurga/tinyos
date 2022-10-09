#include <arch/a20.h>
#include <arch/common.h>
#include <arch/ints/idt.h>
#include <arch/stdio.h>

#include <kernel/kernel.h>

void NORETURN setup(void) {
  enable_a20();
  idt_setup();

  kprintf("ala ma kota: %c\n", 'A');

  int r = 0;
  __asm__ __volatile__(
    "div edx;"
    : "=d"(r)
  );

  //__asm__ __volatile__("int3;");

  kernel_main();
}

