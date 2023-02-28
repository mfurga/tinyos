#ifndef STDIO_H
#define STDIO_H

#define DEBUG 1

#ifdef DEBUG
#  define SAYF(x...) printf(x)
#else
#  define SAYF(x...)
#endif

#define INFO(x...) do { \
    SAYF("[*] " x); \
    SAYF("\n"); \
  } while (0)

#define OK(x...) do { \
    SAYF("[+] " x); \
    SAYF("\n"); \
  } while (0)

#define WARN(x...) do { \
    SAYF("[!] WARNING: " x); \
    SAYF("\n"); \
  } while (0)

#define FATAL(x...) do { \
    SAYF("[-] FATAL: " x); \
    SAYF("\n"); \
  } while (0)

void putchar(char c);
void puts(const char *s);
void printf(const char *fmt, ...);

#endif  // STDIO_H

