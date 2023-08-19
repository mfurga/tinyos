#include <tinyos/common/common.h>
#include <tinyos/kernel/hal.h>

#include "idt.h"

static idt_entry_32_t idt[256] ALIGNED(16);

void load_idt(void) {
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


/*
  Intel vol.3 6.12.1.2:

  The processor checks the DPL of the interrupt or trap gate only if an
  exception or interrupt is generated with an INT n, INT3 or INTO instruction.
  Here, the CPL must be less than or equal to the DPL of the gate. This
  restriction prevents application programs or procedures running at privilege
  level 3 from using a software interrupt to access critical exception
  handlers, such as the page-fault handler, providing that those handlers are
  placed in more privileged code segments (numerically lower privilege level).
  For hardware-generated interrupts and processor-detected exceptions,
  the processor ignores the DPL of interrupt and trap gates.
*/
