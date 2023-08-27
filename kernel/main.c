#include <tinyos/kernel/irq.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/kernel/module.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/terminal.h>
#include <tinyos/kernel/mmap.h>
#include <tinyos/kernel/timer.h>

#include <tinyos/modules/fb.h>

#include <multiboot.h>

void parse_multiboot_info(u32 magic,
                          struct multiboot_info *mbi) {
  printk("Paring multiboot info structure ...\n");

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    panic("Kernel requires the multiboot-compatible bootloader");
  }

  if (!(mbi->flags & MULTIBOOT_INFO_MEM_MAP)) {
    panic("No memory map provided by the bootloader");
  }

  if (mbi->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) {
    printk("Bootloader detected: %s\n", (const char *)mbi->boot_loader_name);
  }

  if (mbi->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO) {
    setup_fb_from_multiboot(mbi);
  }

  init_memory_map((multiboot_memory_map_t *)mbi->mmap_addr, mbi->mmap_length);

  printk("Memory map provided by the bootloader:\n");
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
  early_init_serial_terminal();

  parse_multiboot_info(magic, info);

  init_video_terminal();

  init_exception_handling();
  init_irq_handling();

  init_segmentation();
  init_timer();

  // init_modules();
  // enable_interrupts();

  for (;;);
}

