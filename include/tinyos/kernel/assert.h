#pragma once

#include <tinyos/kernel/panic.h>

#define assert(x) \
	do { if (!(x)) panic("assert failed: %s", #x); } while (0)

