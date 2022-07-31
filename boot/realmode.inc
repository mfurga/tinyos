
; === GLOBAL DESCRIPTOR TABLE ===
;
; Selector      Type
; (GDT, RPL=0)
; 0x00          NULL segment
; 0x08					32-bit code segment
; 0x10          32-bit data segment
; 0x18          16-bit code segment
; 0x20          16-bit data segment

%define GDT_CODE_SEG32 0x08
%define GDT_DATA_SEG32 0x10
%define GDT_CODE_SEG16 0x18
%define GDT_DATA_SEG16 0x20

