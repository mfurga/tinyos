#include <tinyos/kernel/module.h>
#include <tinyos/kernel/irq.h>
#include <tinyos/kernel/printk.h>

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
  UNUSED(c);
}

void kb_init(void) {
  irq_hander_register(X86_IRQ_KB, keyboard_handler);
}

static const struct module kb_module = {
  .name = "kb",
  .priority = 0,
  .init = &kb_init
};

REGISTER_MODULE(&kb_module);
