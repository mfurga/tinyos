#pragma once

#include <tinyos/common/common.h>

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
