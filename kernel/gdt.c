#include <kernel/gdt.h>

#define GDT_TYPE_CODE_RE (10)
#define GDT_TYPE_DATA_RW (2)

#define GDT_SYSTEM       (0 << 4)
#define GDT_CODE_OR_DATA (1 << 4)

#define GDT_DPL_RING_0   (0 << 5)
#define GDT_DPL_RING_1   (1 << 5)
#define GDT_DPL_RING_2   (2 << 5)
#define GDT_DPL_RING_3   (3 << 5)

#define GDT_PRESENT      (1 << 7)
#define GDT_AVL          (1 << 8)
#define GDT_LONG         (1 << 9)

#define GDT_OP_SIZE_16   (0 << 10)
#define GDT_OP_SIZE_32   (1 << 10)

#define GDT_GRANULARITY  (1 << 11)

#define GDT_ENTRY(base, limit, flags)       \
  {                                         \
    .base_0_15 = (base) & 0xffff,           \
    .base_16_23 = ((base) >> 16) & 0xff,    \
    .base_24_31 = ((base) >> 24) & 0xff,    \
    .limit_0_15 = (limit) & 0xffff,         \
    .limit_16_19 = ((limit) >> 16) & 0xf,   \
    .flags_0_7 = (flags) & 0xff,            \
    .flags_8_11 = ((flags) >> 8) & 0xf      \
  }

static gdt_entry_t gdt[] ALIGNED(16) = {
  /* Null segment descriptor. */
  [GDT_ENTRY_NULL] = GDT_ENTRY(0, 0, 0),

  /* 32-bit code segment descriptor, flat (4GB), R/E, Ring 0. */
  [GDT_ENTRY_CODE] = GDT_ENTRY(0, 0xfffff, GDT_TYPE_CODE_RE |
                                           GDT_CODE_OR_DATA |
                                           GDT_DPL_RING_0 |
                                           GDT_PRESENT |
                                           GDT_OP_SIZE_32 |
                                           GDT_GRANULARITY),

  /* 32-bit data segment descriptor, flat (4GB), R/W, Ring 0. */
  [GDT_ENTRY_DATA] = GDT_ENTRY(0, 0xfffff, GDT_TYPE_DATA_RW |
                                           GDT_CODE_OR_DATA |
                                           GDT_DPL_RING_0 |
                                           GDT_PRESENT |
                                           GDT_OP_SIZE_32 |
                                           GDT_GRANULARITY)
};

/* GDTR. */
static gdtr_t gdtr32 = { .address = (u32)&gdt, .limit = sizeof(gdt) - 1 };

void gdt_setup(void) {
  __asm__ __volatile__(
    "lgdt %0;"
    "mov ds, ax;"
    "mov ss, ax;"
    "mov es, ax;"
    "mov fs, ax;"
    "mov gs, ax;"
    "ljmp " STR(GDT_CODE_SEG32) ", 1f;"
    "1:"
    : : "m" (gdtr32), "a"(GDT_DATA_SEG32)
  );
}
