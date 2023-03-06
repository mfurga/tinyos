#include <kernel/common.h>
#include <kernel/boot_params.h>
#include <kernel/video.h>
#include <kernel/ints/idt.h>
#include <kernel/pit.h>
#include <kernel/gdt.h>
#include <kernel/cpu.h>
#include <kernel/keyboard.h>

#include <drivers/rtc.h>

void CDECL NORETURN kernel_main(const boot_params_t *params) {

  video_init(params->video.mode,
             params->video.x,
             params->video.y,
             params->video.cols,
             params->video.lines);

  printf("Kernel starting ...\n");

  idt_setup();

  gdt_setup();

  sti();

  pit_init(100);

  keyboard_init();

  if (!cpuid_check()) {
    printf("CPUID instruction is not supported!\n");
    for(;;);
  }

  rtc_datetime_t datetime = rtc_read_datetime();

  printf("%d %d %d %d:%d:%d\n",
    datetime.day,
    datetime.month,
    datetime.year,
    datetime.hour,
    datetime.minute,
    datetime.second);

  for (;;);
}

