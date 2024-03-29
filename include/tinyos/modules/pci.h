#pragma once

#include <tinyos/common/common.h>

#define PCI_CONFIG_ADDR_PORT      0xcf8
#define PCI_CONFIG_DATA_PORT      0xcfc

#define PCI_BAR_TYPE(bar)         ((bar) & PCI_BAR_TYPE_MASK)
#define PCI_BAR_TYPE_MASK         0x00000001
#define PCI_BAR_TYPE_MEM          0x00000000
#define PCI_BAR_TYPE_IO           0x00000001

#define PCI_BAR_MEM_TYPE(bar)     ((bar) & PCI_BAR_MEM_TYPE_MASK)
#define PCI_BAR_MEM_TYPE_MASK     0x00000006
#define PCI_BAR_MEM_TYPE_32       0x00000000
#define PCI_BAR_MEM_TYPE_64       0x00000004

#define PCI_BAR_MEM_ADDR(bar)     ((bar) & PCI_BAR_MEM_ADDR_MASK)
#define PCI_BAR_MEM_ADDR_MASK     0xfffffff0
#define PCI_BAR_MEM_SIZE(bar)     (~(bar) + 1)

#define PCI_BAR_IO_ADDR(bar)      ((bar) & PCI_BAR_IO_ADDR_MASK)
#define PCI_BAR_IO_ADDR_MASK      0xfffffffc
#define PCI_BAR_IO_SIZE(bar)      (~(bar) + 1)

#define PCI_REG_POS(offset, low, high) \
  ((offset) | ((low) << 8) | ((high) << 16))

#define PCI_REG_POS_OFFSET(pos)   ((pos) & 0xff)
#define PCI_REG_POS_LOW(pos)      (((pos) >> 8) & 0xff)
#define PCI_REG_POS_HIGH(pos)     (((pos) >> 16) & 0xff)

#define PCI_REG_POS_ADD_OFFSET(pos, offset) \
  ((pos) + (offset))

/* PCI device registers */
#define PCI_REG_VENID             PCI_REG_POS(0x0, 0, 15)
#define PCI_REG_DEVID             PCI_REG_POS(0x0, 16, 31)
#define PCI_REG_COMMAND           PCI_REG_POS(0x4, 0, 15)
#define PCI_REG_STATUS            PCI_REG_POS(0x4, 16, 31)
#define PCI_REG_REVID             PCI_REG_POS(0x8, 0, 7)
#define PCI_REG_PROGIF            PCI_REG_POS(0x8, 8, 15)
#define PCI_REG_SUBCLASS          PCI_REG_POS(0x8, 15, 23)
#define PCI_REG_CLASS             PCI_REG_POS(0x8, 24, 31)
#define PCI_REG_CACHE_SIZE        PCI_REG_POS(0xc, 0, 7)
#define PCI_REG_LAT_TIMER         PCI_REG_POS(0xc, 8, 15)
#define PCI_REG_HEADER_TYPE       PCI_REG_POS(0xc, 16, 23)
#define PCI_REG_BIST              PCI_REG_POS(0xc, 24, 31)
#define PCI_REG_BAR_START         PCI_REG_POS(0x10, 0, 31)
#define PCI_REG_BAR_END           PCI_REG_POS(0x24, 0, 31)
#define PCI_REG_INT_LINE          PCI_REG_POS(0x3c, 0, 7)
#define PCI_REG_INT_PIN           PCI_REG_POS(0x3c, 15, 8)

#define PCI_REG_BAR_NUM(pos) \
  (((pos) - PCI_REG_BAR_START) / 4)

#define PCI_REG_SEC_BUS           PCI_REG_POS(0x18, 8, 15)


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

/* PCI command reg */
#define PCI_COMMAND_IO_ENABLE       0b00000001
#define PCI_COMMAND_MEM_ENABLE      0b00000010
#define PCI_COMMAND_MASTER_ENABLE   0b00000100
#define PCI_COMMAND_SPECIAL_ENABLE  0b00001000

typedef struct pci_dev {
  u8 bus;
  u8 dev;
  u8 func;

  u16 venid;        /* Vendor ID */
  u16 devid;        /* Device ID */
  u8 class;         /* Class register */
  u8 subclass;      /* Subclass register */

  u32 bar_addr[6];  /* BARs addr */
  u32 bar_size[6];  /* BARs size */

} pci_dev_t;

void pci_init(void);

void pci_dev_enable(struct pci_dev *dev);

u32 pci_conf_read(const struct pci_dev *dev, u32 pos);

void pci_conf_write(const struct pci_dev *dev, u32 pos, u32 value);

