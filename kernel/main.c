#include <tinyos/kernel/printk.h>
#include <tinyos/kernel/panic.h>

#include <tinyos/drivers/vga.h>
#include <tinyos/drivers/serial.h>
#include <tinyos/drivers/keyboard.h>
#include <tinyos/drivers/rtc.h>
#include <tinyos/drivers/pci.h>
#include <tinyos/drivers/e1000.h>

#include <multiboot.h>

extern void user_main(void);

extern int __CTORS_START_PADDR;
extern int __CTORS_END_PADDR;

// void switch_to_usermode(void (*entry_point)(void)) {

//   __asm__ __volatile__(
//     "cli;"
//     "mov ax, " STR(GDT_SEL_USER_DATA | GDT_SEL_RPL_3) ";"
//     "mov ds, ax;"
//     "mov es, ax;"
//     "mov fs, ax;"
//     "mov gs, ax;"

//     "mov eax, esp;"
//     "push " STR(GDT_SEL_USER_DATA | GDT_SEL_RPL_3) ";"
//     "push eax;"
//     "pushf;"
//     "or dword ptr [esp], 0x200;" /* IF = 1 */
//     "push " STR(GDT_SEL_USER_CODE | GDT_SEL_RPL_3) ";"
//     "push ebx;"
//     "iret;"
//     : : "b"(entry_point)
//   );

// }

void parse_multiboot_info(u32 multiboot_magic,
                          struct multiboot_info *multiboot_info) {

  if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    panic("Kernel requires the multiboot-compatible bootloader");
  }
}

void call_kernel_ctors(void) {
  void (*ctor)(void);

  void **ctors_start = (void **)&__CTORS_START_PADDR;
  void **ctors_end = (void **)&__CTORS_END_PADDR;

  for (void **p = ctors_start; p < ctors_end; p++) {
    *(void **)&ctor = *p;
    ctor();
  }
}

void CDECL NORETURN kernel_main(u32 multiboot_magic,
                                struct multiboot_info *multiboot_info) {
  call_kernel_ctors();
  early_init_serial();

  init_cpu_exception_handling();
  init_segmentation();



  // parse_multiboot_info(multiboot_magic, multiboot_info);

  // vga_init(multiboot_info->vbe_mode,
  //          0,
  //          0,
  //          80,
  //          25);


  // pci_init();

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

  for (;;);
}

