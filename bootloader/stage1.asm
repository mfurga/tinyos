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
org 0x7c00                                  ; Bootloader's starting address

jmp word 0x0000:start
start:
  mov ax, 0x7000                            ; Set stack to end of usable memory
  mov ss, ax
  mov sp, 0xffff

  xor ax, ax
  mov es, ax
  mov ah, 2                                 ; Read sectors into memeory
  mov al, STAGE2_SIZE                       ; Number of sectors
  mov ch, 0                                 ; Cylinder number
  mov cl, 2                                 ; Sector number
  mov dh, 0                                 ; Head number
  ;mov dl, 0                                ; Drive number (already set by BIOS)
  mov bx, 0x7e00                            ; es:bx destination address
  int 0x13

  mov ax, 0x7e0                             ; Set segment registers at start of stage 2
  mov ds, ax
  mov es, ax
  jmp word 0x7e0:0                          ; Jump to second stage

%if ($ - $$) > 510
  %fatal "Bootloader exceed 512 bytes."
%endif
times 510 - ($ - $$) db 0                   ; Padding
dw 0xaa55                                   ; Boot sector signature

