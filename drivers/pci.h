#pragma once

#include <kernel/common.h>

#define PCI_CONFIG_ADDR_PORT 0xcf8
#define PCI_CONFIG_DATA_PORT 0xcfc

void pci_probe(void);

