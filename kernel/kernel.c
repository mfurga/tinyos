#include <kernel/common.h>
#include <kernel/boot_params.h>
#include <kernel/video.h>

void CDECL NORETURN kernel_main(const boot_params_t *params) {

  init_video(params->video.mode,
             params->video.x,
             params->video.y,
             params->video.cols,
             params->video.lines);

  printf("Kernel starting ...\n");

  for (;;);
}

