#define __ASSEMBLY__

#include <kernel/memory/gdt.h>

.intel_syntax noprefix

.extern isr_handler
.extern irq_handler

.macro ISR_WITHOUT_ERROR_CODE int_no int_name
  .globl isr_\int_name
  isr_\int_name:
    pushd 0         // Error code.
    pushd \int_no   // Interrupt number.
    jmp isr_common_stub
.endm

.macro ISR_WITH_ERROR_CODE int_no int_name
  .globl isr_\int_name
  isr_\int_name:
    pushd \int_no   // Interrupt number.
    jmp isr_common_stub
.endm

.macro IRQ irq_no
  .globl irq\irq_no
  irq\irq_no:
    pushd \irq_no       // IRQ number (0-15).
    pushd 32 + \irq_no  // Mapped interrupt number.
    jmp irq_common_stub
.endm

.section .text , "ax", @progbits
.globl entry32
.code32

isr_common_stub:
  pushad

  mov ax, ds
  push eax

  mov ax, GDT_SEL_KERNEL_DATA | GDT_SEL_RPL_0
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  /* XXX: Pass the int_context_t structure as a pointer to the isr_handler
    function. If you pass int_context_t directly, GCC will optymalize the
    isr_handler using the input structure as stack memory. */
  push esp
  call isr_handler
  add esp, 4

  pop eax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popad
  add esp, 8
  iret

irq_common_stub:
  pushad

  mov ax, ds
  push eax

  mov ax, GDT_SEL_KERNEL_DATA | GDT_SEL_RPL_0
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  push esp
  call irq_handler
  add esp, 4

  pop eax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popad
  add esp, 8
  iret

// [Fault] Divide error
ISR_WITHOUT_ERROR_CODE 0, "divide_error"

// [Fault/Trap] Debug
ISR_WITHOUT_ERROR_CODE 1, "debug"

// [Interrupt] Non-maskable Interrupt
ISR_WITHOUT_ERROR_CODE 2, "nmi"

// [Trap] Breakpoint
ISR_WITHOUT_ERROR_CODE 3, "breakpoint"

// [Trap] Overflow
ISR_WITHOUT_ERROR_CODE 4, "overflow"

// [Fault] Bound Range Exceeded
ISR_WITHOUT_ERROR_CODE 5, "bound_range_exceeded"

// [Fault] Invalid Opcode
ISR_WITHOUT_ERROR_CODE 6, "invalid_opcode"

// [Fault] Device Not Available
ISR_WITHOUT_ERROR_CODE 7, "device_not_available"

// [Abort] Double Fault (ERROR CODE!)
ISR_WITH_ERROR_CODE 8, "double_fault"

// [Fault] Coprocessor Segment Overrun
ISR_WITHOUT_ERROR_CODE 9, "coprocessor_segment_overrun"

// [Fault] Invalid TSS (ERROR CODE!)
ISR_WITH_ERROR_CODE 10, "invalid_tss"

// [Fault] Segment Not Present (ERROR CODE!)
ISR_WITH_ERROR_CODE 11, "segment_not_present"

// [Fault] Stack-Segment Fault (ERROR CODE!)
ISR_WITH_ERROR_CODE 12, "stack_fault"

// [Fault] General Protection Fault (ERROR CODE!)
ISR_WITH_ERROR_CODE 13, "general_protection"

// [Fault] Page Fault (ERROR CODE!)
ISR_WITH_ERROR_CODE 14, "page_fault"

// Reserved
ISR_WITHOUT_ERROR_CODE 15, "15"

// [Fault] x87 Floating-Point Exception
ISR_WITHOUT_ERROR_CODE 16, "x87_fpu_floating_point"

// [Fault] x87 Alignment Check (ERROR CODE!)
ISR_WITH_ERROR_CODE 17, "alignment_check"

// [Abort] Machine Check
ISR_WITHOUT_ERROR_CODE 18, "machine_check"

// [Fault] x87 SIMD Floating-Point Exception
ISR_WITHOUT_ERROR_CODE 19, "simd_floating_point"

// [Fault] Virtualization Exception
ISR_WITHOUT_ERROR_CODE 20, "virtualization"

// [Fault] Control Protection Exception (ERROR CODE!)
ISR_WITH_ERROR_CODE 21, "control_protection"

// Reserved
ISR_WITHOUT_ERROR_CODE 22, "reserved_22"

// Reserved
ISR_WITHOUT_ERROR_CODE 23, "reserved_23"

// Reserved
ISR_WITHOUT_ERROR_CODE 24, "reserved_24"

// Reserved
ISR_WITHOUT_ERROR_CODE 25, "reserved_25"

// Reserved
ISR_WITHOUT_ERROR_CODE 26, "reserved_26"

// Reserved
ISR_WITHOUT_ERROR_CODE 27, "reserved_27"

// [Fault] Hypervisor Injection Exception
ISR_WITHOUT_ERROR_CODE 28, "reserved_28"

// [Fault] VMM Communication Exception (ERROR CODE!)
ISR_WITH_ERROR_CODE 29, "reserved_29"

// [Fault] Security Exception (ERROR CODE!)
ISR_WITH_ERROR_CODE 30, "reserved_30"

// Reserved
ISR_WITHOUT_ERROR_CODE 31, "reserved_31"

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
