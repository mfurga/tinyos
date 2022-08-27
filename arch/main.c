#include "stdio.h"
#include "types.h"
#include "a20.h"

#include <kernel/kernel.h>

void NORETURN setup(void) {
  kprint("Kernel starting ...\r\n");

  enable_a20();

  kernel_main();
}

