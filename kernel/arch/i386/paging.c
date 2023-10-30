#include <tinyos/kernel/hal.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/exception.h>
#include <tinyos/kernel/paging.h>
#include <tinyos/common/common.h>
#include <tinyos/common/string.h>

#include "paging.h"

static page_dir_t page_dir ALIGNED(4096);

static void init_page_dir(void) {
  bzero(&page_dir, sizeof(page_dir));
}

static int map_page(void *vaddr, u32 paddr, u32 flags) {
  const int pd_index = ((u32)vaddr >> PAGE_DIR_SHIFT) & PAGE_DIR_MASK;
  const int pt_index = ((u32)vaddr >> PAGE_TAB_SHIFT) & PAGE_TAB_MASK;

  page_tab_t *ptab = KERNEL_PA_TO_VA(
    page_dir.entries[pd_index].ptaddr << PAGE_TAB_SHIFT);

  if (unlikely(KERNEL_VA_TO_PA(ptab)) == 0) {
    ptab = kzmalloc(sizeof(page_tab_t));
    if (ptab == NULL) {
      return -1;
    }

    assert(IS_PAGE_TAB_ALIGNED(ptab));

    page_dir.entries[pd_index].ptaddr = PAGE_ENTRY_PRESENT |
                                        PAGE_ENTRY_RW |
                                        KERNEL_VA_TO_PA(ptab);
  }

  ptab->entries[pt_index].raw = PAGE_ENTRY_PRESENT |
                                flags |
                                paddr;
}

static void page_fault_handler(int_context_t *ctx) {
  const bool present = !!(ctx->error_code & PAGE_FAULT_PRESENT);
  const bool rw = !!(ctx->error_code & PAGE_FAULT_RW);
  const bool us = !!(ctx->error_code & PAGE_FAULT_US);

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