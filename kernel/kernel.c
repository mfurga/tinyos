#include <kernel/common.h>
#include <kernel/boot_params.h>
#include <kernel/ints/idt.h>
#include <kernel/pit.h>
#include <kernel/gdt.h>
#include <kernel/cpu.h>
//#include <kernel/mem/pmm.h>
#include <kernel/stdio.h>

#include <drivers/vga.h>
#include <drivers/serial.h>
#include <drivers/pci.h>
#include <drivers/rtc.h>
#include <drivers/cmos.h>

/*
void print_memory_map(const boot_params_t *params) {
  printf("BIOS-e820 physical RAM map:\n");
  for (u16 i = 0; i < params->memory_entries; i++) {
    printf("[%lx - %lx] %d\n",
      params->memory_map[i].base,
      params->memory_map[i].base + params->memory_map[i].length,
      params->memory_map[i].type);
  }
}
*/

void CDECL NORETURN kernel_main(const boot_params_t *params) {

  /* Console init. */
  serial_init();
  vga_init(params->video.mode,
           params->video.x,
           params->video.y,
           params->video.cols,
           params->video.lines);

  printf("%02o\n", 0x12);

  idt_setup();

  gdt_setup();

  if (!cpuid_check()) {
    //printf("CPUID instruction is not supported!\n");
    for(;;);
  }

  //pmm_init(params->mmap, params->mmap_length);

  //print_memory_map(params);

  /*
  rtc_datetime_t datetime = rtc_read_datetime();

  printf("%d %d %d %d:%d:%d\n",
    datetime.day,
    datetime.month,
    datetime.year,
    datetime.hour,
    datetime.minute,
    datetime.second);
  */

  //pci_probe();

  //int3();

  for (;;);
}

