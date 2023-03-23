#pragma once

#include <kernel/common.h>

#define panic(...) _panic(__FILE__, __LINE__, __VA_ARGS__)

void NORETURN _panic(const char *file, int line, const char *fmt, ...);


