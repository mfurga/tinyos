#include <kernel/common.h>
#include <kernel/boot_params.h>
#include <kernel/video.h>
#include <kernel/ints/idt.h>
#include <kernel/gdt.h>
#include <kernel/cpu.h>

void CDECL NORETURN kernel_main(const boot_params_t *params) {

  init_video(params->video.mode,
             params->video.x,
             params->video.y,
             params->video.cols,
             params->video.lines);

  printf("Kernel starting ...\n");

  idt_setup();

  gdt_setup();

  if (!cpuid_check()) {
    printf("CPUID instruction is not supported!\n");
    for(;;);
  }

  for (;;);
}

