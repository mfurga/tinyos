/* User mode program. */

void user_main(void) {
  const char *s = "Hello world!\n";

  __asm__ __volatile__(
    "mov eax, 0;"
    "int 0x80;"
    : : "b"(s)
  );

  for (;;);
}

