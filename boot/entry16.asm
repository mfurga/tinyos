;
; Entry point of **real mode** kernel.
;

section .text

extern __bss_start
extern __bss_end
extern main

global entry
entry:
  [bits 16]
  cli
  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov sp, 0x7c00

  ; TODO: Enable A20.

  ; Go to protected mode and jump to C main.
  cli
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

  ; Jump to C main function.
  call main
  jmp $

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
  sidt [realidtr]
  lidt [protidtr]

  ; TODO: sti
  ret

prot_to_real:
  [bits 32]
  cli

  ; Make sure GDT is loaded to GDTR.
  lgdt [gdtr]

  ; Save and load new IDT.
  sidt [protidtr]
  lidt [realidtr]

  mov ax, GDT_DATA_SEG16
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  jmp GDT_CODE_SEG16:.set_cs
.set_cs
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
realidtr dw 0x400                           ; Limit
         dd 0                               ; Address

; Protected mode IDTR
protidtr dw 0                               ; Limit
         dd 0                               ; Address

; === GLOBAL DESCRIPTOR TABLE ===
;
; Selector      Type
; (GDT, RPL=0)
; 0x00          NULL segment
; 0x08					32-bit code segment
; 0x10          32-bit data segment
; 0x18          16-bit code segment
; 0x20          16-bit data segment

GDT_CODE_SEG32 equ (gdt_code_seg32 - gdt_begin)
GDT_DATA_SEG32 equ (gdt_data_seg32 - gdt_begin)
GDT_CODE_SEG16 equ (gdt_code_seg16 - gdt_begin)
GDT_DATA_SEG16 equ (gdt_data_seg16 - gdt_begin)

gdtr:
dw gdt_end - gdt_begin - 1                  ; Size of GDT in bytes - 1
dd gdt_begin                                ; Linear address of GDT

align 16                                    ; 16-bytes alignment

gdt_begin:
; NULL Segment
dq 0

; 32-bit Code Segment Descriptor:
; Base: 0x00000000
; Size: 4GB
; Access: Present, Ring 0, Execute/Read
; Flags: 32-bit, 4kB Granularity
gdt_code_seg32:
dw 0xffff                                   ; Segment Limit 15:00
dw 0                                        ; Base Address 15:00
dw 0 | (10 << 8) | (1 << 12) | (1 << 15)    ; Base 23:16, Type, S, DPL, P
dw 0xf | (1 << 6) | (1 << 7)                ; Seg Limit 19:16, AVL, L, D/B, G,

; 32-bit Data Segment Descriptor:
; Base: 0x00000000
; Size: 4GB
; Access: Present, Ring 0, Read/Write
; Flags: 32-bit, 4kB Granularity
gdt_data_seg32:
dw 0xffff                                   ; Segment Limit 15:00
dw 0                                        ; Base Address 15:00
dw 0 | (2 << 8) | (1 << 12) | (1 << 15)     ; Base 23:16, Type, S, DPL, P
dw 0xf | (1 << 6) | (1 << 7)                ; Seg Limit 19:16, AVL, L, D/B, G,
                                            ; Base 31:24

; 16-bit Code Segment Descriptor:
; Base: 0x00000000
; Size: 1Mb
; Access: Present, Ring 0, Execute/Read, accessed
; Flags: 16-bit, 1B Granularity
gdt_code_seg16:
dw 0xffff                                   ; Segment Limit 15:00
dw 0                                        ; Base Address 15:00
dw 0 | (11 << 8) | (1 << 12) | (1 << 15)    ; Base 23:16, Type, S, DPL, P
dw 0xf | (0 << 6) | (0 << 7)                ; Seg Limit 19:16, AVL, L, D/B, G,

; 16-bit Data Segment Descriptor:
; Base: 0x00000000
; Size: 1Mb
; Access: Present, Ring 0, Read/Write, accessed
; Flags: 16-bit, 1B Granularity
gdt_data_seg16:
dw 0xffff                                   ; Segment Limit 15:00
dw 0                                        ; Base Address 15:00
dw 0 | (3 << 8) | (1 << 12) | (1 << 15)     ; Base 23:16, Type, S, DPL, P
dw 0xf | (0 << 6) | (0 << 7)                ; Seg Limit 19:16, AVL, L, D/B, G,

gdt_end:

