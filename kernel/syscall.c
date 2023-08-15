// #include <kernel/interrupt/handler.h>
// #include <kernel/interrupt/idt.h>
// #include <kernel/printk.h>

// typedef void (*syscall_handler_t)(int_context_t *);

// static void syscall_print(int_context_t *context);

// enum {
//   SYSCALL_PRINT = 0,
//   __SYSCALL_END
// };

// static syscall_handler_t syscalls[] = {
//   [SYSCALL_PRINT] = &syscall_print
// };

// static void syscall_handler(int_context_t *context) {
//   if (context->eax >= __SYSCALL_END) {
//     return;
//   }
//   syscalls[context->eax](context);
// }

// void syscall_init(void) {
//   int_handler_register(SYSCALL_VECTOR, &syscall_handler);
// }

// static void syscall_print(int_context_t *context) {
//   printk((char *)context->ebx);
//   return;
// }

