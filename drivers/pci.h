#pragma once

#include <lib/common.h>

#define PCI_CONFIG_ADDR_PORT      0xcf8
#define PCI_CONFIG_DATA_PORT      0xcfc

/* PCI device classes */
#define PCI_DEV_UNCLASSIFIED      0
#define PCI_DEV_MSC               1  /* Mass Storage Controller */
#define PCI_DEV_NET               2  /* Network Controller */
#define PCI_DEV_DISPLAY           3  /* Display Controller */
#define PCI_DEV_MULTIMEDIA        4  /* Multimedia Controller */
#define PCI_DEV_MEMORY            5  /* Memory Controller */
#define PCI_DEV_BRIDGE            6  /* Bridge */

/* PCI device subclasses */
#define PCI_DEV_BRIDGE_PCI2PCI    4  /* PCI-to-PCI Bridge */

#define PCI_HDRTYPE_MULTIFUNC(dev) \
  ((pci_get_header_type(dev) & 0x80) != 0)

typedef struct pci_dev {
  u8 bus;
  u8 dev;
  u8 func;

  u16 venid;
  u16 devid;

  u8 class;
  u8 subclass;
} pci_dev_t;

void pci_init(void);

