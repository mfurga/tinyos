#pragma once

#include <kernel/panic.h>

#define assert(x) \
	do { if (!(x)) panic("assert failed: %s", #x); } while (0)

