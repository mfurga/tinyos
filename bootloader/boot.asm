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

  mov eax, [ELF_BASE + ELF_ENTRY]
  jmp eax                                   ; Jump to kernel entry point

%if ($ - $$) > 510
  %fatal "Bootloader exceed 512 bytes."
%endif

times 510 - ($ - $$) db 0                   ; Padding
dw 0xaa55                                   ; Boot sector signature

