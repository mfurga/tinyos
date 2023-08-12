#include <drivers/e1000.h>
#include <kernel/stdio.h>
#include <kernel/assert.h>
#include <lib/string.h>

/* Receive Address Low */
#define RAL_OFFSET (0x05400 / 4)
#define RAL mmio_base[RAL_OFFSET]

/* Receive Address High */
#define RAH_OFFSET (0x05404 / 4)
#define RAH mmio_base[RAH_OFFSET]

/* Multicast Table Array */
#define MTA_OFFSET (0x05200 / 4)
#define MTA mmio_base[MTA_OFFSET]

/* Interrupt Mask Set/Read */
#define IMS_OFFSET (0x000D0 / 4)
#define IMS mmio_base[IMS_OFFSET]

/* Receive Descriptor Base Address Low */
#define RDBAL_OFFSET (0x02800 / 4)
#define RDBAL mmio_base[RDBAL_OFFSET]

/* Receive Descriptor Base Address High */
#define RDBAH_OFFSET (0x02804 / 4)
#define RDBAH mmio_base[RDBAH_OFFSET]

/* Receive Descriptor Length */
#define RDLEN_OFFSET (0x02808 / 4)
#define RDLEN mmio_base[RDLEN_OFFSET]

/* Receive Descriptor Head */
#define RDH_OFFSET (0x02810 / 4)
#define RDH mmio_base[RDH_OFFSET]

/* Receive Descriptor Tail */
#define RDT_OFFSET (0x02818 / 4)
#define RDT mmio_base[RDT_OFFSET]

/* Receive Control Register */
#define RCTL_OFFSET (0x00100 / 4)
#define RCTL mmio_base[RCTL_OFFSET]

/* Interrupt Cause Read Register */
#define ICR_OFFSET (0x000c0 / 4)
#define ICR mmio_base[ICR_OFFSET]

#define RAH_AV          (1 << 31)

#define RCTL_EN         (1 <<  1)
#define RCTL_BAM        (1 << 15)
#define RCTL_BSIZE_2048 (0 << 16)

#define BIT(pos) (1 << (pos))

#define RX_DESC_ARRAY_SIZE 32
#define RX_DESC_STATUS_DD  (1 << 0)
#define RX_DESC_STATUS_EOP (1 << 1)

static volatile u32 *mmio_base;
static u32 mmio_size;

static e1000_rx_desc_t rx_desc_array[RX_DESC_ARRAY_SIZE] ALIGNED(16);
static e1000_rx_buf_t rx_buf_array[RX_DESC_ARRAY_SIZE] ALIGNED(16);

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

static int e1000_rx_init(void) {
  /*
    RX initialization based on Section 14.4.
  */

  /*
    Program the Receive Address Register(s) (RAL/RAH) with the desired Ethernet
    addresses. RAL[0]/RAH[0] should always be used to store the Individual
    Ethernet MAC address of the Ethernet controller. This can come from the
    EEPROM or from any other means (for example, on some machines, this comes
    from the system PROM not the EEPROM on the adapter port).
  */

  /* 52:54:00:12:34:56 */
  RAL = 0x12005452;
  RAH = 0x00005634 | RAH_AV;

  /*
    Initialize the MTA (Multicast Table Array) to 0b. Per software, entries can
    be added to this table as desired.
  */
  for (int i = 0; i < 128; i++) {
    *(&MTA + i) = 0;
  }

  /*
    Program the Interrupt Mask Set/Read (IMS) register to enable any interrupt
    the software driver wants to be notified of when the event occurs.
    Suggested bits include RXT, RXO, RXDMT, RXSEQ, and LSC. There is no
    immediate reason to enable the transmit interrupts.
  */

  /* Done in e1000_interrupt_enable() */

  /*
    If software uses the Receive Descriptor Minimum Threshold Interrupt,
    the Receive Delay Timer (RDTR) register should be initialized with the
    desired delay time.
  */

  /* not used */

  /*
    Allocate a region of memory for the receive descriptor list. Software
    should insure this memory is aligned on a paragraph (16-byte) boundary.
    Program the Receive Descriptor Base Address (RDBAL/RDBAH) register(s)
    with the address of the region. RDBAL is used for 32-bit addresses
    and both RDBAL and RDBAH are used for 64-bit addresses.
  */
  assert((u32)&rx_desc_array % 16 == 0);
  memset(&rx_desc_array, 0, sizeof(rx_desc_array));

  RDBAL = (u32)&rx_desc_array;
  RDBAH = 0;

  /*
    Set the Receive Descriptor Length (RDLEN) register to the size (in bytes)
    of the descriptor ring. This register must be 128-byte aligned.
  */
  assert(sizeof(rx_desc_array) % 128 == 0);
  RDLEN = sizeof(rx_desc_array);

  /*
    Receive buffers of appropriate size should be allocated and pointers to
    these buffers should be stored in the receive descriptor ring.
  */
  for (int i = 0; i < RX_DESC_ARRAY_SIZE; i++) {
    e1000_rx_desc_t *desc = &rx_desc_array[i];
    desc->addr = (u64)&rx_buf_array[i];
  }

  /*
    Software initializes the Receive Descriptor Head (RDH) register and
    Receive Descriptor Tail (RDT) with the appropriate head and tail addresses.
    Head should point to the first valid receive descriptor in the descriptor
    ring and tail should point to one descriptor beyond the last valid
    descriptor in the descriptor ring.
  */
  RDH = 0;
  RDT = RX_DESC_ARRAY_SIZE - 1;

  /*
    Program the Receive Control (RCTL) register with appropriate values for
    desired operation.
  */
  RCTL = RCTL_EN | RCTL_BAM | RCTL_BSIZE_2048;

  return 0;
}

void e1000_interrupt_enable(void) {
  //IMS = 0x1F6DC;
  //IMS = 0xff & ~4;
  IMS = 0xffffffff;
}

int e1000_receive(u8 *buf, unsigned int size) {
  int idx = (RDT + 1) % RX_DESC_ARRAY_SIZE;
  e1000_rx_desc_t *desc = &rx_desc_array[idx];

  if (!(desc->status & RX_DESC_STATUS_DD)) {
    /* nothing to receive. */
    return -1;
  }

  if (desc->length > size) {
    /* buf too small. */
    return -2;
  }

  memcpy(buf, (void *)desc->addr, desc->length);

  /* The EOP bit should be set since every packet fits into one descriptor */
  assert((desc->status & RX_DESC_STATUS_EOP) > 0);

  /* Clear DD, EOP */
  desc->status &= ~RX_DESC_STATUS_DD;
  desc->status &= ~RX_DESC_STATUS_EOP;

  RDT = idx;
  return desc->length;
}

int e1000_attach(struct pci_dev *dev) {
  if (dev->func != 0) {
    return 0;
  }

  pci_dev_enable(dev);

  mmio_base = (u32 *)dev->bar_addr[0];
  mmio_size = dev->bar_size[0];

  e1000_interrupt_enable();
  e1000_rx_init();

/*
  u8 mac[6];
  read_mac(dev, mac);

  printf("mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
*/

  return 0;
}

