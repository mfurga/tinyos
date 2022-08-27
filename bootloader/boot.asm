;
; Bootloader.
;

%ifndef KERNEL_SIZE
  %fatal "KERNEL_SIZE must be defined."
%endif

%define BASE_ADDR 0x7c00
%define STACK_ADDR BASE_ADDR

%define ELF_LOAD_ADDR 0x500

; ELF header consts.
%define ELF_ENTRY 0x18
%define ELF_PHOFF 0x1c
%define ELF_PHENTSIZE 0x2a
%define ELF_PHNUM 0x2c

; ELF PH consts.
%define PH_TYPE 0x00
%define PH_OFFSET 0x04
%define PH_VADDR 0x08
%define PH_FILESZ 0x10

[bits 16]
[org BASE_ADDR]

start:
  ; Make sure that CS:IP = 0:0x7c00.
  jmp word 0x0000:.set_cs
  ; TODO: Add BIOS parameter block.
.set_cs:
  ; Set DS = SS = 0. SP = 0x7c00.
  cli
  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov sp, STACK_ADDR
  sti

  ; Save drive number.
  mov byte [drive_number], dl

  ; TODO: REMOVE THIS!
  jmp chs_mode

  ; Check if LBA is supported.
  mov ah, 0x41
  mov bx, 0x55aa
  int 0x13

  ; Extensions not supported.
  jc chs_mode

  ; Not installed.
  cmp bx, 0xaa55
  jnz chs_mode

  ; Check extended disk access functions (AH=42h-44h,47h,48h) supported.
  test cx, 1
  jz chs_mode

lba_mode:
  mov si, lba_read
  call print_string

  mov ah, 0x42
  mov byte dl, [drive_number]
  mov si, dap_buffer
  int 0x13

  ; If LBA fail, try CHS.
  jc chs_mode

  ; Parse and load ELF into memory.
  jmp load_elf

chs_mode:
  mov si, chs_read
  call print_string

  xor ax, ax
  mov es, ax
  mov ah, 2                                 ; Read sectors into memeory
  mov al, KERNEL_SIZE                       ; Number of sectors (+1 for second stage)
  mov ch, 0                                 ; Cylinder number
  mov cl, 2                                 ; Sector number
  mov dh, 0                                 ; Head number
  mov byte dl, [drive_number]               ; Drive number
  mov bx, ELF_LOAD_ADDR                     ; es:bx destination address
  int 0x13

  jc read_error
  test ah, ah
  jz load_elf

read_error:
  mov si, disk_error
  call print_string
  jmp $ ; EB FE

load_elf:
  mov si, kernel_load
  call print_string

  ; Parse ELF.
  ; Load segments into memory.
  cld

  mov word cx, [ELF_LOAD_ADDR + ELF_PHNUM]
  mov dword ebx, [ELF_LOAD_ADDR + ELF_PHOFF]
  add ebx, ELF_LOAD_ADDR

read_program_header:
  ; PH type
  mov dword eax, [ebx + PH_TYPE]
  cmp eax, 1                                ; Loadable segment
  jne .skip_segment

  push cx

  mov dword ecx, [ebx + PH_FILESZ]
  mov dword esi, [ebx + PH_OFFSET]
  add esi, ELF_LOAD_ADDR
  mov dword edi, [ebx + PH_VADDR]
  rep movsb                                 ; Copy segment

  pop cx

.skip_segment:
  movzx word ax, [ELF_LOAD_ADDR + ELF_PHENTSIZE]
  add ebx, eax
  loop read_program_header

  mov eax, [ELF_LOAD_ADDR + ELF_ENTRY]
  jmp eax                                   ; Jump to kernel entry point

; Print string (null terminated) given in SI reg.
print_string:
  push ax
  push bx
  push si

  mov ah, 0x0e
  mov bx, 0

.print_string_loop:
  mov byte al, [si]
  int 0x10
  inc si
  cmp byte [si], 0
  jne .print_string_loop

  pop si
  pop bx
  pop ax
  ret

;
; === Data section ===
;

lba_read db "Reading sectors using LBA ...", 0x0d, 0x0a, 0
chs_read db "Reading sectors using CHS ...", 0x0d, 0x0a, 0
disk_error db "Failed to read sectors from the disk.", 0x0d, 0x0a, 0
kernel_load db "Loading kernel ...", 0x0d, 0x0a, 0

drive_number db 0

; The Disk Address Packet (DAP) buffer used in LBA mode.
; Reference: https://en.wikipedia.org/wiki/INT_13H
;               #INT_13h_AH.3D42h:_Extended_Read_Sectors_From_Drive
dap_buffer:
  db 0x10                                   ; Size of DAP (set this to 10h)
  db 0                                      ; Unused (should be zero)
  db KERNEL_SIZE                            ; Number of sectors to be read
                                            ; (max 7Fh for Phoenix EDD)
  db 0                                      ; Unused (should be zero)
  dw ELF_LOAD_ADDR                          ; Buffer offset
  dw 0                                      ; Buffer segment
  dq 0x200                                  ; LBA address

%if ($ - $$) > 510
  %fatal "Bootloader exceed 512 bytes."
%endif

times 510 - ($ - $$) db 0                   ; Padding
dw 0xaa55                                   ; Boot sector signature

