#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/printk.h>

static bool panic = false;

bool in_panic(void) {
  return panic;
}

void NORETURN _panic(const char *func,
                     const char *file,
                     int line,
                     const char *fmt,
                     ...) {
  panic = true;

  va_list ap;
  va_start(ap, fmt);

  printk(
    "\n"
    "=== KERNEL PANIC ===\n"
    "Location : %s(), %s:%u\n\n", func, file, line);
  vprintk(fmt, ap);
  printk("\n\n");

  va_end(ap);
  for (;;);
}


/*
void NORETURN kernel_panic(const int_context_t *context) {

  printf(
    "\n"
    "=== KERNEL PANIC ===\n"
    "Exception: %2xh  error_code: %2xh\n\n"
    "cs:     %2xh\n"
    "eip:    %8xh\n"
    "eflags: %8xh\n"
    "eax: %8xh  ebx: %8xh  ecx: %8xh  edx: %8xh\n"
    "esi: %8xh  edi: %8xh  ebp: %8xh  esp: %8xh\n"
    "\n",
    context->int_no, context->error_code,
    context->cs, context->eip, context->eflags,
    context->eax, context->ebx, context->ecx, context->edx,
    context->esi, context->edi, context->ebp, context->esp
  );

  for (;;);
}
*/

