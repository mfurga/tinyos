#include <drivers/pci.h>
#include <kernel/stdio.h>
#include <lib/common.h>
#include <lib/x86.h>

/* Drivers */
#include <drivers/e1000.h>

struct pci_driver {
  u16 venid;
  u16 devid;
  int (*attach)(const struct pci_dev *dev);
};

static struct pci_driver pci_driver_table[] = {
  { .venid = 0x8086, .devid = 0x100e, .attach = &e1000_attach },
  { 0, 0, NULL }
};

static u32 pci_config_read_dword(const struct pci_dev *dev, u8 offset) {
  u32 addr = (1 << 31) |
             ((u32)(dev->bus & 0xff) << 16) |
             ((u32)(dev->dev & 0x1f) << 11) |
             ((u32)(dev->func & 0x03) << 8) |
              (u32)(offset & 0xfc);
  outd(PCI_CONFIG_ADDR_PORT, addr);
  return ind(PCI_CONFIG_DATA_PORT);
}

static inline u16 pci_get_venid(const struct pci_dev *dev) {
  return pci_config_read_dword(dev, 0) & 0xffff;
}

static inline u16 pci_get_devid(const struct pci_dev *dev) {
  return pci_config_read_dword(dev, 0) >> 16;
}

static inline u8 pci_get_class(const struct pci_dev *dev) {
  return (pci_config_read_dword(dev, 8) >> 24) & 0xff;
}

static inline u8 pci_get_subclass(const struct pci_dev *dev) {
  return (pci_config_read_dword(dev, 8) >> 16) & 0xff;
}

static inline u8 pci_get_header_type(const struct pci_dev *dev) {
  return (pci_config_read_dword(dev, 0xc) >> 16) & 0xff;
}

static inline u8 pci_get_secondary_bus(const struct pci_dev *dev) {
  return (pci_config_read_dword(dev, 0x18) >> 8) & 0xff;
}

static void pci_attach_driver(const struct pci_dev *dev) {
  struct pci_driver *driver;
  int r;

  for (driver = &pci_driver_table[0]; driver->attach != NULL; driver++) {
    if (dev->venid == driver->venid && dev->devid == driver->devid) {
      r = driver->attach(dev);
      if (r != 0) {
        printf("Failed to attach driver to device (%02x, %02x)\n",
          dev->venid, dev->devid);
      }
    }
  }
}

static void pci_probe_bus(u8 bus);

static void pci_probe_device(struct pci_dev *dev) {
  dev->venid = pci_get_venid(dev);
  if (dev->venid == 0xffff) {
    return;  /* no device */
  }
  dev->devid = pci_get_devid(dev);
  dev->class = pci_get_class(dev);
  dev->subclass = pci_get_subclass(dev);

  pci_attach_driver(dev);

  if (dev->class == PCI_DEV_BRIDGE && dev->subclass == PCI_DEV_BRIDGE_PCI2PCI) {
    pci_probe_bus(pci_get_secondary_bus(dev));
  }
}

static void pci_probe_bus(u8 bus) {
  struct pci_dev dev;
  dev.bus = bus;

  for (dev.dev = 0; dev.dev < 32; dev.dev++) {
    dev.func = 0;
    u8 max = PCI_HDRTYPE_MULTIFUNC(&dev) ? 8 : 1;

    for (; dev.func < max; dev.func++) {
      pci_probe_device(&dev);
    }
  }
}

void pci_init(void) {
  pci_probe_bus(0);
}

