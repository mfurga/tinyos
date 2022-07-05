; Bootloader stage 2.
;
; Stage 2 starts at 07e00h with CS = DS = ES = 7e00h, SS = 7000h.

[bits 16]
org 0000h

  mov ax, 0b800h
  mov fs, ax
  mov word [fs:0], 0x4141

  jmp $

%if ($ - $$) % 512 != 0
  times 512 - (($ - $$) % 512) db 0         ; Padding
%endif

