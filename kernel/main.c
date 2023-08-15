#include <tinyos/kernel/hal.h>
#include <tinyos/kernel/irq.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/kernel/panic.h>

#include <tinyos/drivers/serial.h>

#include <multiboot.h>

extern int __CTORS_START_PADDR;
extern int __CTORS_END_PADDR;

void parse_multiboot_info(u32 magic,
                          struct multiboot_info *info) {
  UNUSED(info);
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
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

void CDECL NORETURN kernel_main(u32 magic,
                                struct multiboot_info *info) {
  UNUSED(magic);
  UNUSED(info);

  call_kernel_ctors();
  early_init_serial();

  init_cpu_exception_handling();
  init_irq_handling();

  // init_segmentation();

  for (;;);
}

