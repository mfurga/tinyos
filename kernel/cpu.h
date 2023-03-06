#pragma once

#include <kernel/common.h>

u32 cpuid_check(void);

u32 cpuid(u32 leaf, u32 *eax, u32 *ebx, u32 *ecx, u32 *edx);

