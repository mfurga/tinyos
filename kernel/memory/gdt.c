#include <kernel/memory/gdt.h>
#include <kernel/string.h>
#include <kernel/stdio.h>

#define GDT_TYPE_CODE_RE     0x1a  /* code or data, 10 */
#define GDT_TYPE_DATA_RW     0x12  /* code or data, 2 */
#define GDT_TYPE_TSS_32_AVL  0x09  /* system, 9 */
#define GDT_TYPE_TSS_32_BUSY 0x0b  /* system, 11 */

#define GDT_DPL_RING_0       (0 << 5)
#define GDT_DPL_RING_1       (1 << 5)
#define GDT_DPL_RING_2       (2 << 5)
#define GDT_DPL_RING_3       (3 << 5)

#define GDT_PRESENT          (1 << 7)
#define GDT_AVL              (1 << 8)
#define GDT_LONG             (1 << 9)
#define GDT_OP_SIZE_16       (0 << 10)
#define GDT_OP_SIZE_32       (1 << 10)
#define GDT_GRANULARITY      (1 << 11)

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

static void gdt_entry(u32 no, u32 base, u32 limit, u32 flags);
static inline void tss_flush(void);
static inline void gdt_flush(void);
static void tss_setup(void);

static tss_t tss ALIGNED(16);

static gdt_entry_t gdt[] ALIGNED(16) = {
  /* Null segment descriptor. */
  [GDT_ENTRY_NULL] = GDT_ENTRY(0, 0, 0),

  /* 32-bit code segment descriptor, flat (4GB), R/E, Ring 0. */
  [GDT_ENTRY_KERNEL_CODE] = GDT_ENTRY(0, 0xfffff, GDT_TYPE_CODE_RE |
                                                  GDT_DPL_RING_0 |
                                                  GDT_PRESENT |
                                                  GDT_OP_SIZE_32 |
                                                  GDT_GRANULARITY),

  /* 32-bit data segment descriptor, flat (4GB), R/W, Ring 0. */
  [GDT_ENTRY_KERNEL_DATA] = GDT_ENTRY(0, 0xfffff, GDT_TYPE_DATA_RW |
                                                  GDT_DPL_RING_0 |
                                                  GDT_PRESENT |
                                                  GDT_OP_SIZE_32 |
                                                  GDT_GRANULARITY),

  /* 32-bit code segment descriptor, flat (4GB), R/E, Ring 3. */
  [GDT_ENTRY_USER_CODE] = GDT_ENTRY(0, 0xfffff, GDT_TYPE_CODE_RE |
                                                GDT_DPL_RING_3 |
                                                GDT_PRESENT |
                                                GDT_OP_SIZE_32 |
                                                GDT_GRANULARITY),

  /* 32-bit data segment descriptor, flat (4GB), R/W, Ring 3. */
  [GDT_ENTRY_USER_DATA] = GDT_ENTRY(0, 0xfffff, GDT_TYPE_DATA_RW |
                                                GDT_DPL_RING_3 |
                                                GDT_PRESENT |
                                                GDT_OP_SIZE_32 |
                                                GDT_GRANULARITY),

  /* Task State Segment (TSS) descriptor. Runtime init by gdt_setup. */
  [GDT_ENTRY_TSS] = GDT_ENTRY(0, 0, 0)
};

/* GDTR. */
static gdtr_t gdtr = { .address = (u32)&gdt, .limit = sizeof(gdt) - 1 };

void gdt_setup(void) {
  /* Runtime GDT entries initialization. */
  gdt_entry(GDT_ENTRY_TSS, (u32)&tss, sizeof(tss), GDT_TYPE_TSS_32_AVL |
                                                   GDT_DPL_RING_0 |
                                                   GDT_PRESENT |
                                                   GDT_OP_SIZE_32);
  tss_setup();

  /* Flush memory-management registers. Init selectors. */
  gdt_flush();

  tss_flush();
}

void print_tss(void) {

  printf(
    "TSS:\n"
    "cs:     %02xh\n"
    "eip:    %08xh\n"
    "eflags: %08xh\n"
    "eax: %08xh  ebx: %08xh  ecx: %08xh  edx: %08xh\n"
    "esi: %08xh  edi: %08xh  ebp: %08xh  esp: %08xh\n"
    "\n",
    tss.cs, tss.eip, tss.eflags,
    tss.eax, tss.ebx, tss.ecx, tss.edx,
    tss.esi, tss.edi, tss.ebp, tss.esp
  );

}

static void tss_setup(void) {
  memset(&tss, 0, sizeof(tss));

  tss.ss0 = GDT_SEL_KERNEL_DATA | GDT_SEL_RPL_0;
  tss.esp0 = 0x7c00;  /* TODO: Change. */
}

static inline void gdt_flush(void) {
  __asm__ __volatile__(
    "lgdt %0;"
    "mov ax, " STR(GDT_SEL_KERNEL_DATA | GDT_SEL_RPL_0) ";"
    "mov ds, ax;"
    "mov ss, ax;"
    "mov es, ax;"
    "mov fs, ax;"
    "mov gs, ax;"
    "ljmp " STR(GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0) ", 1f;"
    "1:"
    : : "m" (gdtr)
  );
}

static inline void tss_flush(void) {
  __asm__ __volatile__(
    "mov ax, " STR(GDT_SEL_TSS | GDT_SEL_RPL_0) ";"
    "ltr ax;"
  );
}

static void gdt_entry(u32 no, u32 base, u32 limit, u32 flags) {
  gdt[no].base_0_15 = (base) & 0xffff;
  gdt[no].base_16_23 = ((base) >> 16) & 0xff;
  gdt[no].base_24_31 = ((base) >> 24) & 0xff;
  gdt[no].limit_0_15 = (limit) & 0xffff;
  gdt[no].limit_16_19 = ((limit) >> 16) & 0xf;
  gdt[no].flags_0_7 = (flags) & 0xff;
  gdt[no].flags_8_11 = (flags >> 8) & 0xf;
}

