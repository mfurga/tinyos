#define __ASSEMBLY__

#include <kernel/memory/gdt.h>

.intel_syntax noprefix

.extern isr_handler
.extern irq_handler

.macro ISR_WITHOUT_ERROR_CODE int_no
  .globl isr\int_no
  isr\int_no:
    cli
    pushd 0         // Error code.
    pushd \int_no   // Interrupt number.
    jmp isr_common_stub
.endm

.macro ISR_WITH_ERROR_CODE int_no
  .globl isr\int_no
  isr\int_no:
    cli
    pushd \int_no   // Interrupt number.
    jmp isr_common_stub
.endm

.macro IRQ irq_no
  .globl irq\irq_no
  irq\irq_no:
    cli
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
  sti
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
  sti
  iret

// [Fault] Divide-by-zero
ISR_WITHOUT_ERROR_CODE 0

// [Fault/Trap] Debug
ISR_WITHOUT_ERROR_CODE 1

// [Interrupt] Non-maskable Interrupt
ISR_WITHOUT_ERROR_CODE 2

// [Trap] Breakpoint
ISR_WITHOUT_ERROR_CODE 3

// [Trap] Overflow
ISR_WITHOUT_ERROR_CODE 4

// [Fault] Bound Range Exceeded
ISR_WITHOUT_ERROR_CODE 5

// [Fault] Invalid Opcode
ISR_WITHOUT_ERROR_CODE 6

// [Fault] Device Not Available
ISR_WITHOUT_ERROR_CODE 7

// [Abort] Double Fault (ERROR CODE!)
ISR_WITH_ERROR_CODE 8

// [Fault] Coprocessor Segment Overrun
ISR_WITHOUT_ERROR_CODE 9

// [Fault] Invalid TSS (ERROR CODE!)
ISR_WITH_ERROR_CODE 10

// [Fault] Segment Not Present (ERROR CODE!)
ISR_WITH_ERROR_CODE 11

// [Fault] Stack-Segment Fault (ERROR CODE!)
ISR_WITH_ERROR_CODE 12

// [Fault] General Protection Fault (ERROR CODE!)
ISR_WITH_ERROR_CODE 13

// [Fault] Page Fault (ERROR CODE!)
ISR_WITH_ERROR_CODE 14

// Reserved
ISR_WITHOUT_ERROR_CODE 15

// [Fault] x87 Floating-Point Exception
ISR_WITHOUT_ERROR_CODE 16

// [Fault] x87 Alignment Check (ERROR CODE!)
ISR_WITH_ERROR_CODE 17

// [Abort] Machine Check
ISR_WITHOUT_ERROR_CODE 18

// [Fault] x87 SIMD Floating-Point Exception
ISR_WITHOUT_ERROR_CODE 19

// [Fault] Virtualization Exception
ISR_WITHOUT_ERROR_CODE 20

// [Fault] Control Protection Exception (ERROR CODE!)
ISR_WITH_ERROR_CODE 21

// Reserved
ISR_WITHOUT_ERROR_CODE 22

// Reserved
ISR_WITHOUT_ERROR_CODE 23

// Reserved
ISR_WITHOUT_ERROR_CODE 24

// Reserved
ISR_WITHOUT_ERROR_CODE 25

// Reserved
ISR_WITHOUT_ERROR_CODE 26

// Reserved
ISR_WITHOUT_ERROR_CODE 27

// [Fault] Hypervisor Injection Exception
ISR_WITHOUT_ERROR_CODE 28

// [Fault] VMM Communication Exception (ERROR CODE!)
ISR_WITH_ERROR_CODE 29

// [Fault] Security Exception (ERROR CODE!)
ISR_WITH_ERROR_CODE 30

// Reserved
ISR_WITHOUT_ERROR_CODE 31

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
