#include "common.h"
#include "vbe.h"

#include "multiboot.h"

struct multiboot_info mbi;

struct vbe_info_block vbe_info_block;

NORETURN void main(void) {
  early_init_serial();

  enable_a20();

  detect_memory_e820();

  vbe_get_info_block(&vbe_info_block);
  printf("VGE version: %d\n", (u32)vbe_info_block.VbeVersion);

  u16 mode = 0;
  mode |= (1 << 14);
  mode |= 0x18;
  vbe_get_mode_info(mode);

  // vbe_set_video_mode(0x118);

  void *entry = load_kernel();

  /* Jump to the kernel entry point */
  asm_volatile(
    "jmp ecx;"
    : /* no output */
    : "a" (MULTIBOOT_BOOTLOADER_MAGIC),
      "b" (&mbi),
      "c" (entry)
    : /* no clobber */
  );

  for (;;);
}
