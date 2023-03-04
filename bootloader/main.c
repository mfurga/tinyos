#include "common.h"
#include "a20.h"

boot_params_t boot_params;

void NORETURN main(void) {
  OK("Stage 2 loaded.");

  enable_a20();

  detect_memory_e820();

  set_video();

  INFO("Loading kernel ...\n");
  load_kernel();
}

