; Bootloader stage 2.
;
; Stage 2 starts at 07e00h with CS = DS = ES = 7e0h, SS = 7000h.

BASE_ADDR equ 0x07e00

GDT_CODE_SEG equ (gdt_code_seg - gdt_begin)
GDT_DATA_SEG equ (gdt_data_seg - gdt_begin)

[bits 16]
org 0x0000
  nop

  ; Switch to protected mode
  cli                                       ; Ignore maskable external interrupts
  lgdt [gdtr]                               ; Load GDT into GDTR register
  mov eax, cr0                              ; Set protected mode enable
  or eax, 1
  mov cr0, eax
  jmp dword GDT_CODE_SEG:(BASE_ADDR + start_protected_mode)

start_protected_mode:
[bits 32]
  mov ax, GDT_DATA_SEG
  mov ss, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov esp, 0x7ffff

  push eax
  jmp $

gdtr:
dw gdt_end - gdt_begin - 1                  ; Size of GDT in bytes - 1
dd (BASE_ADDR + gdt_begin)                  ; Linear address of GDT

align 8                                     ; 8-bytes alignment

; Declare code and data segment descriptor in GDT as flat mode
gdt_begin:
dq 0                                        ; Null segment

gdt_code_seg:
dw 0xffff                                   ; Segment Limit 15:00
dw 0                                        ; Base Address 15:00
dw 0 | (10 << 8) | (1 << 12) | (1 << 15)    ; Base 23:16, Type, S, DPL, P
dw 0xf | (1 << 6) | (1 << 7)                ; Seg Limit 19:16, AVL, L, D/B, G,

gdt_data_seg:
dw 0xffff                                   ; Segment Limit 15:00
dw 0                                        ; Base Address 15:00
dw 0 | (2 << 8) | (1 << 12) | (1 << 15)     ; Base 23:16, Type, S, DPL, P
dw 0xf | (1 << 6) | (1 << 7)                ; Seg Limit 19:16, AVL, L, D/B, G,
                                            ; Base 31:24
gdt_end:

align 512

