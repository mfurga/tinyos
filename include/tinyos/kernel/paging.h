#pragma once

#include <tinyos/common/config.h>

#define KERNEL_VA_TO_PA(vaddr) ((u32)(vaddr) - KERNEL_BASE_VADDR)

#define KERNEL_PA_TO_VA(paddr) (void *)((paddr) + KERNEL_BASE_VADDR)
