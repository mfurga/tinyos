#pragma once

#define GDT_ENTRY_NULL        0
#define GDT_ENTRY_KERNEL_CODE 1
#define GDT_ENTRY_KERNEL_DATA 2
#define GDT_ENTRY_USER_CODE   3
#define GDT_ENTRY_USER_DATA   4

#define GDT_SEL_RPL_0         0
#define GDT_SEL_RPL_3         3

#define GDT_SEL_KERNEL_CODE ((GDT_ENTRY_KERNEL_CODE) << 3)
#define GDT_SEL_KERNEL_DATA ((GDT_ENTRY_KERNEL_DATA) << 3)
#define GDT_SEL_USER_CODE ((GDT_ENTRY_USER_CODE) << 3)
#define GDT_SEL_USER_DATA ((GDT_ENTRY_USER_DATA) << 3)

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

void gdt_setup(void);

#endif
