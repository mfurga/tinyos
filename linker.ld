/*
 * Linker script for the kernel code.
 */

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(entry32)

SECTIONS
{
  . = 0x100000;

  .text :
  {
    *(.entry)
    *(.text)
  }

  . = ALIGN(0x10);
  .rodata : { *(.rodata) }

  . = ALIGN(0x10);
  .data : { *(.data) }

  . = ALIGN(0x10);
  .bss :
  {
    __bss_start = .;
    *(.bss)
    __bss_end = .;
  }

  /DISCARD/ :
  {
    *(.note*)
    *(.comment)
  }
}

