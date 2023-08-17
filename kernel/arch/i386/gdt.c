#include <tinyos/common/string.h>
#include <tinyos/kernel/hal.h>

#include "gdt.h"

static void tss_setup(void);

static tss_t tss ALIGNED(16);
static gdt_entry_t gdt[8] ALIGNED(16);

static void gdt_set_entry(u32 no, u32 base, u32 limit, u32 flags) {
  gdt[no].base_0_15 = (base) & 0xffff;
  gdt[no].base_16_23 = ((base) >> 16) & 0xff;
  gdt[no].base_24_31 = ((base) >> 24) & 0xff;
  gdt[no].limit_0_15 = (limit) & 0xffff;
  gdt[no].limit_16_19 = ((limit) >> 16) & 0xf;
  gdt[no].flags_0_7 = (flags) & 0xff;
  gdt[no].flags_8_11 = (flags >> 8) & 0xf;
}

static inline void load_gdt(void) {
  struct {
    u16 limit;
    u32 address;
  } PACKED gdtr = { .address = (u32)&gdt, .limit = sizeof(gdt) - 1 };

  asm_volatile("lgdt %0;"
               "mov ds, %1;"
               "mov ss, %1;"
               "mov es, %1;"
               "mov fs, %1;"
               "mov gs, %1;"
               "ljmp " STR(X86_KERNEL_CODE_SEL) ", 1f;"
               "1:"
               : /* no output */
               : "m" (gdtr), "q" (X86_KERNEL_DATA_SEL)
               : /* no clobber */);
}

static inline void load_tss(void) {
  asm_volatile("ltr ax"
               : /* no output */
               : "a" (X86_TSS_SEL)
               : /* no clobber */);
}

void init_segmentation(void) {
  /* Null segment descriptor. */
  gdt_set_entry(GDT_ENTRY_NULL, 0, 0, 0);

  /* Kernel code segment descriptor, flat (4GB), R/E. */
  gdt_set_entry(GDT_ENTRY_KERNEL_CODE, 0, 0xfffff, GDT_TYPE_CODE_RE |
                                                   GDT_DPL_RING_0 |
                                                   GDT_PRESENT |
                                                   GDT_OP_SIZE_32 |
                                                   GDT_GRANULARITY);

  /* Kernel data segment descriptor, flat (4GB), R/W. */
  gdt_set_entry(GDT_ENTRY_KERNEL_DATA, 0, 0xfffff, GDT_TYPE_DATA_RW |
                                                   GDT_DPL_RING_0 |
                                                   GDT_PRESENT |
                                                   GDT_OP_SIZE_32 |
                                                   GDT_GRANULARITY);

  /* User code segment descriptor, flat (4GB), R/E. */
  gdt_set_entry(GDT_ENTRY_USER_CODE, 0, 0xfffff, GDT_TYPE_CODE_RE |
                                                 GDT_DPL_RING_3 |
                                                 GDT_PRESENT |
                                                 GDT_OP_SIZE_32 |
                                                 GDT_GRANULARITY);

  /* User data segment descriptor, flat (4GB), R/W. */
  gdt_set_entry(GDT_ENTRY_USER_DATA, 0, 0xfffff, GDT_TYPE_DATA_RW |
                                                 GDT_DPL_RING_3 |
                                                 GDT_PRESENT |
                                                 GDT_OP_SIZE_32 |
                                                 GDT_GRANULARITY);

  /* TSS segment descriptor. */
  gdt_set_entry(GDT_ENTRY_TSS, (u32)&tss, sizeof(tss), GDT_TYPE_TSS_32_AVL |
                                                       GDT_DPL_RING_0 |
                                                       GDT_PRESENT |
                                                       GDT_OP_SIZE_32);
  tss_setup();

  /* Flush memory-management registers. Init selectors. */
  load_gdt();

  load_tss();
}

static void tss_setup(void) {
  memset(&tss, 0, sizeof(tss));

  tss.ss0 = X86_KERNEL_DATA_SEL;
  tss.esp0 = 0x7c00;  /* TODO: To change */
}
