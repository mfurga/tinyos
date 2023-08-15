#pragma once

#define GDT 0
#define LDT 1

#define X86_SEL(idx, ti, rpl) ((idx) << 3 | (ti) << 2 | (rpl))

#define X86_KERNEL_CODE_SEL X86_SEL(1, GDT, 0)
#define X86_KERNEL_DATA_SEL X86_SEL(2, GDT, 0)
#define X86_USER_CODE_SEL X86_SEL(3, GDT, 3)
#define X86_USER_DATA_SEL X86_SEL(4, GDT, 3)

#ifndef ASM_FILE

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
