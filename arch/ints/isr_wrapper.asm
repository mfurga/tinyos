;
; Interrupt Service Routines for protected mode.
;

extern int_handler

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

section .text

isr_common:
  pushad
  call int_handler
  popad
  add esp, 8
  iret

; [Fault] Divide-by-zero
isr0:
  push dword 0  ; Error code.
  push dword 0  ; Interrupt numer.
  jmp isr_common

; [Fault/Trap] Debug 
isr1:
  push dword 0
  push dword 1
  jmp isr_common

; [Interrupt] Non-maskable Interrupt
isr2:
  push dword 0
  push dword 2
  jmp isr_common

; [Trap] Breakpoint
isr3:
  push dword 0
  push dword 3
  jmp isr_common

; [Trap] Overflow
isr4:
  push dword 0
  push dword 4
  jmp isr_common

; [Fault] Bound Range Exceeded
isr5:
  push dword 0
  push dword 5
  jmp isr_common

; [Fault] Invalid Opcode
isr6:
  push dword 0
  push dword 6
  jmp isr_common

; [Fault] Device Not Available
isr7:
  push dword 0
  push dword 7
  jmp isr_common

; [Abort] Double Fault (ERROR CODE!)
isr8:
  ; Error code.
  push dword 8
  jmp isr_common

; [Fault] Coprocessor Segment Overrun
isr9:
  push dword 0
  push dword 9
  jmp isr_common

; [Fault] Invalid TSS (ERROR CODE!)
isr10:
  ; Error code.
  push dword 10
  jmp isr_common

; [Fault] Segment Not Present (ERROR CODE!)
isr11:
  ; Error code.
  push dword 11
  jmp isr_common

; [Fault] Stack-Segment Fault (ERROR CODE!)
isr12:
  ; Error code.
  push dword 12
  jmp isr_common

; [Fault] General Protection Fault (ERROR CODE!)
isr13:
  ; Error code.
  push dword 13
  jmp isr_common

; [Fault] Page Fault (ERROR CODE!)
isr14:
  ; Error code.
  push dword 14
  jmp isr_common

; Reserved
isr15:
  push dword 0
  push dword 15
  jmp isr_common

; [Fault] x87 Floating-Point Exception
isr16:
  push dword 0
  push dword 16
  jmp isr_common

; [Fault] x87 Alignment Check (ERROR CODE!)
isr17:
  ; Error code.
  push dword 17
  jmp isr_common

; [Abort] Machine Check
isr18:
  push dword 0
  push dword 18
  jmp isr_common

; [Fault] x87 SIMD Floating-Point Exception
isr19:
  push dword 0
  push dword 16
  jmp isr_common

; [Fault] Virtualization Exception
isr20:
  push dword 0
  push dword 16
  jmp isr_common

; [Fault] Control Protection Exception (ERROR CODE!)
isr21:
  ; Error code.
  push dword 21
  jmp isr_common

; Reserved
isr22:
  push dword 0
  push dword 22
  jmp isr_common

; Reserved
isr23:
  push dword 0
  push dword 23
  jmp isr_common

; Reserved
isr24:
  push dword 0
  push dword 24
  jmp isr_common

; Reserved
isr25:
  push dword 0
  push dword 25
  jmp isr_common

; Reserved
isr26:
  push dword 0
  push dword 26
  jmp isr_common

; Reserved
isr27:
  push dword 0
  push dword 27
  jmp isr_common

; [Fault] Hypervisor Injection Exception
isr28:
  push dword 0
  push dword 28
  jmp isr_common

; [Fault] VMM Communication Exception (ERROR CODE!)
isr29:
  ; Error code.
  push dword 29
  jmp isr_common

; [Fault] Security Exception (ERROR CODE!)
isr30:
  ; Error code.
  push dword 30
  jmp isr_common

; Reserved
isr31:
  push dword 0
  push dword 31
  jmp isr_common

