#include <tinyos/common/common.h>
#include <tinyos/kernel/hal.h>

#include "idt.h"

extern void (*exception_entry_points[32])(void);

static idt_entry_32_t idt[256] ALIGNED(16);

static void load_idt(void) {
  struct {
    u16 limit;
    u32 address;
  } PACKED idtr = { .limit = sizeof(idt) - 1, .address = (u32)&idt };

  asm_volatile("lidt %0"
               : /* no output */
               : "m" (idtr)
               : /* no clobber */);
}

void idt_entry_set(u8 no, u16 segment, void *offset, u8 type, u8 dpl) {
  idt[no].offset_0_15 = (u32)offset & 0xffff;
  idt[no].offset_16_31 = (u32)offset >> 16;
  idt[no].segment = segment;
  idt[no].type = type & 0b11111;
  idt[no].dpl = dpl & 0b11;
  idt[no].present = 1;
  idt[no]._zero = 0;
}

void init_cpu_exception_handling(void) {
  /* Set the entries for the x86 exceptions. */
  for (u8 i = 0; i < 32; i++) {
    idt_entry_set(i,
                  X86_KERNEL_CODE_SEL,
                  exception_entry_points[i],
                  IDT_GATE_INT32,
                  DPL_RING_0);
  }

  /* Allow `int3` to work from userspace. */
  idt_entry_set(X86_EXP_BP,
                X86_KERNEL_CODE_SEL,
                exception_entry_points[X86_EXP_BP],
                IDT_GATE_INT32,
                DPL_RING_3);

  load_idt();
}


// void idt_setup(void) {

//   /*
//     Intel vol.3 6.12.1.2:

//     The processor checks the DPL of the interrupt or trap gate only if an
//     exception or interrupt is generated with an INT n, INT3 or INTO instruction.
//     Here, the CPL must be less than or equal to the DPL of the gate. This
//     restriction prevents application programs or procedures running at privilege
//     level 3 from using a software interrupt to access critical exception
//     handlers, such as the page-fault handler, providing that those handlers are
//     placed in more privileged code segments (numerically lower privilege level).
//     For hardware-generated interrupts and processor-detected exceptions,
//     the processor ignores the DPL of interrupt and trap gates.
//   */

//   INTG(X86_INT_DE, (u32)isr_divide_error);
//   INTG(X86_INT_DB, (u32)isr_debug);
//   INTG(X86_INT_NMI, (u32)isr_nmi);

//   SYSG(X86_INT_BP, (u32)isr_breakpoint);

//   INTG(X86_INT_OF, (u32)isr_overflow);
//   INTG(X86_INT_BR, (u32)isr_bound_range_exceeded);
//   INTG(X86_INT_UD, (u32)isr_invalid_opcode);
//   INTG(X86_INT_NM, (u32)isr_device_not_available);
//   INTG(X86_INT_DF, (u32)isr_double_fault);
//   INTG(X86_INT_CSO, (u32)isr_coprocessor_segment_overrun);
//   INTG(X86_INT_TS, (u32)isr_invalid_tss);
//   INTG(X86_INT_NP, (u32)isr_segment_not_present);
//   INTG(X86_INT_SS, (u32)isr_stack_fault);
//   INTG(X86_INT_GP, (u32)isr_general_protection);
//   INTG(X86_INT_PF, (u32)isr_page_fault);

//   SYSG(SYSCALL_VECTOR, (u32)isr_syscall_vector);

//   /*
//     Set the master PIC's offset to IRQ_OFFSET and the slave's
//     to IRQ_OFFSET + 8.
//   */
//   pic_remap(IRQ_OFFSET, IRQ_OFFSET + 8);

//   INTG(IRQ_OFFSET + 0, (u32)irq0);
//   INTG(IRQ_OFFSET + 1, (u32)irq1);
//   INTG(IRQ_OFFSET + 2, (u32)irq2);
//   INTG(IRQ_OFFSET + 3, (u32)irq3);
//   INTG(IRQ_OFFSET + 4, (u32)irq4);
//   INTG(IRQ_OFFSET + 5, (u32)irq5);
//   INTG(IRQ_OFFSET + 6, (u32)irq6);
//   INTG(IRQ_OFFSET + 7, (u32)irq7);
//   INTG(IRQ_OFFSET + 8, (u32)irq8);
//   INTG(IRQ_OFFSET + 9, (u32)irq9);
//   INTG(IRQ_OFFSET + 10, (u32)irq10);
//   INTG(IRQ_OFFSET + 11, (u32)irq11);
//   INTG(IRQ_OFFSET + 12, (u32)irq12);
//   INTG(IRQ_OFFSET + 13, (u32)irq13);
//   INTG(IRQ_OFFSET + 14, (u32)irq14);
//   INTG(IRQ_OFFSET + 15, (u32)irq15);

//   __asm__ __volatile__("lidt %0" : : "m" (idtr32));

// }

