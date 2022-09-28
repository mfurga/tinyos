#include <arch/ints/idt.h>
#include <arch/gdt.h>
#include <arch/stdio.h>

idt_gate_32_t idt[256];

/* Real mode IDT (Interrupt Vector Table). */
idtr_t idtr16 = { .address = 0, .limit = 1024 - 1 };

/* Protected mode IDT. */
idtr_t idtr32 = { .address = (u32)&idt, .limit = sizeof(idt) - 1 };

void idt_gate_set(u16 no, u32 handler) {
  idt[no].offset1 = (handler & 0xffff);
  idt[no].offset2 = (handler >> 16) & 0xffff;
  idt[no].selector = GDT_CODE_SEG_32;
  idt[no].type = IDT_GATE_INT_32;
  idt[no].dpl = 0;
  idt[no].present = 1;

  idt[no]._reserved = 0;
  idt[no]._zero = 0;
}

void idt_setup(void) {
  idt_gate_set(0, (u32)isr0);
  idt_gate_set(1, (u32)isr1);
  idt_gate_set(2, (u32)isr2);
  idt_gate_set(3, (u32)isr3);
  idt_gate_set(4, (u32)isr4);
  idt_gate_set(5, (u32)isr5);
  idt_gate_set(6, (u32)isr6);
  idt_gate_set(7, (u32)isr7);
  idt_gate_set(8, (u32)isr8);
  idt_gate_set(9, (u32)isr9);
  idt_gate_set(10, (u32)isr10);
  idt_gate_set(11, (u32)isr11);
  idt_gate_set(12, (u32)isr12);
  idt_gate_set(13, (u32)isr13);
  idt_gate_set(14, (u32)isr14);
  idt_gate_set(15, (u32)isr15);
  idt_gate_set(16, (u32)isr16);
  idt_gate_set(17, (u32)isr17);
  idt_gate_set(18, (u32)isr18);
  idt_gate_set(19, (u32)isr19);
  idt_gate_set(20, (u32)isr20);
  idt_gate_set(21, (u32)isr21);
  idt_gate_set(22, (u32)isr22);
  idt_gate_set(23, (u32)isr23);
  idt_gate_set(24, (u32)isr24);
  idt_gate_set(25, (u32)isr25);
  idt_gate_set(26, (u32)isr26);
  idt_gate_set(27, (u32)isr27);
  idt_gate_set(28, (u32)isr28);
  idt_gate_set(29, (u32)isr29);
  idt_gate_set(30, (u32)isr30);
  idt_gate_set(31, (u32)isr31);

  __asm__ __volatile__("lidt [%0]" : : "r" (&idtr32));
}

