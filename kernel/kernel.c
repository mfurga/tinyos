#include <kernel/boot/multiboot.h>
#include <kernel/interrupt/idt.h>
#include <kernel/memory/gdt.h>
#include <kernel/memory/pmm.h>
#include <kernel/syscall.h>
#include <kernel/stdio.h>
#include <kernel/panic.h>
#include <lib/x86.h>

#include <drivers/vga.h>
#include <drivers/serial/serial.h>
#include <drivers/keyboard.h>
#include <drivers/rtc.h>
#include <drivers/pci.h>
#include <drivers/e1000.h>

#include <net/ethernet.h>

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

void parse_multiboot_info(u32 multiboot_magic,
                          struct multiboot_info *multiboot_info) {

  if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    panic("Kernel requires the multiboot-compatible bootloader");
  }

}

void CDECL NORETURN kernel_main(u32 multiboot_magic,
                                struct multiboot_info *multiboot_info) {

  early_init_serial();

  parse_multiboot_info(multiboot_magic, multiboot_info);

  vga_init(multiboot_info->vbe_mode,
           0,
           0,
           80,
           25);

  printf("%p\n", multiboot_info->framebuffer_addr);
  printf("w=%d h=%d\n", multiboot_info->framebuffer_width,
                        multiboot_info->framebuffer_height);
  char *cmdline = multiboot_info->cmdline;
  printf("cmdline: %s\n", cmdline);

#if 0

  printf("Kernel starting ...\n");

  gdt_setup();

  idt_setup();

  sti();

  pci_init();

/*
  pmm_init(params->memory_map,
           params->memory_map_size);

  syscall_init();

  keyboard_init();

  rtc_datetime_t datetime = rtc_read_datetime();

  printf("%d %d %d %d:%d:%d\n",
    datetime.day,
    datetime.month,
    datetime.year,
    datetime.hour,
    datetime.minute,
    datetime.second);

  u8 buf[2048];
  int r;

  while (1) {
    r = e1000_receive(buf, sizeof(buf));
    if (r > 0) {
      ethernet_parse(buf, r);
    }
  }

  /*
  printf("Switching to usermode ...\n");

  switch_to_usermode(user_main);
  */

#endif
  for (;;);
}

