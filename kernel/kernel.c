#include <kernel/boot/boot_params.h>
#include <kernel/interrupt/idt.h>
#include <kernel/memory/gdt.h>
#include <kernel/memory/pmm.h>
#include <kernel/syscall.h>
#include <kernel/common.h>
#include <kernel/stdio.h>
#include <kernel/panic.h>

#include <drivers/vga.h>
#include <drivers/serial.h>

extern void user_main(void);

void switch_to_usermode(void (*entry_point)(void)) {

  __asm__ __volatile__(
    "cli;"
    "mov ax, " STR(GDT_SEL_USER_DATA | GDT_SEL_RPL_3) ";"
    "mov ds, ax;"
    "mov es, ax;"
    "mov fs, ax;"
    "mov gs, ax;"

    "mov eax, esp;"
    "push " STR(GDT_SEL_USER_DATA | GDT_SEL_RPL_3) ";"
    "push eax;"
    "pushf;"
    "or dword ptr [esp], 0x200;" /* IF = 1 */
    "push " STR(GDT_SEL_USER_CODE | GDT_SEL_RPL_3) ";"
    "push ebx;"
    "iret;"
    : : "b"(entry_point)
  );

}

void CDECL NORETURN kernel_main(const boot_params_t *params) {

  /* Console init. */
  serial_init();
  vga_init(params->video.mode,
           params->video.x,
           params->video.y,
           params->video.cols,
           params->video.lines);

  printf("Kernel starting ...\n");

  gdt_setup();

  idt_setup();

  sti();

  pmm_init(params->memory_map,
           params->memory_map_size);

  syscall_init();

  printf("Switching to usermode ...\n");

  switch_to_usermode(user_main);

  for (;;);
}

