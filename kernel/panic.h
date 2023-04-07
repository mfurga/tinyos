#pragma once

#include <lib/common.h>

#define panic(...) _panic(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)

void NORETURN _panic(const char *func,
                     const char *file,
                     int line,
                     const char *fmt,
                     ...);

