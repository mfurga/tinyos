/*
 * Linker script for the kernel code.
 */

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(entry)

SECTIONS
{
  /* 16-bit real mode */
  . = 0x8000;

  .text16 :
  {
    *boot*(.text.startup)
    *boot*(.text)
  }

  . = ALIGN(0x10);
  .data16 : { *boot*(.data) }

  __end16 = .;

  /* 32-bit protected mode */

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

  . = ASSERT(__end16 <= 0xffff, "Real mode kernel too big!");
}

