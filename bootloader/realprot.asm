%include "gdt.inc"

extern gdtr

section .text
global real_to_prot
real_to_prot:
  [bits 16]
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

  ret

global prot_to_real
prot_to_real:
  [bits 32]

  ; Make sure GDT is loaded to GDTR.
  lgdt [gdtr]

  mov ax, GDT_DATA_SEG16
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  jmp GDT_CODE_SEG16:.set_cs
.set_cs:
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

  retd

