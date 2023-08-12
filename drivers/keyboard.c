#include <kernel/interrupt/handler.h>
#include <kernel/interrupt/idt.h>
#include <kernel/stdio.h>
#include <lib/common.h>
#include <lib/x86.h>

#define SC_MAX 57

static const char kbdus[] = {
  '?', '?',
  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
  '?', '?',
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
  '?', '?',
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
  '?',
  '\\',
  'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/',
  '?', '?', '?',
  ' '
};

static void keyboard_handler(int_context_t *context) {
  UNUSED(context);

  u8 scancode = inb(0x60);

  if (scancode & 0x80) {
    /* Key release */
    return;
  }

  if (scancode > SC_MAX) {
    return;
  }

  char c = kbdus[scancode];
  printf("%c", c);
  UNUSED(c);
}

void keyboard_init(void) {
  int_handler_register(IRQ1, &keyboard_handler);
}

