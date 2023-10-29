#include <tinyos/kernel/hal.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/exception.h>

#define PAGE_FAULT_PRESENT  (1 << 0)
#define PAGE_FAULT_RW       (1 << 1)
#define PAGE_FAULT_US       (1 << 2)

static void page_fault_handler(int_context_t *ctx) {
  bool present = !!(ctx->error_code & PAGE_FAULT_PRESENT);
  bool rw = !!(ctx->error_code & PAGE_FAULT_RW);
  bool us = !!(ctx->error_code & PAGE_FAULT_US);

  u32 vaddr = read_cr2();

  panic("Page fault due to %s @ 0x%x %s",
        rw ? "write" : "read",
        vaddr,
        present ? "" : "(not present)");
}

void init_paging(void) {
  exception_handler_register(X86_EXP_PF, &page_fault_handler);

  // TODO
}