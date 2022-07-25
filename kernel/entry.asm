; Entry point of the kernel.

[bits 32]

extern main
extern __bss_start
extern __bss_end

section .entry align=16
global _start
_start:

  ; Clear BSS section.
  xor ax, ax
  mov ecx, __bss_end
  sub ecx, __bss_start
  mov edi, __bss_start
  rep stosb

  call main
  ; Should never happen.
  jmp $

