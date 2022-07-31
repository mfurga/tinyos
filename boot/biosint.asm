;
; BIOS interrupt interface.
;

extern prot_to_real
extern real_to_prot

REGS_SIZE equ 8 * 4 + 4 + 4

section .text

global biosint
biosint:
  [bits 32]
  ; esp + 4 = no_int
  ; esp + 8 = reg_in
  ; esp + 12 = reg_out

  call dword prot_to_real
  [bits 16]

  mov byte al, [esp + 4]
  mov byte [.int_no], al

  push fs
  push gs
  pushfd
  pushad

  ; Check if reg_in == NULL.
  mov dword eax, [esp + 8 + REGS_SIZE]
  test eax, eax
  jz .return

  ; Save current SP in reg_in.
  mov dword [eax + 12], esp

  ; Load regs.
  mov dword esp, eax
  popad
  popfd
  pop gs
  pop fs

  ; Back esp.
  mov dword esp, [esp - 4 * 5 - 8]

  ; int instruction opcode.
  db 0xcd
.int_no:
  db 0x00

  ; Save state after interrupt.
  push fs
  push gs
  pushfd
  pushad

  mov edi, [esp + 12 + 2 * REGS_SIZE]
  test edi, edi
  jz .null_reg_out

  mov esi, esp
  mov ecx, REGS_SIZE
  rep movsb

.null_reg_out:
  add esp, REGS_SIZE
.return
  popad
  popfd
  pop gs
  pop fs

  call dword real_to_prot
  [bits 32]

  ret

