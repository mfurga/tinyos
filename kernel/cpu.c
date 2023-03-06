#include <kernel/cpu.h>

#define CPUID(leaf, a, b, c, d)               \
  __asm__ __volatile__(                       \
    "cpuid"                                   \
    : "=a"(a), "=b"(b), "=c"(c), "=d"(d)      \
    : "0"(leaf)                               \
  )

static inline u32 cpuid_max(void) {
  u32 eax, unused;
  CPUID(0, eax, unused, unused, unused);
  return eax;
}

/*
 * Returns 1 if CPU supports the CPUID instruction, 0 otherwise.
 */
u32 cpuid_check(void) {
  u32 eax;

  __asm__ __volatile__(
    "pushfd;"                           /* Save eflags */
    "pushfd;"
    "xor dword ptr [esp], 0x00200000;"  /* Invert ID flag */
    "popfd;"                            /* Change eflags */
    "pushfd;"
    "pop eax;"                          /* eax = eflags */
    "xor eax, [esp];"                   /* Check which bits were changed */
    "and eax, 0x00200000;"              /* eax = 1 if ID can be changed */
    "popfd;"                            /* Restore eflags */
    : "=a"(eax)
  );

  return eax >> 21;
}

u32 cpuid(u32 leaf, u32 *eax, u32 *ebx, u32 *ecx, u32 *edx) {
  if (cpuid_max() < leaf) {
    return 1;
  }

  CPUID(leaf, *eax, *ebx, *ecx, *edx);
  return 0;
}

