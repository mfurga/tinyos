#pragma once

#define GDT_ENTRY_NULL        0
#define GDT_ENTRY_KERNEL_CODE 1
#define GDT_ENTRY_KERNEL_DATA 2
#define GDT_ENTRY_USER_CODE   3
#define GDT_ENTRY_USER_DATA   4
#define GDT_ENTRY_TSS         5

#define GDT_SEL_RPL_0         0
#define GDT_SEL_RPL_3         3

#define GDT_SEL_KERNEL_CODE ((GDT_ENTRY_KERNEL_CODE) << 3)
#define GDT_SEL_KERNEL_DATA ((GDT_ENTRY_KERNEL_DATA) << 3)
#define GDT_SEL_USER_CODE   ((GDT_ENTRY_USER_CODE) << 3)
#define GDT_SEL_USER_DATA   ((GDT_ENTRY_USER_DATA) << 3)
#define GDT_SEL_TSS         ((GDT_ENTRY_TSS) << 3)

#ifndef __ASSEMBLY__

#include <kernel/common.h>

typedef struct {
  u16 limit_0_15;
  u16 base_0_15;
  u8 base_16_23;
  u8 flags_0_7;
  u8 limit_16_19 : 4;
  u8 flags_8_11 : 4;
  u8 base_24_31;
} PACKED gdt_entry_t;

typedef struct {
  u16 limit;
  u32 address;
} PACKED gdtr_t;

typedef struct {
  u32 prev_tss;
  u32 esp0;
  u32 ss0;
  u32 esp1;
  u32 ss1;
  u32 esp2;
  u32 ss2;
  u32 cr3;
  u32 eip;
  u32 eflags;
  u32 eax;
  u32 ecx;
  u32 edx;
  u32 ebx;
  u32 esp;
  u32 ebp;
  u32 esi;
  u32 edi;
  u32 es;
  u32 cs;
  u32 ss;
  u32 ds;
  u32 fs;
  u32 gs;
  u32 ldt;
  u16 trap;
  u16 iomap_base;
} PACKED tss_t;

void gdt_setup(void);

#endif
