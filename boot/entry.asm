;
; Entry point of **real mode** kernel.
;

[bits 16]

extern main
extern __bss16_start
extern __bss16_end

section .text

global entry16
entry16:
  jmp 0x0000:.zero_cs
.zero_cs:
  xor ax, ax
  mov ss, ax
  mov ds, ax
  mov es, ax
  mov sp, 0x7c00

  ; Clear BSS section for 16bit.
  mov cx, __bss16_end
  sub cx, __bss16_start
  mov di, __bss16_start
  rep stosb

  jmp main

