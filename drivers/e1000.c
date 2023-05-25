#include <drivers/e1000.h>
#include <kernel/stdio.h>

static volatile u32 *mmio_base;
static u32 mmio_size;

/*
u32 detect_eeprom(const struct pci_dev *dev) {
  u32 exists = 0, val;

  write(dev, 0x0014, 0x1);

  for (int i = 0; i < 1000 & !exists; i++) {
    val = read(dev, 0x0014);
    if (val & 0x10) {
      exists = 1;
    } else {
      exists = 0;
    }
  }

  return exists;
}

u32 read_eeprom(const struct pci_dev *dev, u32 addr) {
  u32 tmp;

  write(dev, 0x0014, (addr << 8) | 1);

  do {
    tmp = read(dev, 0x0014);
  } while ((tmp & (1 << 4)) == 0);

  return (tmp >> 16);
}


void read_mac(const struct pci_dev *dev, u8 *mac) {
  u32 exists = detect_eeprom(dev);
  exists = 1;

  if (exists) {
    u32 tmp;
    tmp = read_eeprom(dev, 0);
    mac[0] = tmp & 0xff;
    mac[1] = tmp >> 8;

    tmp = read_eeprom(dev, 1);
    mac[2] = tmp & 0xff;
    mac[3] = tmp >> 8;

    tmp = read_eeprom(dev, 2);
    mac[4] = tmp & 0xff;
    mac[5] = tmp >> 8;
  } else {
    u32 bar0 = pci_conf_read(dev, PCI_REG_BAR0);
    u32 base = bar0 & 0xfffffff0;

    u8 *mem_base_mac_8 = (u8 *)(base + 0x5400);
    u32 *mem_base_mac_32 = (u32 *)(base + 0x5400);

    if (mem_base_mac_32[0] != 0) {
      for (int i = 0; i < 6; i++) {
        mac[i] = mem_base_mac_8[i];
      }
    }
  }
}
*/

int e1000_attach(struct pci_dev *dev) {
  pci_dev_enable(dev);

  mmio_base = (u32 *)dev->bar_addr[0];
  mmio_size = dev->bar_size[0];

  printf("%08x\n", mmio_base[2]);

/*
  u8 mac[6];
  read_mac(dev, mac);

  printf("mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
*/

  return 0;
}

