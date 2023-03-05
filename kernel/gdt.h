#pragma once

#define GDT_ENTRY_NULL 0
#define GDT_ENTRY_CODE 1
#define GDT_ENTRY_DATA 2

#define GDT_CODE_SEG32 ((GDT_ENTRY_CODE) * 8)
#define GDT_DATA_SEG32 ((GDT_ENTRY_DATA) * 8)

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
