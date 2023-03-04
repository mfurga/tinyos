/*
 * Entry point of 32-bit kernel.
 *
 * BOOT PROTOCOL:
 * The CPU must be in 32-bit protected mode with paging disabled.
 * ESI must hold the base address of the boot_params_t structure.
 * A20 has to be enabled.
 */

.intel_syntax noprefix

.section .text , "ax", @progbits
.global entry32
.code32

entry32:
  cli

  // Clear BSS section.
  xor eax, eax
  mov ecx, __bss_end
  sub ecx, __bss_start
  mov edi, __bss_start
  rep stosb

  // Pass the base address of boot_params_t structure.
  push esi

  // Call C kernel_main function.
  call kernel_main

  // Never go here!
  jmp $

