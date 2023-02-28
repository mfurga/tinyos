#ifndef KERNEL_INTS_IDT_H
#define KERNEL_INTS_IDT_H

#include <kernel/common.h>

#define IDT_GATE_TASK 0b00101
#define IDT_GATE_INT16 0b00110
#define IDT_GATE_INT32 0b01110
#define IDT_GATE_TRAP16 0b00111
#define IDT_GATE_TRAP32 0b01111

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

typedef struct idt_entry_32 {
  u16 offset_lo;
  u16 segment;
  u8 _zero;  /* Reserved / zero. */
  u8 type : 5;
  u8 dpl : 2;
  u8 present : 1;
  u16 offset_hi;
} PACKED idt_entry_32_t;

typedef struct idtr {
  u16 limit;
  u32 address;
} PACKED idtr_t;

void idt_setup(void);
void pic_remap(u8 master_offset, u8 slave_offset);

#endif  // KERNEL_INTS_IDT_H

