#include "common.h"
#include "vbe.h"

#include "multiboot.h"

struct multiboot_info mbi;

bool main_loop_logic(void) {
  char buf[16];
  char c;
  unsigned idx;

  console_clear_screen();

  printf(C_GREEN "\n*** TinyOS bootloader ***\n\n");

  printf("Menu:\n");
  printf("b) Boot kernel\n");
  printf("v) Change video mode | ");
  print_video_mode_info();
  printf("\n");

  printf("> ");
  c = getchar();
  printf("\r");

  switch (c) {
    case 'b':
      return true;
    case 'v':
      print_video_modes();
      printf("> ");
      getline(buf, sizeof(buf));
      idx = str_to_uint(buf);
      select_video_mode(idx);
      break;
    default:
      return false;
  }

  return false;
}

NORETURN void main(void) {
  early_init_serial();

  init_console();

  enable_a20();

  detect_memory_e820();

  vga_set_video_mode(0x03);

  fetch_video_modes();

  /* Main loop */
  while (!main_loop_logic())
    ;

  vbe_set_video_mode();

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
