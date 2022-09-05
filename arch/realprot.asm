%include "boot/gdt.inc"

extern gdtr
extern idtr16
extern idtr32

section .text
global real_to_prot
real_to_prot:
  [bits 16]
  cli
  lgdt [gdtr]

  ; Turn on protected mode.
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  ; Jump far, load CS, flush prefetch queue
  jmp GDT_CODE_SEG32:.prot_start

.prot_start:
  [bits 32]
  mov ax, GDT_DATA_SEG32
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ; Save and load new IDT.
  sidt [idtr16]
  lidt [idtr32]

  ; TODO: sti
  ret

global prot_to_real
prot_to_real:
  [bits 32]
  cli

  ; Make sure GDT is loaded to GDTR.
  lgdt [gdtr]

  ; Save and load new IDT.
  sidt [idtr32]
  lidt [idtr16]

  mov ax, GDT_DATA_SEG16
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  jmp GDT_CODE_SEG16:.set_cs
.set_cs:
  ; Tell nasm to generate 66h (operand size), 67h (address size)
  ; instruction prefixes.
  [bits 16]
  mov eax, cr0
  and eax, ~1
  mov cr0, eax

  jmp 0:.real_start

.real_start:
  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  sti
  retd

section .data
;
; === INTERRUPT DESCRIPTOR TABLE ===
;
; Real mode IDTR
; realidtr dw 0x400                           ; Limit
;          dd 0                               ; Address

; ; Protected mode IDTR
; protidtr dw 0                               ; Limit
;          dd 0                               ; Address

