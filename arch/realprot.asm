%include "arch/boot/gdt.inc"

extern gdtr
extern idtr16
extern idtr32

extern pic_remap

section .text
global real_to_prot
real_to_prot:
  [bits 16]
  ; TODO: cli
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

  ; Remap IRQs.
  push 0x28
  push 0x20
  call pic_remap
  add esp, 8

  ; TODO: sti
  ret

global prot_to_real
prot_to_real:
  [bits 32]
  ; TODO: cli

  ; Remap IRQs.
  push 0x70  ; slave_offset
  push 8     ; master_offset
  call pic_remap
  add esp, 8

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

  ; TODO: sti
  retd

