#include <tinyos/kernel/hal.h>
#include <tinyos/kernel/irq.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/kernel/module.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/terminal.h>

#include <multiboot.h>

void parse_multiboot_info(u32 magic,
                          struct multiboot_info *info) {
  UNUSED(info);
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    panic("Kernel requires the multiboot-compatible bootloader");
  }
}

void call_kernel_ctors(void) {
  /* Start and end of the ctors section provided by the linker. */
  extern int __CTORS_START_PADDR;
  extern int __CTORS_END_PADDR;

  void (*ctor)(void);

  void **ctors_start = (void **)&__CTORS_START_PADDR;
  void **ctors_end = (void **)&__CTORS_END_PADDR;

  for (void **p = ctors_start; p < ctors_end; p++) {
    *(void **)&ctor = *p;
    ctor();
  }
}

NORETURN CDECL void kernel_main(u32 magic,
                                struct multiboot_info *info) {
  UNUSED(magic);
  UNUSED(info);

  call_kernel_ctors();
  early_init_terminals();

  init_cpu_exception_handling();
  init_irq_handling();

  // init_modules();

  for (;;);
}

