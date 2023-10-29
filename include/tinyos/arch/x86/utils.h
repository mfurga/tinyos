#pragma once

#if !defined(__i386__) && !defined(__x86_64__)
  #error The header can be used only for x86 architectures.
#endif

#define COM1  0x3f8
#define COM2  0x2f8
#define COM3  0x3e8
#define COM4  0x2e8

#define X86_EXP_DE         0
#define X86_EXP_DB         1
#define X86_EXP_NMI        2
#define X86_EXP_BP         3
#define X86_EXP_OF         4
#define X86_EXP_BR         5
#define X86_EXP_UD         6
#define X86_EXP_NM         7
#define X86_EXP_DF         8
#define X86_EXP_CSO        9
#define X86_EXP_TS        10
#define X86_EXP_NP        11
#define X86_EXP_SS        12
#define X86_EXP_GP        13
#define X86_EXP_PF        14
#define X86_EXP_15        15  /* reserved */
#define X86_EXP_MF        16
#define X86_EXP_AC        17
#define X86_EXP_MC        18
#define X86_EXP_XM        19
#define X86_EXP_VR        20
#define X86_EXP_CP        21
#define X86_EXP_22        22  /* reserved */
#define X86_EXP_23        23  /* reserved */
#define X86_EXP_24        24  /* reserved */
#define X86_EXP_25        25  /* reserved */
#define X86_EXP_26        26  /* reserved */
#define X86_EXP_27        27  /* reserved */
#define X86_EXP_28        28  /* reserved */
#define X86_EXP_29        29  /* reserved */
#define X86_EXP_30        30  /* reserved */
#define X86_EXP_31        31  /* reserved */

#define X86_IRQ_PC_TIMER   0
#define X86_IRQ_KB         1

#define X86_FLAGS_CF   0x0001
#define X86_FLAGS_PF   0x0004
#define X86_FLAGS_AF   0x0010
#define X86_FLAGS_ZF   0x0040
#define X86_FLAGS_SF   0x0080
#define X86_FLAGS_TF   0x0100
#define X86_FLAGS_IF   0x0200
#define X86_FLAGS_DF   0x0400
#define X86_FLAGS_OV   0x0800
#define X86_FLAGS_IOPL 0x3000
#define X86_FLAGS_NT   0x4000
#define X86_FLAGS_MD   0x8000

#define CR0_PE  (1 << 0)
#define CR0_MP  (1 << 1)
#define CR0_EM  (1 << 2)
#define CR0_TS  (1 << 3)
#define CR0_ET  (1 << 4)
#define CR0_NE  (1 << 5)
#define CR0_WP  (1 << 16)
#define CR0_AM  (1 << 18)
#define CR0_NW  (1 << 29)
#define CR0_CD  (1 << 30)
#define CR0_PG  (1 << 31)

#ifndef ASM_FILE

#include <tinyos/common/common.h>

static inline void int3(void) {
  asm_volatile("int3");
}

static inline void enable_interrupts(void) {
  asm_volatile("sti");
}

static inline void disable_interrupts(void) {
  asm_volatile("cli");
}

static inline void invalidate_cache_with_writeback(void) {
  asm_volatile("wbinvd");
}

static inline u32 read_cr0(void) {
  u32 value;
  asm_volatile("mov %0, cr0"
               : "=q" (value)
               : /* no input */
               : /* no clobber */);
  return value;
}

static inline void write_cr0(u32 value) {
  asm_volatile("mov cr0, %0"
               : /* no output */
               : "q" (value)
               : /* no clobber */);
}

static inline u32 read_cr2(void) {
  u32 value;
  asm_volatile("mov %0, cr2"
               : "=q" (value)
               : /* no input */
               : /* no clobber */);
  return value;
}

static inline void write_cr2(u32 value) {
  asm_volatile("mov cr2, %0"
               : /* no output */
               : "q" (value)
               : /* no clobber */);
}

static inline u32 read_cr4(void) {
  u32 value;
  asm_volatile("mov %0, cr4"
               : "=q" (value)
               : /* no input */
               : /* no clobber */);
  return value;
}

static inline void write_cr4(u32 value) {
  asm_volatile("mov cr4, %0"
               : /* no output */
               : "q" (value)
               : /* no clobber */);
}

static inline void cpuid(u32 code, u32 *a, u32 *b, u32 *c, u32 *d) {
  asm_volatile("cpuid"
               : "=a" (*a), "=b" (*b), "=c" (*c), "=d" (*d)
               : "a" (code)
               : /* no clobber */);
}

static inline void wrmsr(u32 id, u64 value) {
  asm_volatile("wrmsr"
               : /* no output */
               : "c" (id), "A" (value)
               : /* no clobber */);
}

static inline u64 rdmsr(u32 id) {
  u64 value;
  asm_volatile("rdmsr"
             : "=A" (value)
             : "c" (id)
             : /* no clobber */);
  return value;
}

#endif
