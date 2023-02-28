;
; Bootloader stage 2.
;

%include "gdt.inc"

section .data
extern BOOTLOADER_SIZE
extern DRIVE_NUMBER

DRIVE_NUMBER db 0
BOOTLOADER_SIZE db 0

section .entry

extern __bss_start, __bss_end
extern gdtr
extern main

global _start
_start:
  [bits 16]
  cli
  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov sp, 0x7c00

  ; Go to protected mode and jump to C main.
  lgdt [gdtr]

  ; Turn on protected mode.
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  ; Jump far, load CS, flush prefetch queue
  jmp GDT_CODE_SEG32:.protected_mode_start
.protected_mode_start:
  [bits 32]
  mov ax, GDT_DATA_SEG32
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ; Clear BSS section.
  xor eax, eax
  mov ecx, __bss_end
  sub ecx, __bss_start
  mov edi, __bss_start
  rep stosb

  ; Save params from stage 1.
  mov byte [DRIVE_NUMBER], dl
  mov byte [BOOTLOADER_SIZE], dh

  ; Jump to C main function.
  call main
  jmp $

