; Entry point of the kernel.

[bits 32]

extern main

section .entry align=16
global _start
_start:
  call main
  ; Should never happen.
  jmp $

