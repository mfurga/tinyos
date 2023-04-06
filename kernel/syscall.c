#include <kernel/interrupt/handler.h>
#include <kernel/interrupt/idt.h>
#include <kernel/stdio.h>

static void syscall_handler(int_context_t *context) {
  printf("SYSCALL!!!\n");
}

void syscall_init(void) {
  int_handler_register(SYSCALL_VECTOR, &syscall_handler);
}

