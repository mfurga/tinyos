#pragma once

#include <kernel/common.h>

#define panic(...) _panic(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

void NORETURN _panic(const char *file,
                     const char *func,
                     int line,
                     const char *fmt,
                     ...);

