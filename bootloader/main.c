#include "common.h"
#include "a20.h"

boot_params_t boot_params;

void print_boot_params(void) {
  SAYF(
    "mode: %d\n"
    "x: %d\n"
    "y: %d\n"
    "cols: %d\n"
    "lines: %d\n",
    boot_params.video.mode,
    boot_params.video.x,
    boot_params.video.y,
    boot_params.video.cols,
    boot_params.video.lines
  );
}

void NORETURN main(void) {
  OK("Stage 2 loaded.");

  enable_a20();

  set_video();

  //print_boot_params();

  INFO("Loading kernel ...\n");
  load_kernel();
}

