#pragma once

#include <tinyos/common/common.h>

#define assert(x) \
  do { if (!(x)) panic("assert failed: %s", #x); } while (0)

#define panic(...) _panic(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)

void NORETURN _panic(const char *func,
                     const char *file,
                     int line,
                     const char *fmt,
                     ...);
