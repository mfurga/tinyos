#pragma once

#include <drivers/pci.h>
#include <lib/common.h>

typedef struct e1000_rx_desc {
  u64 addr;
  u16 length;
  u16 checksum;
  u8 status;
  u8 errors;
  u16 special;
} e1000_rx_desc_t;

typedef struct e1000_rx_buf {
  u8 data[2048];
} e1000_rx_buf_t;

int e1000_attach(struct pci_dev *dev);

int e1000_transmit(const u8 *data, unsigned int size);

int e1000_receive(u8 *buf, unsigned int size);

