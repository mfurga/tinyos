#include <kernel/common.h>
#include <kernel/boot_params.h>
#include <kernel/ints/idt.h>
#include <kernel/pit.h>
#include <kernel/gdt.h>
#include <kernel/cpu.h>
#include <kernel/stdio.h>
#include <kernel/panic.h>

#include <drivers/vga.h>
#include <drivers/serial.h>

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

  idt_setup();

  gdt_setup();

  panic("Kernel panic test! %x", 0xdeadbeef);

/*
  if (!cpuid_check()) {
    panic("CPUID instruction is not supported!\n");
  }
*/

  for (;;);
}

