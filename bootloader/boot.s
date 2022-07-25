; Boot.

%ifndef KERNEL_SIZE
  %fatal "KERNEL_SIZE must be defined."
%endif

BASE_ADDR equ 0x07c00

; ELF header consts.
ELF_BASE equ 0x500
ELF_ENTRY equ 0x18
ELF_PHOFF equ 0x1c
ELF_PHENTSIZE equ 0x2a
ELF_PHNUM equ 0x2c

; ELF PH consts.
PH_TYPE equ 0x00
PH_OFFSET equ 0x04
PH_VADDR equ 0x08
PH_FILESZ equ 0x10

[bits 16]
[org BASE_ADDR]                             ; Bootloader's starting address

start:
  cli                                       ; Ignore maskable external interrupts
  jmp word 0x0000:set_cs
set_cs:
  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov sp, BASE_ADDR

  xor ax, ax
  mov es, ax
  mov ah, 2                                 ; Read sectors into memeory
  mov al, KERNEL_SIZE                       ; Number of sectors (+1 for second stage)
  mov ch, 0                                 ; Cylinder number
  mov cl, 2                                 ; Sector number
  mov dh, 0                                 ; Head number
  ;mov dl, 0                                ; Drive number (already set by BIOS)
  mov bx, ELF_BASE                          ; es:bx destination address
  int 0x13

  jnc disk_ok
  jmp $

disk_ok:
  ; Switch to protected mode
  lgdt [gdtr]                               ; Load GDT into GDTR register
  mov eax, cr0                              ; Set protected mode enable
  or eax, 1
  mov cr0, eax
  jmp dword GDT_CODE_SEG:start_protected_mode

start_protected_mode:
[bits 32]
  mov ax, GDT_DATA_SEG                      ; Set segment register to data segment selector
  mov ss, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov esp, 0x7c00                           ; Set stack to end of usable memory

  ; Parse ELF.
  ; Load segments into memory.
  cld

  movzx ecx, word [ELF_BASE + ELF_PHNUM]
  movzx edx, word [ELF_BASE + ELF_PHENTSIZE]
  mov dword ebx, [ELF_BASE + ELF_PHOFF]
  add ebx, ELF_BASE

read_program_header:
  ; PH type
  mov dword eax, [ebx + PH_TYPE]
  cmp eax, 1                                ; Loadable segment
  jne .skip_segment

  push ecx

  mov dword ecx, [ebx + PH_FILESZ]
  mov dword esi, [ebx + PH_OFFSET]
  add esi, ELF_BASE
  mov dword edi, [ebx + PH_VADDR]
  rep movsb                                 ; Copy segment

  pop ecx

.skip_segment:
  add ebx, edx
  loop read_program_header

  sti

  mov eax, [ELF_BASE + ELF_ENTRY]
  jmp eax                                   ; Jump to kernel entry point

GDT_CODE_SEG equ (gdt_code_seg - gdt_begin)
GDT_DATA_SEG equ (gdt_data_seg - gdt_begin)

gdtr:
dw gdt_end - gdt_begin - 1                  ; Size of GDT in bytes - 1
dd gdt_begin                                ; Linear address of GDT

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

%if ($ - $$) > 510
  %fatal "Bootloader exceed 512 bytes."
%endif

times 510 - ($ - $$) db 0                   ; Padding
dw 0xaa55                                   ; Boot sector signature

