#include <kernel/common.h>
#include <kernel/boot_params.h>
#include <kernel/video.h>

void print_memory_map(const boot_params_t *params) {
  printf("BIOS-e820 physical RAM map:\n");

  for (u16 i = 0; i < params->memory_entries; i++) {
    printf("[%lx - %lx] %d\n",
      params->memory_map[i].addr,
      params->memory_map[i].addr + params->memory_map[i].length,
      params->memory_map[i].type);
  }
}

void CDECL NORETURN kernel_main(const boot_params_t *params) {

  init_video(params->video.mode,
             params->video.x,
             params->video.y,
             params->video.cols,
             params->video.lines);

  printf("Kernel starting ...\n");

  print_memory_map(params);

  for (;;);
}

