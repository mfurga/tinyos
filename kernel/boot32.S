/*
 * Entry point of 32-bit kernel.
 */

.intel_syntax noprefix

.section .text , "ax", @progbits
.global entry32
.code32

entry32:
  cli
  jmp $

