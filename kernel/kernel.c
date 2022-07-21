
void _start(void) {
  char *video_memory = (char *)0xb8000;
  video_memory[0] = 'A';
  for (;;);
}

