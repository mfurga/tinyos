#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include <kernel/common.h>

#define MEMORY_TYPE_AVAILABLE 0x01
#define MEMORY_TYPE_RESERVED 0x02
#define MEMORY_TYPE_ACPI_RECLAIM 0x03
#define MEMORY_TYPE_ACPI_NVS 0x04

#define MEMORY_MAP_SIZE 16

void memory_map_init(void);

#endif  // KERNEL_MEMORY_H

