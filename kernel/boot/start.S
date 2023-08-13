/*
 * Entry point of 32-bit kernel.
 *
 * Kernel supports Multiboot 1.0 standard:
 * https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
 */

.intel_syntax noprefix

#define ASM_FILE 1
#include <kernel/boot/multiboot.h>

.code32

.section .text.start
.global _start
_start:
  jmp multiboot_entry

#define MUTIBOOT_FLAGS    (MULTIBOOT_PAGE_ALIGN |  \
                           MULTIBOOT_MEMORY_INFO | \
                           MULTIBOOT_VIDEO_MODE)

/* Multiboot 1.0 header */
.align MULTIBOOT_HEADER_ALIGN

  .long MULTIBOOT_HEADER_MAGIC                      /* magic */
  .long MUTIBOOT_FLAGS                              /* flags */
  .long -(MULTIBOOT_HEADER_MAGIC + MUTIBOOT_FLAGS)  /* checksum */
  .long 0                                           /* header_addr */
  .long 0                                           /* load_addr */
  .long 0                                           /* load_end_addr */
  .long 0                                           /* bss_end_addr */
  .long 0                                           /* entry_addr */
  .long 0                                           /* mode_type */
  .long 0                                           /* width */
  .long 0                                           /* height */
  .long 32                                          /* color depth */

multiboot_entry:
  cli
  cld

  /* Push Multiboot information structure. */
  push ebx

  /* Push magic value. */
  push eax

  /* Clear BSS section. */
  xor eax, eax
  mov ecx, offset __BSS_END_PADDR
  sub ecx, offset __BSS_START_PADDR
  mov edi, offset __BSS_START_PADDR
  rep stosb

  /* Call C kernel_main function. */
  call kernel_main

  /* Never go here! */
  jmp $

