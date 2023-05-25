#pragma once

#include <drivers/pci.h>
#include <lib/common.h>

typedef struct e1000 {
  
} e1000_t;

int e1000_attach(struct pci_dev *dev);

int e1000_transmit(const u8 *data, u32 size);

int e1000_receive(const u8 *buf, u32 size);

