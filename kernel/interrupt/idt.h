#pragma once

#include <kernel/common.h>

typedef struct idt_entry_32 {
  u16 offset_0_15;
  u16 segment;
  u8 _zero;  /* Reserved / zero. */
  u8 type : 5;
  u8 dpl : 2;
  u8 present : 1;
  u16 offset_16_31;
} PACKED idt_entry_32_t;

typedef struct idtr {
  u16 limit;
  u32 address;
} PACKED idtr_t;

void idt_setup(void);
void pic_remap(u8 master_offset, u8 slave_offset);

