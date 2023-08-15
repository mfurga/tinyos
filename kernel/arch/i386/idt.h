#pragma once

#include <tinyos/common/common.h>

#define IRQ_OFFSET        0x20

#define IRQ0              (IRQ_OFFSET + 0)
#define IRQ1              (IRQ_OFFSET + 1)
#define IRQ2              (IRQ_OFFSET + 2)
#define IRQ3              (IRQ_OFFSET + 3)
#define IRQ4              (IRQ_OFFSET + 4)
#define IRQ5              (IRQ_OFFSET + 5)
#define IRQ6              (IRQ_OFFSET + 6)
#define IRQ7              (IRQ_OFFSET + 7)
#define IRQ8              (IRQ_OFFSET + 8)
#define IRQ9              (IRQ_OFFSET + 9)
#define IRQ9              (IRQ_OFFSET + 9)
#define IRQ10             (IRQ_OFFSET + 10)
#define IRQ11             (IRQ_OFFSET + 11)
#define IRQ12             (IRQ_OFFSET + 12)
#define IRQ13             (IRQ_OFFSET + 13)
#define IRQ14             (IRQ_OFFSET + 14)
#define IRQ15             (IRQ_OFFSET + 15)

#define IDT_GATE_TASK     0b00101
#define IDT_GATE_INT16    0b00110
#define IDT_GATE_INT32    0b01110
#define IDT_GATE_TRAP16   0b00111
#define IDT_GATE_TRAP32   0b01111

#define DPL_RING_0        0
#define DPL_RING_3        3


#define SYSCALL_VECTOR    0x80

typedef struct idt_entry_32 {
  u16 offset_0_15;
  u16 segment;
  u8 _zero;  /* Reserved / zero. */
  u8 type : 5;
  u8 dpl : 2;
  u8 present : 1;
  u16 offset_16_31;
} PACKED idt_entry_32_t;

void idt_setup(void);
void pic_remap(u8 master_offset, u8 slave_offset);

