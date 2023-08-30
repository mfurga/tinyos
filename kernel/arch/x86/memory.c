#include <tinyos/common/common.h>
#include <tinyos/common/utils.h>
#include <tinyos/kernel/printk.h>

#include "mtrr.h"

void set_memory_cache_for_fb(u64 paddr, u32 size) {
  if (!is_mtrr_supported()) {
    printk("WARNING: MTRRs are not supported\n");
    return;
  }

  int no = find_free_mtrr();
  if (no < 0) {
    printk("WARNING: No free MTRR to set cache policy for framebuffer\n");
    return;
  }

  size = roundup_next_power_of_2(size);

  set_mtrr((u8)no, paddr, size, MEM_TYPE_WC);
}
