#include <drivers/pci.h>

#include <kernel/video.h>

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

static u32 pci_config_read_dword(u8 bus, u8 dev, u8 func, u8 offset) {
  u32 addr = (1 << 31) |
             ((u32)(bus & 0xff) << 16) |
             ((u32)(dev & 0x1f) << 11) |
             ((u32)(func & 0x03) << 8) |
              (u32)(offset & 0xfc);
  outd(PCI_CONFIG_ADDR_PORT, addr);
  return ind(PCI_CONFIG_DATA_PORT);
}

static inline u16 pci_get_vendor(u8 bus, u8 dev, u8 func) {
  return pci_config_read_dword(bus, dev, func, 0) & 0xffff;
}

static inline u16 pci_get_device(u8 bus, u8 dev, u8 func) {
  return pci_config_read_dword(bus, dev, func, 0) >> 16;
}

static inline u8 pci_get_class(u8 bus, u8 dev, u8 func) {
  return (pci_config_read_dword(bus, dev, func, 8) >> 24) & 0xff;
}

static inline u8 pci_get_subclass(u8 bus, u8 dev, u8 func) {
  return (pci_config_read_dword(bus, dev, func, 8) >> 16) & 0xff;
}

static inline u8 pci_get_header_type(u8 bus, u8 dev, u8 func) {
  return (pci_config_read_dword(bus, dev, func, 0xc) >> 16) & 0xff;
}

static inline u8 pci_get_secondary_bus(u8 bus, u8 dev, u8 func) {
  return (pci_config_read_dword(bus, dev, func, 0x18) >> 8) & 0xff;
}

static void pci_probe_bus(u8 bus);

static void pci_probe_func(u8 bus, u8 dev, u8 func) {
  u16 vendor = pci_get_vendor(bus, dev, func);
  if (vendor == 0xffff) {
    /* no device. */
    return;
  }

  u8 class = pci_get_class(bus, dev, func);
  u8 subclass = pci_get_subclass(bus, dev, func);
  u8 htype = pci_get_header_type(bus, dev, func);

  /* TODO: add device */

  printf("%2x:%2x:%2x ven: %x dev: %x class: %x sub: %x htype: %x\n",
    bus, dev, func, vendor, dev, class, subclass, htype);

  if (class == PCI_DEV_BRIDGE && subclass == PCI_DEV_BRIDGE_PCI2PCI) {
    pci_probe_bus(pci_get_secondary_bus(bus, dev, func));
  }
}

static void pci_probe_bus(u8 bus) {
  u8 func = 0;

  for (u8 dev = 0; dev < 32; dev++) {
    pci_probe_func(bus, dev, func);

    u8 htype = pci_get_header_type(bus, dev, func);
    if ((htype & 0x80) != 0) {
      /* multi-function device. */
      for (func++; func < 8; func++) {
        pci_probe_func(bus, dev, func);
      }
    }
   }
}

void pci_probe(void) {
  pci_probe_bus(0);
}

