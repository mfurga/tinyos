#pragma once

#include <kernel/common.h>

#define X86_INT_DE        0x00
#define X86_INT_DB        0x01
#define X86_INT_NMI       0x02
#define X86_INT_BP        0x03
#define X86_INT_OF        0x04
#define X86_INT_BR        0x05
#define X86_INT_UD        0x06
#define X86_INT_NM        0x07
#define X86_INT_DF        0x08
#define X86_INT_CSO       0x09
#define X86_INT_TS        0x0a
#define X86_INT_NP        0x0b
#define X86_INT_SS        0x0c
#define X86_INT_GP        0x0d
#define X86_INT_PF        0x0e
/* reserved */
#define X86_INT_MF        0x10
#define X86_INT_AC        0x11
#define X86_INT_MC        0x12
#define X86_INT_XM        0x13
#define X86_INT_VR        0x14
#define X86_INT_CP        0x15

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

typedef struct idtr {
  u16 limit;
  u32 address;
} PACKED idtr_t;

void idt_setup(void);
void pic_remap(u8 master_offset, u8 slave_offset);

