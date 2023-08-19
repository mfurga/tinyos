#pragma once

#define GDT 0
#define LDT 1

#define X86_SELECTOR(idx, ti, rpl) ((idx) << 3 | (ti) << 2 | (rpl))

#define GDT_ENTRY_NULL 0
#define GDT_ENTRY_KERNEL_CODE 1
#define GDT_ENTRY_KERNEL_DATA 2
#define GDT_ENTRY_USER_CODE 3
#define GDT_ENTRY_USER_DATA 4
#define GDT_ENTRY_TSS 5

#define X86_KERNEL_CODE_SEL X86_SELECTOR(GDT_ENTRY_KERNEL_CODE, GDT, 0)
#define X86_KERNEL_DATA_SEL X86_SELECTOR(GDT_ENTRY_KERNEL_DATA, GDT, 0)
#define X86_USER_CODE_SEL X86_SELECTOR(GDT_ENTRY_USER_CODE, GDT, 3)
#define X86_USER_DATA_SEL X86_SELECTOR(GDT_ENTRY_USER_DATA, GDT, 3)
#define X86_TSS_SEL X86_SELECTOR(GDT_ENTRY_TSS, GDT, 0)

#ifndef ASM_FILE

#include <tinyos/common/common.h>

static inline u32 get_eflags(void) {
  u32 eflags;
  asm_volatile("pushfd;"
               "pop %0;"
               : "=g" (eflags)
               : /* no input */
               : /* no clobber */);
  return eflags;
}

static inline bool are_interrupts_enabled(void) {
  u32 eflags = get_eflags();
  return !!(eflags & X86_FLAGS_IF);
}

struct i386_int_context {
  u32 ds;
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by `pushad` */
  u32 int_no;
  union {
    u32 error_code;
    u32 irq_no;
  };
  u32 eip, cs, eflags, ret_esp, ret_ss;  /* Pushed by CPU */
} PACKED;

#endif
