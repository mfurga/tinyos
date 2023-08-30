#include <tinyos/common/utils.h>
#include <tinyos/kernel/hal.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/printk.h>

#include "mtrr.h"

#define MTRR_VALID (1 << 11)

#define IA32_MTRRCAP 0xfe
#define IA32_MTRR_DEF_TYPE 0x2FF

#define IA32_MTRR_PHYSBASE(n) (0x200 + (n) * 2)
#define IA32_MTRR_PHYSMASK(n) (0x201 + (n) * 2)

struct mtrr_change_ctx {
  u32 cr0;
  u32 cr4;
};

static inline u8 max_phys_addr_size(void) {
  u32 eax, ebx, ecx, edx;
  cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
  return eax & 0xff;
}

static inline u8 var_size_mtrr_count(void) {
  assert(is_mtrr_supported());
  return rdmsr(IA32_MTRRCAP) & 0xff;
}

static u32 disable_cache(void) {
  u32 cr0 = read_cr0();
  u32 saved = cr0;

  /* No-fill Cache Mode. Memory coherency is maintained. */
  cr0 |= CR0_CD;
  cr0 &= ~CR0_NW;

  write_cr0(cr0);
  return saved;
}

static void enable_cache(u32 cr0) {
  write_cr0(cr0);
}

static void enable_mtrrs(void) {
  assert(is_mtrr_supported());

  u64 v = rdmsr(IA32_MTRR_DEF_TYPE);
  v |= (1ull << 11);
  wrmsr(IA32_MTRR_DEF_TYPE, v);
}

static void disable_mtrrs(void) {
  assert(is_mtrr_supported());

  u64 v = rdmsr(IA32_MTRR_DEF_TYPE);
  v &= ~(1ull << 11);
  wrmsr(IA32_MTRR_DEF_TYPE, v);
}

/* Based on: Intel Manulas Vol. 3A, Section: 12.11.7.2 */
static void pre_mtrr_change(struct mtrr_change_ctx *ctx) {
  disable_interrupts();
  ctx->cr4 = read_cr4();

  ctx->cr0 = disable_cache();
  invalidate_cache_with_writeback();
  disable_mtrrs();
  /* TODO: Flush TLBs */
  /* TODO: If multiprocessing: maintain consistency through IPIs */
}

/* Based on: Intel Manulas Vol. 3A, Section: 12.11.7.2 */
static void post_mtrr_change(struct mtrr_change_ctx *ctx) {
  /* TODO: Flush TLBs */
  invalidate_cache_with_writeback();
  enable_mtrrs();
  enable_cache(ctx->cr0);
  write_cr4(ctx->cr4);
  enable_interrupts();
}

bool is_mtrr_supported(void) {
  u32 eax, ebx, ecx, edx;
  cpuid(1, &eax, &ebx, &ecx, &edx);
  return !!((edx >> 12) & 1);
}

int find_free_mtrr(void) {
  assert(is_mtrr_supported());

  for (u8 no = 0; no < var_size_mtrr_count(); no++) {
    u64 mask = rdmsr(IA32_MTRR_PHYSMASK(no));
    if (!((mask >> 11) & 1)) {
      return (int)no;
    }
  }

  return -1;
}

void set_mtrr(u32 no, u64 paddr, u32 size, u8 mem_type) {
  assert(is_mtrr_supported());
  assert(no < var_size_mtrr_count());
  assert(size > 0);
  assert(size == roundup_next_power_of_2(size));

  struct mtrr_change_ctx ctx;
  pre_mtrr_change(&ctx);
  {
    u64 mask = ~((u64)size - 1);

    u64 phys_base = (paddr & ~0xfff) | mem_type;
    u64 phys_mask = mask & ((1ull << max_phys_addr_size()) - 1);
    phys_mask |= MTRR_VALID;

    wrmsr(IA32_MTRR_PHYSBASE(no), phys_base);
    wrmsr(IA32_MTRR_PHYSMASK(no), phys_mask);
  }
  post_mtrr_change(&ctx);
}
