;
; Bootloader stage 1.
;

%ifndef STAGE2_SIZE
  %fatal "STAGE2_SIZE must be defined."
%endif

%define BASE_ADDR 0x7c00
%define STACK_ADDR BASE_ADDR

%define STAGE2_LOAD_ADDR 0x500

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

;%if (BASE_ADDR - (STAGE2_SIZE * 512 + STAGE2_LOAD_ADDR) <= 0)
;  %fatal "Kernel too large!"
;%endif

start:
  ; Make sure that CS:IP = 0:0x7c00.
  jmp word 0x0000:.set_cs
.set_cs:
  ; Set DS = SS = 0. SP = 0x7c00.
  cli
  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov sp, STACK_ADDR

  ; Save drive number.
  mov byte [drive_number], dl

  ; Print banner.
  mov si, banner
  call print_string

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

  ; Load stage2.
  jmp load_stage2

chs_mode:
  mov si, chs_read
  call print_string

  xor ax, ax
  mov es, ax
  mov ah, 2                                 ; Read sectors into memeory
  mov al, STAGE2_SIZE                       ; Number of sectors
  mov ch, 0                                 ; Cylinder number
  mov cl, 2                                 ; Sector number (+1 for stage1)
  mov dh, 0                                 ; Head number
  mov byte dl, [drive_number]               ; Drive number
  mov bx, STAGE2_LOAD_ADDR                  ; es:bx destination address
  int 0x13

  jc read_error
  test ah, ah
  jz load_stage2

read_error:
  mov si, disk_error
  call print_string
  jmp $ ; EB FE

load_stage2:
  mov si, stage2_load
  call print_string

  ; dl - Driver number
  ; dh - Bootloader size
  mov byte dl, [drive_number]
  mov dh, STAGE2_SIZE
  inc dh                                   ; +1 for stage1.
  jmp 0x500                                ; Jump to stage2.

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

banner db "*** TinyOS bootloader ***", 0xd, 0xa, 0xd, 0xa, 0
lba_read db "Reading sectors using LBA ...", 0xd, 0xa, 0
chs_read db "Reading sectors using CHS ...", 0xd, 0xa, 0
disk_error db "Failed to read sectors from the disk.", 0xd, 0xa, 0
stage2_load db "Loading stage 2 ...", 0x0d, 0x0a, 0

drive_number db 0

; The Disk Address Packet (DAP) buffer used in LBA mode.
; Reference: https://en.wikipedia.org/wiki/INT_13H
;               #INT_13h_AH.3D42h:_Extended_Read_Sectors_From_Drive
dap_buffer:
  db 0x10                                   ; Size of DAP (set this to 10h)
  db 0                                      ; Unused (should be zero)
  db STAGE2_SIZE                            ; Number of sectors to be read
                                            ; (max 7Fh for Phoenix EDD)
  db 0                                      ; Unused (should be zero)
  dw STAGE2_LOAD_ADDR                       ; Buffer offset
  dw 0                                      ; Buffer segment
  dq 0x200                                  ; LBA address

%if ($ - $$) > 510
  %fatal "Bootloader exceed 512 bytes."
%endif

times 510 - ($ - $$) db 0                   ; Padding
dw 0xaa55                                   ; Boot sector signature

