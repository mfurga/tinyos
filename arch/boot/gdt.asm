section .data

; === GLOBAL DESCRIPTOR TABLE ===
;
; Selector      Type
; (GDT, RPL=0)
; 0x00          NULL segment
; 0x08          32-bit code segment
; 0x10          32-bit data segment
; 0x18          16-bit code segment
; 0x20          16-bit data segment

global gdtr
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
dw 0xffff                                   ; Segment Limit 15:00
dw 0                                        ; Base Address 15:00
dw 0 | (10 << 8) | (1 << 12) | (1 << 15)    ; Base 23:16, Type, S, DPL, P
dw 0xf | (1 << 6) | (1 << 7)                ; Seg Limit 19:16, AVL, L, D/B, G,

; 32-bit Data Segment Descriptor:
; Base: 0x00000000
; Size: 4GB
; Access: Present, Ring 0, Read/Write
; Flags: 32-bit, 4kB Granularity
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
dw 0xffff                                   ; Segment Limit 15:00
dw 0                                        ; Base Address 15:00
dw 0 | (11 << 8) | (1 << 12) | (1 << 15)    ; Base 23:16, Type, S, DPL, P
dw 0xf | (0 << 6) | (0 << 7)                ; Seg Limit 19:16, AVL, L, D/B, G,

; 16-bit Data Segment Descriptor:
; Base: 0x00000000
; Size: 1Mb
; Access: Present, Ring 0, Read/Write, accessed
; Flags: 16-bit, 1B Granularity
dw 0xffff                                   ; Segment Limit 15:00
dw 0                                        ; Base Address 15:00
dw 0 | (3 << 8) | (1 << 12) | (1 << 15)     ; Base 23:16, Type, S, DPL, P
dw 0xf | (0 << 6) | (0 << 7)                ; Seg Limit 19:16, AVL, L, D/B, G,

gdt_end:

