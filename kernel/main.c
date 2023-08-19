#include <tinyos/kernel/irq.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/kernel/module.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/mmap.h>

#include <multiboot.h>

void parse_multiboot_info(u32 magic,
                          struct multiboot_info *info) {
  printk("Paring multiboot info structure ...\n");

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    panic("Kernel requires the multiboot-compatible bootloader");
  }

  if (!(info->flags & MULTIBOOT_INFO_MEMORY)) {
    panic("No basic memory information provided by the bootloader");
  }

  if (!(info->flags & MULTIBOOT_INFO_MEM_MAP)) {
    panic("No memory map provided by the bootloader");
  }

  if (info->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) {
    // const char *name = (const char *)info->boot_loader_name;
    // printk("Bootloader detected: %s\n", name);
  }

  if (info->flags & MULTIBOOT_INFO_CMDLINE) {
    // const char *cmdline = (const char *)info->cmdline;
    // printk("Cmdline: %s\n", cmdline);
  }

  init_memory_map((multiboot_memory_map_t *)info->mmap_addr, info->mmap_length);
  dump_memory_map();
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
  call_kernel_ctors();
  early_init_terminals();

  parse_multiboot_info(magic, info);

  init_cpu_exception_handling();
  init_irq_handling();

  init_segmentation();

  // init_modules();

  for (;;);
}

