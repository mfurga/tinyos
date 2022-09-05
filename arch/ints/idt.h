#ifndef ARCH_INTS_IDT_H
#define ARCH_INTS_IDT_H

#include <arch/types.h>

#define IDT_GATE_TASK 0x5
#define IDT_GATE_INT_16 0x6
#define IDT_GATE_TRAP_16 0x7
#define IDT_GATE_INT_32 0xe
#define IDT_GATE_TRAP_32 0xf

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

typedef struct idt_gate_32 {
  u16 offset1;
  u16 selector;
  u8 _reserved;
  u8 type : 4;
  u8 _zero : 1;
  u8 dpl : 2;
  u8 present : 1;
  u16 offset2;
} PACKED idt_gate_32_t;

typedef struct idtr {
  u16 limit;
  u32 address;
} PACKED idtr_t;

void idt_setup(void);

#endif  // ARCH_INTS_IDT_H

