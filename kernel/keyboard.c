#include <kernel/ints/int_handlers.h>
#include <kernel/pit.h>

#define SC_MAX 57

const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};


static void keyboard_handler(int_context_t *context) {
  UNUSED(context);

  u8 scancode = inb(0x60);
  if (scancode > SC_MAX) {
    return;
  }

  // char letter = sc_ascii[(int)scancode];
  //kprintf("%c\n", letter);
}

void keyboard_init(void) {
  int_handler_register(IRQ1, &keyboard_handler);
}


