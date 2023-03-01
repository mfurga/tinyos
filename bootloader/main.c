#include "common.h"
#include "a20.h"

void NORETURN main(void) {
  INFO("Starting bootloader stage 2 ...");

  if (enable_a20()) {
    OK("A20 successfully enabled.");
  } else {
    FATAL("Failed to enable A20.");
  }

  INFO("Loading kernel ...");
  load_kernel();
}

