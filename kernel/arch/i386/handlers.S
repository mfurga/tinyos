.intel_syntax noprefix

#define ASM_FILE
#include <tinyos/arch/x86/utils.h>
#include <tinyos/arch/i386/utils.h>

.section .text

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

.macro irq no
irq_\no:
  push \no
  push 32 + \no
  jmp irq_common_stub
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

/* IRQ entry points */
irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
irq 8
irq 9
irq 10
irq 11
irq 12
irq 13
irq 14
irq 15

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

irq_common_stub:
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
  call irq_entry
  add esp, 4

  pop eax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popad
  add esp, 8
  iret


.section .rodata
.global exception_entry_points
.global irq_entry_points

.altmacro

.macro exception_addr no
  .long exception_\no
.endm

.macro irq_addr no
  .long irq_\no
.endm

exception_entry_points:
.set i, 0
.rept 32
  exception_addr %i
  .set i, i + 1
.endr

irq_entry_points:
.set i, 0
.rept 16
  irq_addr %i
  .set i, i + 1
.endr

