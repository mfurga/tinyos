#include "common.h"
#include "a20.h"

boot_params_t boot_params;

void NORETURN main(void) {
  serial_init();

  enable_a20();

  detect_memory_e820();

  set_video();

  load_kernel();
}

