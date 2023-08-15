#include <tinyos/drivers/pci.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/common/common.h>
#include <tinyos/kernel/hal.h>

/* Drivers */
#include <tinyos/drivers/e1000.h>

struct pci_driver {
  u16 venid;
  u16 devid;
  int (*attach)(struct pci_dev *dev);
};

static struct pci_driver pci_driver_table[] = {
  // { .venid = 0x8086, .devid = 0x10ea, .attach = &e1000_attach },
  // { .venid = 0x8086, .devid = 0x100e, .attach = &e1000_attach },
  { 0, 0, NULL }
};

static void pci_conf_set_addr(u8 bus, u8 dev, u8 func, u8 offset) {
  u32 addr = (1 << 31) |
             ((u32)(bus & 0xff) << 16) |
             ((u32)(dev & 0x1f) << 11) |
             ((u32)(func & 0x03) << 8) |
              (u32)(offset & 0xfc);
  outd(PCI_CONFIG_ADDR_PORT, addr);
}

u32 pci_conf_read(const struct pci_dev *dev, u32 pos) {
  u8 offset = PCI_REG_POS_OFFSET(pos);
  u8 low = PCI_REG_POS_LOW(pos);
  u8 high = PCI_REG_POS_HIGH(pos);

  pci_conf_set_addr(dev->bus, dev->dev, dev->func, offset);

  u32 data = ind(PCI_CONFIG_DATA_PORT);
  return (data >> low) & (((u64)1 << (high - low + 1)) - 1);
}

void pci_conf_write(const struct pci_dev *dev, u32 pos, u32 value) {
  u8 offset = PCI_REG_POS_OFFSET(pos);
  u8 low = PCI_REG_POS_LOW(pos);
  u8 high = PCI_REG_POS_HIGH(pos);

  u32 mask = ((1 << (high - low + 1)) - 1) << low;
  value <<= low;

  pci_conf_set_addr(dev->bus, dev->dev, dev->func, offset);
  u32 current = ind(PCI_CONFIG_DATA_PORT);
  value = (current & ~mask) | (value & mask);
  outd(PCI_CONFIG_DATA_PORT, value);
}

static void pci_attach_driver(struct pci_dev *dev) {
  struct pci_driver *driver;
  int r;

  for (driver = &pci_driver_table[0]; driver->attach != NULL; driver++) {
    if (dev->venid == driver->venid && dev->devid == driver->devid) {
      r = driver->attach(dev);
      if (r != 0) {
        printk("Failed to attach driver to device %04x:%04x\n",
          dev->venid, dev->devid);
      }
    }
  }
}

void pci_dev_enable(struct pci_dev *dev) {
  u32 bar;
  u32 addr, size;

  pci_conf_write(dev, PCI_REG_COMMAND,
                 PCI_COMMAND_MASTER_ENABLE |
                 PCI_COMMAND_IO_ENABLE |
                 PCI_COMMAND_MEM_ENABLE);

  for (bar = PCI_REG_BAR_START; bar < PCI_REG_BAR_END;
       bar = PCI_REG_POS_ADD_OFFSET(bar, 4)) {
    int regnum = PCI_REG_BAR_NUM(bar);

    u32 old = pci_conf_read(dev, bar);
    pci_conf_write(dev, bar, 0xffffffff);
    u32 cur = pci_conf_read(dev, bar);

    if (PCI_BAR_TYPE(old) == PCI_BAR_TYPE_MEM) {
      /* Memory space BAR. */
      if (PCI_BAR_MEM_TYPE(old) == PCI_BAR_MEM_TYPE_64) {
        /* Not supported. */
        bar = PCI_REG_POS_ADD_OFFSET(bar, 4);
        continue;
      }
      addr = PCI_BAR_MEM_ADDR(old);
      size = PCI_BAR_MEM_SIZE(cur);
    } else {
      /* IO space BAR. */
      addr = PCI_BAR_IO_ADDR(old);
      size = PCI_BAR_IO_SIZE(cur);
    }

    pci_conf_write(dev, bar, old);

    dev->bar_addr[regnum] = addr;
    dev->bar_size[regnum] = size;

    /*
    printk("%02x:%02x.%d (%04x:%04x) reg=%d base=0x%08x addr=0x%08x\n",
      dev->bus, dev->dev, dev->func, dev->venid, dev->devid, regnum, addr, size);
    */
  }
}

static void pci_probe_bus(u8 bus);

static void pci_probe_device(struct pci_dev *dev) {
  dev->venid = pci_conf_read(dev, PCI_REG_VENID);
  if (dev->venid == 0xffff) {
    return;  /* no device */
  }
  dev->devid = pci_conf_read(dev, PCI_REG_DEVID);
  dev->class = pci_conf_read(dev, PCI_REG_CLASS);
  dev->subclass = pci_conf_read(dev, PCI_REG_SUBCLASS);

  printk("%02x:%02x.%d %04x:%04x class=%02x subclass=%02x \n",
    dev->bus, dev->dev, dev->func, dev->venid, dev->devid,
    dev->class, dev->subclass);

  pci_attach_driver(dev);

  if (dev->class == PCI_DEV_BRIDGE && dev->subclass == PCI_DEV_BRIDGE_PCI2PCI) {
    pci_probe_bus(pci_conf_read(dev, PCI_REG_SEC_BUS));
  }
}

static void pci_probe_bus(u8 bus) {
  struct pci_dev dev;
  dev.bus = bus;

  for (dev.dev = 0; dev.dev < 32; dev.dev++) {
    dev.func = 0;
    u8 htype = pci_conf_read(&dev, PCI_REG_HEADER_TYPE);
    u8 max = ((htype & 0x80) == 0) ? 8 : 1;

    for (; dev.func < max; dev.func++) {
      pci_probe_device(&dev);
    }
  }
}

void pci_init(void) {
  pci_probe_bus(0);
}

