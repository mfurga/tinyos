#include <kernel/exception/idt.h>
#include <kernel/exception/pic.h>
#include <kernel/gdt.h>

static idt_entry_32_t idt[256] ALIGNED(16);

/* IDTR32 */
static idtr_t idtr32 = { .address = (u32)&idt, .limit = sizeof(idt) - 1 };

static void idt_entry_set(u16 no, u32 handler, u16 seg, u8 type) {
  idt[no].offset_lo = (handler & 0xffff);
  idt[no].offset_hi = (handler >> 16) & 0xffff;
  idt[no].segment = seg;
  idt[no].type = type;
  idt[no].dpl = 0;
  idt[no].present = 1;
  idt[no]._zero = 0;
}

void idt_setup(void) {
  idt_entry_set(0, (u32)isr0, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(1, (u32)isr1, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(2, (u32)isr2, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(3, (u32)isr3, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(4, (u32)isr4, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(5, (u32)isr5, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(6, (u32)isr6, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(7, (u32)isr7, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(8, (u32)isr8, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(9, (u32)isr9, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(10, (u32)isr10, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(11, (u32)isr11, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(12, (u32)isr12, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(13, (u32)isr13, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(14, (u32)isr14, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(15, (u32)isr15, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(16, (u32)isr16, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(17, (u32)isr17, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(18, (u32)isr18, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(19, (u32)isr19, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(20, (u32)isr20, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(21, (u32)isr21, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(22, (u32)isr22, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(23, (u32)isr23, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(24, (u32)isr24, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(25, (u32)isr25, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(26, (u32)isr26, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(27, (u32)isr27, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(28, (u32)isr28, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(29, (u32)isr29, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(30, (u32)isr30, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(31, (u32)isr31, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);

  /* 32-255: User defined interrupts. */

  /* Set the master PIC's offset to 0x20 and the slave's to 0x28. */
  pic_remap(0x20, 0x28);

  idt_entry_set(32 + 0, (u32)irq0, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 1, (u32)irq1, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 2, (u32)irq2, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 3, (u32)irq3, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 4, (u32)irq4, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 5, (u32)irq5, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 6, (u32)irq6, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 7, (u32)irq7, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 8, (u32)irq8, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 9, (u32)irq9, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 11, (u32)irq11, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 12, (u32)irq12, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 13, (u32)irq13, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 14, (u32)irq14, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);
  idt_entry_set(32 + 15, (u32)irq15, GDT_SEL_KERNEL_CODE | GDT_SEL_RPL_0, IDT_GATE_INT32);

  __asm__ __volatile__("lidt %0" : : "m" (idtr32));
}

