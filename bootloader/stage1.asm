; Bootloader stage 1.
;
; All x86 processors start in real mode due to backward compatibility.
;
; +------------------------+ 00000h
; |        IVT (1 KB)      |
; +------------------------+ 003FFh / 00400h
; | BIOS data area (256 B) |
; +------------------------+ 004FFh / 00500h
; |                        |
; | Usable memory (30 KiB) |
; |                        |
; +------------------------+ 07BFFh / 07C00h
; |   Boot sector (512 B)  |
; |  (Bootloader stage 1)  |
; +------------------------+ 07DFFh / 07E00h
; |  (Bootloader stage 2)  |
; |                        |
; |     Usable memory      |
; |       (480.5 KiB)      |
; |                        |
; |         (stack)        |
; +------------------------+ 7FFFFh / 80000h
; |                        |
; | Extended BIOS Data Area|
; |        (128 KiB)       |
; |                        |
; +------------------------+ 9FFFFh / A0000h
; |                        |
; |  Video display memory  |
; |        (128 Kib)       |
; |                        |
; +------------------------+ BFFFFh / C0000h
; |                        |
; |          BIOS          |
; |                        |
; +------------------------+ FFFFFh

%ifndef STAGE2_SIZE
  %fatal "STAGE2_SIZE must be defined."
%endif

[bits 16]
org 7c00h                                   ; Bootloader's starting address

jmp word 0000h:start
start:
  mov ax, 7000h                             ; Set stack to end of usable memory
  mov ss, ax
  mov sp, 0ffffh

  xor ax, ax
  mov es, ax
  mov ah, 2                                 ; Read sectors into memeory
  mov al, STAGE2_SIZE                       ; Number of sectors
  mov ch, 0                                 ; Cylinder number
  mov cl, 2                                 ; Sector number
  mov dh, 0                                 ; Head number
  ;mov dl, 0                                ; Drive number (already set by BIOS)
  mov bx, 7e00h                             ; es:bx destination address
  int 13h

  jnc jump_to_stage2
  mov si, DISK_READ_ERROR
  call print_str
  jmp $

jump_to_stage2:
  mov ax, 7e0h
  mov ds, ax
  mov es, ax
  jmp word 7e0h:0h                          ; Jump to second stage

print_str:
  pushfd
  push ax
  push bx
  push si

  mov ah, 0eh
  mov bx, 000fh
print_str_loop:
  mov al, [si]
  int 10h
  inc si
  cmp byte [si], 0
  jnz print_str_loop

  pop si
  pop bx
  pop ax
  popfd
  ret

DISK_READ_ERROR db "Failed to read disk sectors.", 0

%if ($ - $$) > 510
  %fatal "Bootloader exceed 512 bytes."
%endif
times 510 - ($ - $$) db 0                   ; Padding
dw 0aa55h                                   ; Boot sector signature

