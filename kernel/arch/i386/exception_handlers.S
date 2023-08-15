
.intel_syntax noprefix

#define ASM_FILE
#include <tinyos/arch/x86/utils.h>
#include <tinyos/arch/i386/utils.h>

.section .text
.global exception_entry_points

.macro exception no
exception_\no:
  push 0
  push \no
  jmp exception_common_stub
.endm

.macro exception_with_err_code no
exception_\no:
  push \no
  jmp exception_common_stub
.endm

/* Exception entry points */
exception X86_EXP_DE
exception X86_EXP_DB
exception X86_EXP_NMI
exception X86_EXP_BP
exception X86_EXP_OF
exception X86_EXP_BR
exception X86_EXP_UD
exception X86_EXP_NM
exception_with_err_code X86_EXP_DF
exception X86_EXP_CSO
exception_with_err_code X86_EXP_TS
exception_with_err_code X86_EXP_NP
exception_with_err_code X86_EXP_SS
exception_with_err_code X86_EXP_GP
exception_with_err_code X86_EXP_PF
exception X86_EXP_15  /* reserved */
exception X86_EXP_MF
exception X86_EXP_AC
exception X86_EXP_MC
exception X86_EXP_XM
exception X86_EXP_VR
exception X86_EXP_CP
exception X86_EXP_22  /* reserved */
exception X86_EXP_23  /* reserved */
exception X86_EXP_24  /* reserved */
exception X86_EXP_25  /* reserved */
exception X86_EXP_26  /* reserved */
exception X86_EXP_27  /* reserved */
exception X86_EXP_28  /* reserved */
exception X86_EXP_29  /* reserved */
exception X86_EXP_30  /* reserved */
exception X86_EXP_31  /* reserved */


exception_common_stub:
  pushad

  mov ax, ds
  push eax

  mov ax, X86_KERNEL_DATA_SEL
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  /* XXX: Pass the int_context_t structure as a pointer to the exception_entry
    function. If you pass int_context_t directly, GCC will optymalize the
    exception_entry using the input structure as stack memory. */
  push esp
  call exception_entry
  add esp, 4

  pop eax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popad
  add esp, 8
  iret


.altmacro

.macro exception_addr no
  .long exception_\no
.endm

.section .data

exception_entry_points:
.set i, 0
.rept 32
  exception_addr %i
  .set i, i + 1
.endr
